
#include "RendererGL.h"

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Utils/Log.h>

#include <GaladHen/Material.h>
#include <GaladHen/TextureImage.h>

namespace GaladHen
{
	RendererGL::RendererGL()
		: MeshIndex(0)
		, BufferIndex(0)
		, LightingBufferID(0)
		, ShaderIndex(0)
		, TextureIndex(0)
	{}

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
		if (MeshIndex >= Meshes.size())
		{
			Meshes.push_back(MeshGL{});
			
			return ++MeshIndex;
		}

		unsigned int next = *(unsigned int*)(&Meshes[MeshIndex]);
		Meshes[MeshIndex] = MeshGL{};
		unsigned int old = MeshIndex;
		MeshIndex = next;

		return old + 1;
	}

	void RendererGL::DestroyLowLevelMesh(unsigned int meshID)
	{
		Meshes[meshID - 1].FreeMemoryGPU();

		*(unsigned int*)(&Meshes[meshID - 1]) = MeshIndex;
		MeshIndex = meshID - 1;
	}

	void RendererGL::LoadMeshDataIntoGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int meshID)
	{
		Meshes[meshID - 1].LoadMemoryGPU(vertices, indices);
	}

	void RendererGL::LoadLighingDataIntoGPU(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights)
	{
		std::vector<size_t> sizes;
		sizes.push_back(pointLights.size() * sizeof(PointLight));
		sizes.push_back(dirLights.size() * sizeof(DirectionalLight));

		std::vector<void*> datas;
		datas.push_back((void*)pointLights.data());
		datas.push_back((void*)dirLights.data());

		if (LightingBufferID == 0)
		{
			// Lighting data not created yet -> loading needed
			LoadBufferData(0, GL_STATIC_DRAW, sizes, datas);
		}
		else
		{
			// Lighting data already created -> update needed
			UpdateBufferData(LightingBufferID, 0, GL_STATIC_DRAW, sizes, datas);
		}
	}

	void RendererGL::FreeLightingDataFromGPU()
	{
		FreeBufferData(LightingBufferID);
	}

	unsigned int RendererGL::CreateLowLevelShaderProgram()
	{
		if (ShaderIndex >= Shaders.size())
		{
			Shaders.push_back(ShaderProgramGL{});

			return ++ShaderIndex;
		}

		unsigned int next = *(unsigned int*)(&Shaders[ShaderIndex]);
		Shaders[ShaderIndex] = ShaderProgramGL{};
		unsigned int old = ShaderIndex;
		ShaderIndex = next;

		return old + 1;
	}

	void RendererGL::DestroyLowLevelShaderProgram(unsigned int shaderID)
	{
		Shaders[shaderID - 1].Delete();

		*(unsigned int*)(&Shaders[shaderID - 1]) = ShaderIndex;
		ShaderIndex = shaderID - 1;
	}

	CompilationResult RendererGL::CompileShaderProgramPipeline(std::string& vertexCode, std::string& tessContCode, std::string& tessEvalCode, std::string& geometryCode, std::string& fragmentCode, unsigned int shaderID)
	{
		ShaderProgramGL& program = Shaders[shaderID - 1];
		return program.Compile(vertexCode, tessContCode, tessEvalCode, geometryCode, fragmentCode);
	}

	CompilationResult RendererGL::CompilerShaderProgram(std::string& computeCode, unsigned int shaderID)
	{
		ShaderProgramGL& program = Shaders[shaderID - 1];
		return program.CompileCompute(computeCode);
	}

	unsigned int RendererGL::CreateLowLevelTexture()
	{
		if (TextureIndex >= Textures.size())
		{
			Textures.push_back(TextureGL{});

			return ++TextureIndex;
		}

		unsigned int next = *(unsigned int*)(&Textures[TextureIndex]);
		Textures[TextureIndex] = TextureGL{};
		unsigned int old = TextureIndex;
		TextureIndex = next;

		return old + 1;
	}

	void RendererGL::DestroyLowLevelTexture(unsigned int textureID)
	{
		Textures[textureID - 1].FreeMemoryGPU();

		*(unsigned int*)(&Textures[textureID - 1]) = TextureIndex;
		TextureIndex = textureID - 1;
	}

	void RendererGL::LoadTextureIntoGPU(unsigned int textureID, const void* textureBytes, unsigned int width, unsigned int height, TextureFormat textureFormat, PixelDataFormat pixelFormat, PixelDataType pixelType, bool generateMipMaps)
	{
		TextureGL& tex = Textures[textureID - 1];
		tex.LoadMemoryGPU(textureBytes, width, height, textureFormat, pixelFormat, pixelType, generateMipMaps);
	}


	void RendererGL::EnableDepthTest(bool enable)
	{
		glEnable(GL_DEPTH_TEST);
	}

	void RendererGL::LoadMaterialData(unsigned int shaderID, Material& material)
	{
		std::vector<TextureDataGL> texs;
		for (MaterialDataTexture& tex : material.Data->GetTextureData())
		{
			TextureDataGL dataGL{};
			dataGL.Texture = &Textures[tex.Tex.TextureData->TextureID - 1];
			dataGL.Parameters = &tex.Tex.Parameters;
			texs.emplace_back(dataGL);
		}

		ShaderProgramGL& program = Shaders[shaderID - 1];
		program.LoadMaterialData(*material.Data, texs);
	}

	void Draw(unsigned int meshID, unsigned int shaderID)
	{

	}

	unsigned int RendererGL::LoadBufferData(GLuint binding, GLenum usage, size_t totalSizeBytes, void* data)
	{
		unsigned int index;

		if (BufferIndex >= Buffers.size())
		{
			GLuint ssbo;
			glGenBuffers(1, &ssbo);
			Buffers.push_back(ssbo);
			
			index = MeshIndex++;
		}
		else
		{
			unsigned int next = *(unsigned int*)(&Buffers[BufferIndex]);
			
			glGenBuffers(1, &Buffers[BufferIndex]);
			index = MeshIndex;
			BufferIndex = next;
		}

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, Buffers[index]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, totalSizeBytes, data, usage);

		return index + 1;
	}

	unsigned int RendererGL::LoadBufferData(GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*> data)
	{
		unsigned int index;

		if (BufferIndex >= Buffers.size())
		{
			GLuint ssbo;
			glGenBuffers(1, &ssbo);
			Buffers.push_back(ssbo);

			index = BufferIndex++;
		}
		else
		{
			unsigned int next = *(unsigned int*)(&Buffers[BufferIndex]);

			glGenBuffers(1, &Buffers[BufferIndex]);
			index = BufferIndex;
			BufferIndex = next;
		}

		// init
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, Buffers[index]);
		size_t totalSizeBytes = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			totalSizeBytes += sizesBytes[i];
		}
		glBufferData(GL_SHADER_STORAGE_BUFFER, totalSizeBytes, nullptr, usage);

		// fill
		size_t offset = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizesBytes[i], data[i]);
		}

		return index + 1;
	}

	void RendererGL::UpdateBufferData(unsigned int bufferID, GLuint binding, GLenum usage, size_t totalSizeBytes, void* newData)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buffers[bufferID - 1]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, totalSizeBytes, newData, usage);
	}

	void RendererGL::UpdateBufferData(unsigned int bufferID, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*> data)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buffers[bufferID - 1]);
		size_t totalSizeBytes = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			totalSizeBytes += sizesBytes[i];
		}
		glBufferData(GL_SHADER_STORAGE_BUFFER, totalSizeBytes, nullptr, usage);

		size_t offset = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizesBytes[i], data[i]);
		}
	}

	void RendererGL::FreeBufferData(unsigned int bufferID)
	{
		glDeleteBuffers(1, &Buffers[bufferID - 1]);
	}
}

