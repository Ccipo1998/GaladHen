
#include <iostream>

#include <utils/log.h>

void Log::Error(const char* source, const char* message)
{
    std::cout << "[ERROR from " << source << "] " << message << std::endl;
}

void Log::Error(const char* source, const std::string& message)
{
    std::cout << "[ERROR from " << source << "] " << message << std::endl;
}

void Log::Error(const std::string& source, const char* message)
{
    std::cout << "[ERROR from " << source << "] " << message << std::endl;
}

void Log::Error(const std::string& source, const std::string& message)
{
    std::cout << "[ERROR from " << source << "] " << message << std::endl;
}
