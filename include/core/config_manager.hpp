#pragma once

#include <string>

class ConfigManager
{
public:
    bool load(const std::string& filename);

    std::string getHost() const;
    int getPort() const;
    int getTimeout() const;
    std::string getLogLevel() const;

private:
    std::string host_;
    int port_ = 0;
    int timeout_ = 0;
    std::string logLevel_;
};