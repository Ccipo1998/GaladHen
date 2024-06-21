
#include "ShaderProgramGL.h"

#include <gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <GaladHen/Material.h>

namespace GaladHen
{
    ShaderProgramGL::ShaderProgramGL()
        : Program(0)
        {}

    void ShaderProgramGL::Use()
    {
        glUseProgram(Program);
    }

    void ShaderProgramGL::Delete()
    {
        glDeleteProgram(Program);
    }

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
    }

    void ShaderProgramGL::LoadShaderData(MaterialData& data)
    {
        std::vector<MaterialDataScalar> scalars = data.GetScalarData();
        std::vector<MaterialDataVector> vectors data.GetVectorData();
        std::vector<MaterialDataTexture> texs = data.GetTextureData();

        for (MaterialDataScalar& scalar : scalars)
        {
            glProgramUniform1f(Program, glGetUniformLocation(Program, scalar.Name.data()), scalar.Scalar);
        }

        for (MaterialDataVector& vec : vectors)
        {
            glProgramUniform3fv(Program, glGetUniformLocation(Program, vec.Name.data()), 1, value_ptr(vec.Vector));
        }

        for (MaterialDataTexture& tex : texs)
        {

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, )
            tex.Tex
            
            glProgramUniform1f(Program, glGetUniformLocation(Program, scalar.Name.data()), scalar.Scalar);
        }
    }

    void SetShadingMode(ShadingMode mode)
    {
        // TODO
    }

    void Use()
    {
        glUseProgram(Program);
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
}
