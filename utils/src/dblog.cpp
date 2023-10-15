
#include <iostream>

#include <utils/dblog.h>

void dblog::error(const char* source, const char* message)
{
    std::cout << "[Error " << source << "] " << message << std::endl;
}

void dblog::error(const char* source, const std::string& message)
{
    std::cout << "[Error " << source << "] " << message << std::endl;
}

void dblog::error(const std::string& source, const char* message)
{
    std::cout << "[Error " << source << "] " << message << std::endl;
}

void dblog::error(const std::string& source, const std::string& message)
{
    std::cout << "[Error " << source << "] " << message << std::endl;
}
