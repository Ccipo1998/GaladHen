
#pragma once

#include <string>

namespace Log
{
    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Error(const char* source, const char* message);

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Error(const char* source, const std::string& message);

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Error(const std::string& source, const std::string& message);

    /*
    @brief
    Print a message as an error
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Error(const std::string& source, const char* message);

    /*
    @brief
    Print a message as a warning
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Warning(const char* source, const char* message);

    /*
    @brief
    Print a message as a warning
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Warning(const char* source, const std::string& message);

    /*
    @brief
    Print a message as a warning
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Warning(const std::string& source, const std::string& message);

    /*
    @brief
    Print a message as a warning
    @param source: the source file or entity name
    @param message: the message to display
    */
    void Warning(const std::string& source, const char* message);

};
