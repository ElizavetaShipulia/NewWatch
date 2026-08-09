#include "network/network_scanner.hpp"

#include "core/logger.hpp"

void NetworkScanner::scan(const std::string& host)
{
    Logger::info("Starting scan of host: " + host);

    Logger::info("Scan finished");
}