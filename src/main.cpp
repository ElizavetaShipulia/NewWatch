#include "core/logger.hpp"

int main()
{
    Logger::info("NetWatch started");
    Logger::warning("Configuration file not found, using defaults");
    Logger::error("Demo error message");

    return 0;
}