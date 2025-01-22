
#include <fstream>
#include <sstream>
#include <iostream>

#include "FileLoader.h"
#include "Log.h"

namespace GaladHen
{
    std::string FileLoader::ReadTextFile(const std::string& filePath)
    {
        if (filePath.empty())
            return "";

        // ensure ifstream object can throw exceptions
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

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
        catch (const std::exception& e)
        {
            std::string description = std::string{ filePath } + " not succesfully read: " + e.what();
            Log::Error("FileLoader", description);
            return std::string{};
        }
    }
}
