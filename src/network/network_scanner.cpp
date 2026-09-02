#include "network/network_scanner.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
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

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) <= 0)
    {
        close(socketFd);
        return ScanResult::InvalidAddress;
    }

    timeval timeout{};
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;

    if (setsockopt(
            socketFd,
            SOL_SOCKET,
            SO_SNDTIMEO,
            &timeout,
            sizeof(timeout))
        == -1)
    {
        close(socketFd);
        return ScanResult::SocketError;
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

    int error = errno;

    close(socketFd);

    switch (error)
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
