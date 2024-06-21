
#pragma once

#include <Renderer/LayerAPI/IShaderProgramAPI.h>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <string>

namespace GaladHen
{
    class ShaderProgramGL : public IShaderProgramAPI
    {

    public:

        ShaderProgramGL();

        // @brief
        // Set shader program as current active
        void Use();

        // @brief
        // Delete program from gpu memory
        void Delete();

        // @brief
        // Compile graphics pipeline's shaders and assign program
        // @param vertexCode: vertex shader code
        // @param tessContCode: tessellation control shader code
        // @param tessEvalCode: tessellation evaluation shader code
        // @param geometryCode: geometry shader code
        // @param fragmentCode: fragment shader code
        // @param [out]: info of compilation result
        virtual CompilationResult Compile(std::string& vertexCode, std::string& tessContCode, std::string& tessEvalCode, std::string& geometryCode, std::string& fragmentCode) override;

        // @brief
        // Compile compute shader and assign program
        // @param computeCode: compute shader code
        // @param [out]: info of compilation result
        virtual CompilationResult Compile(std::string& computeCode) override;

        //@brief
        // Load material data for shader in GPU memory
        void LoadShaderData(MaterialData& data) override;

        void SetShadingMode(ShadingMode mode) override;

        void Use() override;

    protected:

        bool CheckCompilation(GLuint shader, char* outLog);
        bool CheckLinking(GLuint program, char* outLog);

        GLuint Program;
    };
}
