
#pragma once

#include <string>
#include <iostream>

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

    template<class T>
    void Error(const char* source, const char* format, T value)
    {
		int size_s = std::snprintf(nullptr, 0, format, value) + 1; // Extra space for '\0'
        if (size_s <= 0) return;
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<char[]> buf(new char[size]);
		std::snprintf(buf.get(), size, format, value);
		std::string error = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
        Log::Error(source, error);
    }

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
