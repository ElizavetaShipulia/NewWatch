#include "core/config_manager.hpp"
#include "core/logger.hpp"
#include "network/network_scanner.hpp"

#include <string>

int main()
{
    Logger::info("NetWatch started");

    ConfigManager config;

    if (!config.load("config.json"))
    {
        return 1;
    }

    Logger::info("Host: " + config.getHost());
    Logger::info("Port: " + std::to_string(config.getPort()));
    Logger::info("Timeout: " + std::to_string(config.getTimeout()));
    Logger::info("Log level: " + config.getLogLevel());

    NetworkScanner scanner;

    Logger::info(
        "Checking port " + std::to_string(config.getPort()) +
        " on " + config.getHost());

    ScanResult result = scanner.checkPort(
    config.getHost(),
    config.getPort(),
    config.getTimeout());

    switch (result)
    {
    case ScanResult::Open:
        Logger::info("Port is open");
        break;

    case ScanResult::ConnectionRefused:
        Logger::warning("Connection refused");
        break;

    case ScanResult::Timeout:
        Logger::warning("Connection timed out");
        break;

    case ScanResult::HostUnreachable:
        Logger::warning("Host is unreachable");
        break;

    case ScanResult::NetworkUnreachable:
        Logger::warning("Network is unreachable");
        break;

    case ScanResult::InvalidAddress:
        Logger::error("Invalid IP address");
        break;

    case ScanResult::SocketError:
        Logger::error("Socket error");
        break;
    }


    return 0;
}

