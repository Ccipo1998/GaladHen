
#include <ezengine/shader.h>

Shader::Shader(const char* shaderPath, GLenum shaderType)
{
    std::string shaderCode;
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open file
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        // Read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // Convert stream into string
        shaderCode = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::string description = (std::string)shaderPath + " not succesfully read";
        Log::Error("Shader", e.what());
    }
    
    // Convert strings to char pointers
    const char* shaderCodePtr = shaderCode.c_str();

    // Step 2: we compile the shaders
    unsigned int shader;

    // Vertex Shader
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCodePtr, NULL);
    glCompileShader(shader);
    // check compilation errors
    CheckShaderCompilation(shader, shaderPath);

    // Step 3: Shader Program creation
    this->Program = glCreateProgram();
    glAttachShader(this->Program, shader);
    glLinkProgram(this->Program);
    // check linking errors
    CheckShaderLinking(this->Program);

    // Step 4: we delete the shaders because they are linked to the Shader Program, and we do not need them anymore
    glDeleteShader(shader);
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    // Step 1: we retrieve shaders source code from provided filepaths
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open file
        vShaderFile.open(vertexShaderPath);
        std::stringstream vShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::string description = (std::string)vertexShaderPath + " not succesfully read";
        Log::Error("Shader", e.what());
    }

    try
    {
        // Open file
        fShaderFile.open(fragmentShaderPath);
        std::stringstream fShaderStream;
        // Read file's buffer contents into streams
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        fShaderFile.close();
        // Convert stream into string
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::string description = (std::string)fragmentShaderPath + " not succesfully read";
        Log::Error("Shader", description);
    }

    // Convert strings to char pointers
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Step 2: we compile the shaders
    unsigned int vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // check compilation errors
    CheckShaderCompilation(vertex, vertexShaderPath);

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // check compilation errors
    CheckShaderCompilation(fragment, fragmentShaderPath);

    // Step 3: Shader Program creation
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // check linking errors
    CheckShaderLinking(this->Program);

    // Step 4: we delete the shaders because they are linked to the Shader Program, and we do not need them anymore
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::GetShaderProgram()
{
    return this->Program;
}

void Shader::Use()
{
    glUseProgram(this->Program);
}

void Shader::Delete()
{
    glDeleteProgram(this->Program);
}

// privates

void Shader::CheckShaderCompilation(GLuint shader, const char* shaderPath)
{
    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::string type = shaderPath;
        std::string description = ": shader compilation failed";
        Log::Error("Shader", (char*)infoLog);
    }
}

void Shader::CheckShaderLinking(GLuint program)
{
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        Log::Error("Shader", (char*)infoLog);
    }
}
