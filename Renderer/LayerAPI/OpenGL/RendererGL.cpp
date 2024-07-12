
#include "RendererGL.h"

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Utils/Log.h>

namespace GaladHen
{
	void RendererGL::Init()
	{
		// Init context

		if (!glfwInit())
		{
			Log::Error("RendererGL", "GLFW failed to initialize the context");

			return;
		}

		// setting the minimum required version of OpenGL
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		// core profile is a subset of OpenGL features (without the backward-compatible features)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// for MacOS:
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		if (!gl3wInit())
		{
			Log::Error("RendererGL", "Error: GL3W failed to initialize the context");

			return;
		}
	}

	unsigned int RendererGL::CreateLowLevelMesh()
	{
		Meshes.emplace(Meshes.size() + 1, MeshGL{});

		return Meshes.size();
	}

	void RendererGL::DestroyLowLevelMesh(unsigned int meshID)
	{
		Meshes.erase(meshID);
	}

	void RendererGL::LoadMeshDataIntoGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int meshID)
	{
		MeshGL& mesh = Meshes.at(meshID);
		mesh.LoadMemoryGPU(vertices, indices);
	}

	void RendererGL::EnableDepthTest(bool enable)
	{
		glEnable(GL_DEPTH_TEST);
	}
}

