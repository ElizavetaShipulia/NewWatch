#include "core/logger.hpp"

#include <iostream>

void Logger::info(const std::string& message)
{
    std::cout << "[INFO] " << message << '\n';
}

void Logger::warning(const std::string& message)
{
    std::cout << "[WARNING] " << message << '\n';
}

void Logger::error(const std::string& message)
{
    std::cerr << "[ERROR] " << message << '\n';
}