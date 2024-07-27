
#include "ShaderProgramGL.h"

#include <glm/gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <Renderer/LayerAPI/OpenGL/MaterialDataGL.h>
#include <Renderer/LayerAPI/OpenGL/TextureGL.h>

#include <Core/Material.h>

namespace GaladHen
{
    ShaderProgramGL::ShaderProgramGL()
        : Program(0)
        {}

    CompilationResult ShaderProgramGL::Compile(std::string& vertexCode, std::string& tessContCode, std::string& tessEvalCode, std::string& geometryCode, std::string& fragmentCode)
    {
        // result
        CompilationResult res;

        // Convert strings to char pointers
        const char* vCode = vertexCode.c_str();
        const char* tcCode = tessContCode.c_str();
        const char* teCode = tessEvalCode.c_str();
        const char* gCode = geometryCode.c_str();
        const char* fCode = fragmentCode.c_str();

        // previous shader program delete
        if (Program)
            Delete();

        // shader program creation
        Program = glCreateProgram();

        // compile the shaders
        GLuint vShader, tcShader, teShader, gShader, fShader;

        // Vertex Shader
        if (vertexCode.length() > 0)
        {
            vShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vShader, 1, &vCode, NULL);
            glCompileShader(vShader);
            // check compilation errors
            res.vSuccess = CheckCompilation(vShader, res.vLog);

            glAttachShader(Program, vShader);
        }

        // Tesselation control Shader
        if (tessContCode.length() > 0)
        {
            tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tcShader, 1, &tcCode, NULL);
            glCompileShader(tcShader);
            // check compilation errors
            res.tcSuccess = CheckCompilation(tcShader, res.tcLog);

            glAttachShader(Program, tcShader);
        }

        // Tesselation evaluation Shader
        if (tessEvalCode.length() > 0)
        {
            teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(teShader, 1, &teCode, NULL);
            glCompileShader(teShader);
            // check compilation errors
            res.teSuccess = CheckCompilation(teShader, res.teLog);

            glAttachShader(Program, teShader);
        }

        // Geometry Shader
        if (geometryCode.length() > 0)
        {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &gCode, NULL);
            glCompileShader(gShader);
            // check compilation errors
            res.gSuccess = CheckCompilation(gShader, res.gLog);

            glAttachShader(Program, gShader);
        }

        // Fragment Shader
        if (fragmentCode.length() > 0)
        {
            fShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fShader, 1, &fCode, NULL);
            glCompileShader(fShader);
            // check compilation errors
            res.fSuccess = CheckCompilation(fShader, res.fLog);

            glAttachShader(Program, fShader);
        }

        // link
        glLinkProgram(Program);
        // check linking errors
        res.linkSuccess = CheckLinking(Program, res.linkLog);

        // delete the shaders because they are linked to the Shader Program, and we do not need them anymore
        if (vertexCode.length() > 0)
        {
            glDeleteShader(vShader);
        }
        if (tessContCode.length() > 0)
        {
            glDeleteShader(tcShader);
        }
        if (tessEvalCode.length() > 0)
        {
            glDeleteShader(teShader);
        }
        if (geometryCode.length() > 0)
        {
            glDeleteShader(gShader);
        }
        if (fragmentCode.length() > 0)
        {
            glDeleteShader(fShader);
        }

        return res;
    }

    CompilationResult ShaderProgramGL::CompileCompute(std::string& computeCode)
    {
        // result
        CompilationResult res;

        // Convert strings to char pointers
        const char* cCode = computeCode.c_str();

        // compile the shaders
        GLuint cShader;

        // Compute Shader
        cShader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(cShader, 1, &cCode, NULL);
        glCompileShader(cShader);
        // check compilation errors
        res.cSuccess = CheckCompilation(cShader, res.cLog);

        // previous shader program delete
        if (Program)
            Delete();

        // shader program creation
        Program = glCreateProgram();
        glAttachShader(Program, cShader);

        // link
        glLinkProgram(Program);
        // check linking errors
        res.linkSuccess = CheckLinking(Program, res.linkLog);

        // delete the shader because it is linked to the Shader Program, and we do not need it anymore
        glDeleteShader(cShader);

        return res;
    }

    void ShaderProgramGL::LoadMaterialData(MaterialData& data, std::vector<TextureDataGL>& textureData)
    {
        for (MaterialDataScalar& scalar : data.GetScalarData())
        {
            glProgramUniform1f(Program, glGetUniformLocation(Program, scalar.Name.data()), scalar.Scalar);
        }
        for (MaterialDataVector2& vec2 : data.GetVector2Data())
        {
            glProgramUniform2fv(Program, glGetUniformLocation(Program, vec2.Name.data()), 1, glm::value_ptr(glm::vec2(vec2.Vector.x, vec2.Vector.y)));
        }
        for (MaterialDataVector3& vec3 : data.GetVector3Data())
        {
            glProgramUniform3fv(Program, glGetUniformLocation(Program, vec3.Name.data()), 1, glm::value_ptr(glm::vec3(vec3.Vector.x, vec3.Vector.y, vec3.Vector.z)));
        }
        for (MaterialDataVector4& vec4 : data.GetVector4Data())
        {
            glProgramUniform4fv(Program, glGetUniformLocation(Program, vec4.Name.data()), 1, glm::value_ptr(glm::vec4(vec4.Vector.x, vec4.Vector.y, vec4.Vector.z, vec4.Vector.w)));
        }

        int texUnit = 0;
        for (TextureDataGL& tex : textureData)
        {
            TextureGL* texImage = tex.Texture;
            TextureParameters* params = tex.Parameters;
            
            // tex image should be already loaded when arriving here
            //assert(texImage->IsLoaded());

            texImage->LoadTextureParameters(this, texUnit, params->HorizontalWrapping, params->Filtering, params->MipMapMode); // TODO: here should be wrapping for both horizontal and vertical axes
            
        }
    }

    void ShaderProgramGL::SetShadingMode(ShadingMode mode)
    {
        // TODO
    }

    void ShaderProgramGL::Use()
    {
        glUseProgram(Program);
    }

    void ShaderProgramGL::Delete()
    {
        glDeleteProgram(Program);
    }

    bool ShaderProgramGL::CheckCompilation(GLuint shader, char* outLog)
    {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(shader, 1000, NULL, outLog);
        }

        return success;
    }

    bool ShaderProgramGL::CheckLinking(GLuint program, char* outLog)
    {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if(!success)
        {
            glGetProgramInfoLog(program, 1000, NULL, outLog);
        }

        return success;
    }

    GLuint ShaderProgramGL::GetShaderProgram()
    {
        return Program;
    }
}
