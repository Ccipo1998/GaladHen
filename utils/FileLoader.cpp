
#include "FileLoader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "Log.h"

namespace FileLoader
{
    std::string ReadTextFile(const char* filePath)
    {
        // ensure ifstream object can throw exceptions
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Open file
            file.open(filePath);
            
            std::stringstream stream;
            // Read file's buffer contents into streams
            stream << file.rdbuf();
            // close file handlers
            file.close();
            // Convert stream into string
            return stream.str();
        }
        catch(const std::exception& e)
        {
            std::string description = std::string{filePath} + " not succesfully read";
            Log::Error("FileLoader", e.what());
            return std::string{};
        }
    }
}
