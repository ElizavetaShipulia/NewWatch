#pragma once

#include <string>

enum class ScanResult
{
    Open,
    ConnectionRefused,
    Timeout,
    HostUnreachable,
    NetworkUnreachable,
    InvalidAddress,
    SocketError
};

class NetworkScanner
{
public:
    ScanResult checkPort(
        const std::string& host,
        int port,
        int timeoutMs) const;
};
