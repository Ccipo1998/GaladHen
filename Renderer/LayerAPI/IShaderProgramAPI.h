
// This is a pure virtual class (interface) to define basic functionalities of a shader program at API level

#pragma once

#include <string>
#include <vector>
#include <Core/MaterialData.h>
#include <Renderer/LayerAPI/OpenGL/MaterialDataGL.h>

namespace GaladHen
{
    struct CompilationResult
    {
        CompilationResult()
            : vSuccess(true)
            , tcSuccess(true)
            , teSuccess(true)
            , gSuccess(true)
            , fSuccess(true)
            , cSuccess(true)
            , linkSuccess(true)
        {}

        // @brief
        // True if the compilation was successful, false otherwise
        bool Success() { return vSuccess && tcSuccess && teSuccess && gSuccess && fSuccess && cSuccess && linkSuccess; }

        bool vSuccess;
        bool tcSuccess;
        bool teSuccess;
        bool gSuccess;
        bool fSuccess;
        bool cSuccess;
        bool linkSuccess;

        char vLog[1000];
        char tcLog[1000];
        char teLog[1000];
        char gLog[1000];
        char fLog[1000];
        char cLog[1000];
        char linkLog[1000];
    };

    class IShaderProgramAPI
    {

    public:

        virtual CompilationResult Compile(std::string& vertexCode, std::string& tessContCode, std::string& tessEvalCode, std::string& geometryCode, std::string& fragmentCode) = 0;
        
        virtual CompilationResult CompileCompute(std::string& computeCode) = 0;

        virtual void LoadMaterialData(MaterialData& data, std::vector<TextureDataGL>& textureData) = 0;

        virtual void SetShadingMode(ShadingMode mode) = 0;

        virtual void Use() = 0;

        virtual void Delete() = 0;
    };
}
