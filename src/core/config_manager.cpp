#include "core/config_manager.hpp"
#include "core/logger.hpp"

#include <fstream>

#include <json/json.hpp>

using json = nlohmann::json;

bool ConfigManager::load(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        Logger::error("Cannot open config file: " + filename);
        return false;
    }

    json config;
    file >> config;

    host_ = config.value("host", "localhost");
    port_ = config.value("port", 80);
    timeout_ = config.value("timeout", 3000);
    logLevel_ = config.value("logLevel", "info");

    Logger::info("Configuration loaded successfully");

    return true;
}

std::string ConfigManager::getHost() const
{
    return host_;
}

int ConfigManager::getPort() const
{
    return port_;
}

int ConfigManager::getTimeout() const
{
    return timeout_;
}

std::string ConfigManager::getLogLevel() const
{
    return logLevel_;
}