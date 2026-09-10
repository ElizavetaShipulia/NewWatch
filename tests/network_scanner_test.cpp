#include "network/network_scanner.hpp"

#include <gtest/gtest.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

TEST(NetworkScannerTest, InvalidAddress)
{
    NetworkScanner scanner;

    ScanResult result = scanner.checkPort(
        "999.999.999.999",
        8080,
        3000);

    EXPECT_EQ(result, ScanResult::InvalidAddress);
}

TEST(NetworkScannerTest, ConnectionRefused)
{
    NetworkScanner scanner;

    ScanResult result = scanner.checkPort(
        "127.0.0.1",
        8080,
        3000);

    EXPECT_EQ(result, ScanResult::ConnectionRefused);
}

TEST(NetworkScannerTest, OpenPort)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    ASSERT_NE(serverSocket, -1);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(0);

    ASSERT_EQ(
        inet_pton(
            AF_INET,
            "127.0.0.1",
            &address.sin_addr),
        1);

    ASSERT_EQ(
        bind(
            serverSocket,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)),
        0);

    socklen_t addressLength = sizeof(address);

    ASSERT_EQ(
        getsockname(
            serverSocket,
            reinterpret_cast<sockaddr*>(&address),
            &addressLength),
        0);

    int port = ntohs(address.sin_port);

    ASSERT_EQ(listen(serverSocket, 1), 0);

    NetworkScanner scanner;

    ScanResult result = scanner.checkPort(
        "127.0.0.1",
        port,
        3000);

    close(serverSocket);

    EXPECT_EQ(result, ScanResult::Open);
}