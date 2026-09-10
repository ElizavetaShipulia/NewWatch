#include "network/network_scanner.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

ScanResult NetworkScanner::checkPort(
    const std::string& host,
    int port,
    int timeoutMs) const
{
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketFd == -1)
    {
        return ScanResult::SocketError;
    }

    int flags = fcntl(socketFd, F_GETFL, 0);

    if (flags == -1)
    {
        close(socketFd);
        return ScanResult::SocketError;
    }

    if (fcntl(socketFd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        close(socketFd);
        return ScanResult::SocketError;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) <= 0)
    {
        close(socketFd);
        return ScanResult::InvalidAddress;
    }

    int result = connect(
        socketFd,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address));

    if (result == 0)
    {
        close(socketFd);
        return ScanResult::Open;
    }

    if (errno != EINPROGRESS)
    {
        int error = errno;

        close(socketFd);

        switch (error)
        {
            case ECONNREFUSED:
                return ScanResult::ConnectionRefused;

            case EHOSTUNREACH:
                return ScanResult::HostUnreachable;

            case ENETUNREACH:
                return ScanResult::NetworkUnreachable;

            default:
                return ScanResult::SocketError;
        }
    }

    fd_set writeSet;

    FD_ZERO(&writeSet);
    FD_SET(socketFd, &writeSet);

    timeval timeout{};
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;

    int selectResult = select(
        socketFd + 1,
        nullptr,
        &writeSet,
        nullptr,
        &timeout);

    if (selectResult == 0)
    {
        close(socketFd);
        return ScanResult::Timeout;
    }

    if (selectResult < 0)
    {
        close(socketFd);
        return ScanResult::SocketError;
    }

    int socketError = 0;
    socklen_t socketErrorLength = sizeof(socketError);

    if (getsockopt(
            socketFd,
            SOL_SOCKET,
            SO_ERROR,
            &socketError,
            &socketErrorLength)
        == -1)
    {
        close(socketFd);
        return ScanResult::SocketError;
    }

    if (socketError == 0)
    {
        close(socketFd);
        return ScanResult::Open;
    }

    close(socketFd);

    switch (socketError)
    {
        case ECONNREFUSED:
            return ScanResult::ConnectionRefused;

        case ETIMEDOUT:
            return ScanResult::Timeout;

        case EHOSTUNREACH:
            return ScanResult::HostUnreachable;

        case ENETUNREACH:
            return ScanResult::NetworkUnreachable;

        default:
            return ScanResult::SocketError;
    }
}