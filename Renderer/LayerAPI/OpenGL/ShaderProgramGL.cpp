
#include "ShaderProgramGL.h"

#include <glm/gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <Renderer/LayerAPI/IMaterialDataAPI.h>
#include <Renderer/LayerAPI/OpenGL/MaterialDataGL.h>
#include <Renderer/LayerAPI/OpenGL/TextureGL.h>

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

        // compile the shaders
        GLuint vShader, tcShader, teShader, gShader, fShader;

        // Vertex Shader
        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vCode, NULL);
        glCompileShader(vShader);
        // check compilation errors
        res.vSuccess = CheckCompilation(vShader, res.vLog);

        // Tesselation control Shader
        tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tcShader, 1, &tcCode, NULL);
        glCompileShader(tcShader);
        // check compilation errors
        res.tcSuccess = CheckCompilation(tcShader, res.tcLog);

        // Tesselation evaluation Shader
        teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(teShader, 1, &teCode, NULL);
        glCompileShader(teShader);
        // check compilation errors
        res.teSuccess = CheckCompilation(teShader, res.teLog);

        // Geometry Shader
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &gCode, NULL);
        glCompileShader(gShader);
        // check compilation errors
        res.gSuccess = CheckCompilation(gShader, res.gLog);

        // Tesselation control Shader
        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fCode, NULL);
        glCompileShader(fShader);
        // check compilation errors
        res.fSuccess = CheckCompilation(fShader, res.fLog);

        // previous shader program delete
        if (Program)
            Delete();

        // shader program creation
        Program = glCreateProgram();
        glAttachShader(Program, vShader);
        glAttachShader(Program, tcShader);
        glAttachShader(Program, teShader);
        glAttachShader(Program, gShader);
        glAttachShader(Program, fShader);

        // link
        glLinkProgram(Program);
        // check linking errors
        res.linkSuccess = CheckLinking(Program, res.linkLog);

        // delete the shaders because they are linked to the Shader Program, and we do not need them anymore
        glDeleteShader(vShader);
        glDeleteShader(tcShader);
        glDeleteShader(teShader);
        glDeleteShader(gShader);
        glDeleteShader(fShader);

        return res;
    }

    void ShaderProgramGL::LoadShaderData(IMaterialDataAPI* data)
    {
        MaterialDataGL* dataGL = static_cast<MaterialDataGL*>(data);
        for (MaterialScalarGL& scalar : dataGL->ScalarData)
        {
            glProgramUniform1f(Program, glGetUniformLocation(Program, scalar.Name), scalar.Scalar);
        }
        for (MaterialVector2GL& vec2 : dataGL->Vector2Data)
        {
            glProgramUniform2fv(Program, glGetUniformLocation(Program, vec2.Name), 1, glm::value_ptr(glm::vec2(vec2.Vector[0], vec2.Vector[1])));
        }
        for (MaterialVector3GL& vec3 : dataGL->Vector3Data)
        {
            glProgramUniform3fv(Program, glGetUniformLocation(Program, vec3.Name), 1, glm::value_ptr(glm::vec3(vec3.Vector[0], vec3.Vector[1], vec3.Vector[2])));
        }
        for (MaterialVector4GL& vec4 : dataGL->Vector4Data)
        {
            glProgramUniform4fv(Program, glGetUniformLocation(Program, vec4.Name), 1, glm::value_ptr(glm::vec4(vec4.Vector[0], vec4.Vector[1], vec4.Vector[2], vec4.Vector[3])));
        }

        int texUnit = 0;
        for (std::pair<TextureGL*, TextureParameters>& tex : dataGL->TextureData)
        {
            TextureGL* texImage = tex.first;
            TextureParameters& params = tex.second;
            
            // tex image must be already loaded when arriving here -> this is load shader data, for textures it is for parameters
            assert(texImage->IsLoaded());

            texImage->LoadTextureParameters(this, texUnit, params.HorizontalWrapping, params.Filtering, params.MipMapMode); // TODO: here should be wrapping for both horizontal and vertical axes
            
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
