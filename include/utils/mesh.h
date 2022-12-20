/*
Mesh class

VBO : Vertex Buffer Object - memory allocated on GPU memory to store the mesh data (vertices and their attributes, like e.g. normals, etc)
EBO : Element Buffer Object - a buffer maintaining the indices of vertices composing the mesh faces
VAO : Vertex Array Object - a buffer that helps to "manage" VBO and its inner structure. It stores pointers to the different vertex attributes stored in the VBO. When we need to render an object, we can just bind the corresponding VAO, and all the needed calls to set up the binding between vertex attributes and memory positions in the VBO are automatically configured.

Model and Mesh classes follow RAII principles (https://en.cppreference.com/w/cpp/language/raii).
Mesh class is in charge of releasing the allocated GPU buffers, and
it is a "move-only" class. A move-only class ensures that you always have a 1:1 relationship between the total number of resources being created and the total number of actual instantiations occurring.
Moreover, we want to have, CPU-side, a Mesh instance, with associated GPU resources, which is responsible of their life cycle (RAII), and which could be "moved" in memory keeping the ownership of its resources
*/

#include <vector>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

struct Vertex
{
    vec3 Position;
    vec3 Normal;
    vec2 UV;
    vec3 Tangent;
    vec3 Bitangent;
};

// mesh class for GPU usage (with indices and without other type of structure like half-edges)
class Mesh
{
    public:

    vector<Vertex> Vertices;
    vector<GLuint> Indices;
    GLuint VAO;

    // Constructors

    // We want Mesh to be a move-only class. We delete copy constructor and copy assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/constructors-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/copy_constructor
    // https://en.cppreference.com/w/cpp/language/copy_assignment
    // https://www.geeksforgeeks.org/preventing-object-copy-in-cpp-3-different-ways/
    // Section 4.6 of the "A Tour in C++" book
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;

    // We use initializer list and std::move in order to avoid a copy of the arguments
    // This constructor empties the source vectors (vertices and indices)
    Mesh(vector<Vertex>& vertices, vector<GLuint>& indices) noexcept
        : Vertices(move(vertices)), Indices(move(indices))
    {
        this->initGPUmemory();
    }

    // We implement a user-defined move constructor and move assignment
    // see:
    // https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
    // https://en.cppreference.com/w/cpp/language/move_constructor
    // https://en.cppreference.com/w/cpp/language/move_assignment
    // https://www.learncpp.com/cpp-tutorial/15-1-intro-to-smart-pointers-move-semantics/
    // https://www.learncpp.com/cpp-tutorial/15-3-move-constructors-and-move-assignment/
    // Section 4.6 of the "A Tour in C++" book

    // Move constructor
    // The source object of a move constructor is not expected to be valid after the move.
    // In our case it will no longer imply ownership of the GPU resources and its vectors will be empty.
    Mesh(Mesh&& other) noexcept
        // Calls move for both vectors, which internally consists of a simple pointer swap between the new instance and the source one.
        : Vertices(move(other.Vertices)), Indices(move(other.Indices)),
        VAO(other.VAO), VBO(other.VBO), EBO(other.EBO)
    {
        other.VAO = 0; // We *could* set VBO and EBO to 0 too,
        // but since we bring all the 3 values around we can use just one of them to check ownership of the 3 resources.
    }

    // Move assignment
    Mesh& operator=(Mesh&& other) noexcept
    {
        // skip if we are trying to assign an object to itself
        if (this == &other)
            return;

        // calls the function which will delete (if needed) the GPU resources for this instance
        this->freeGPUresources();

        if (other.VAO) // source instance has GPU resources
        {
            Vertices = move(other.Vertices);
            Indices = move(other.Indices);
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;

            other.VAO = 0;
        }
        else // source instance was already invalid
        {
            VAO = 0;
        }
        
        return *this;
    }

    // destructor
    ~Mesh() noexcept
    {
        // calls the function which will delete (if needed) the GPU resources
        this->freeGPUresources();
    }

    private:
    
    GLuint VBO, EBO;

    // initialization and allocation of mesh data to GPU memory
    // https://learnopengl.com/#!Getting-started/Hello-Triangle
    void initGPUmemory()
    {
        // we create the buffers
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        // VAO is made "active"
        glBindVertexArray(this->VAO);
        // we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(Vertex), &this->Vertices[0], GL_STATIC_DRAW);
        // we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(GLuint), &this->Indices[0], GL_STATIC_DRAW);

        // we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
        // vertex positions
        // these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Texture Coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, UV));
        // Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
        // Bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }

    void freeGPUresources()
    {
        // If VAO is 0, this instance of Mesh has been through a move, and no longer owns GPU resources,
        // so there's no need for deleting.
        if (VAO)
        {
            glDeleteVertexArrays(1, &this->VAO);
            glDeleteBuffers(1, &this->VBO);
            glDeleteBuffers(1, &this->EBO);
        }
    }
};
