
// Mesh class

// VBO : Vertex Buffer Object - memory allocated on GPU memory to store the mesh data (vertices and their attributes, like e.g. normals, etc)
// EBO : Element Buffer Object - a buffer maintaining the indices of vertices composing the mesh faces
// VAO : Vertex Array Object - a buffer that helps to "manage" VBO and its inner structure. It stores pointers to the different vertex attributes stored in the VBO. When we need to render an object, we can just bind the corresponding VAO, and all the needed calls to set up the binding between vertex attributes and memory positions in the VBO are automatically configured.

// Model and Mesh classes follow RAII principles (https://en.cppreference.com/w/cpp/language/raii).
// Mesh class is in charge of releasing the allocated GPU buffers, and
// it is a "move-only" class. A move-only class ensures that you always have a 1:1 relationship between the total number of resources being created and the total number of actual instantiations occurring.
// Moreover, we want to have, CPU-side, a Mesh instance, with associated GPU resources, which is responsible of their life cycle (RAII), and which could be "moved" in memory keeping the ownership of its resources

#pragma once

#include <vector>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

// mesh class for GPU usage (with indices and without other type of structure like half-edges)
class Mesh
{
    
public:

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
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) noexcept;

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
    Mesh(Mesh&& other) noexcept;

    // Move assignment
    Mesh& operator=(Mesh&& other) noexcept;

    // destructor
    ~Mesh() noexcept;

    // @brief
    // Draw call for the mesh
    void Draw();

protected:

    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;
    GLuint VAO, VBO, EBO;

    // @brief
    // Initialization and allocation of mesh data to GPU memory
    // https://learnopengl.com/#!Getting-started/Hello-Triangle
    void InitGPUmemory();

    // @brief
    // To delete and free gpu memory from mesh data
    void FreeGPUresources();
};
