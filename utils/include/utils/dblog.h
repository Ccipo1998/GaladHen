
#pragma once

#include <string>

namespace dblog
{

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void error(const char* source, const char* message);

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void error(const char* source, const std::string& message);

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void error(const std::string& source, const std::string& message);

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void error(const std::string& source, const char* message);

};
