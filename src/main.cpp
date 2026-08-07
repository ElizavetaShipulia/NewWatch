#include "core/config_manager.hpp"
#include "core/logger.hpp"

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

    return 0;
}