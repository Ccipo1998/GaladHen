
#include "RendererGL.h"

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <glm/gtx/quaternion.hpp>

#include <Utils/Log.h>

#include <Core/Mesh.h>
#include <Core/Material.h>
#include <Core/Texture.h>
#include <Core/ShaderProgram.h>
#include <Core/Shader.h>
#include <Core/FileLoader.h>
#include <Core/Camera.h>
#include <Core/Transform.h>

namespace GaladHen
{
	RendererGL::RendererGL()
		: MeshIndex(0)
		, BufferIndex(0)
		, ShaderIndex(0)
		, TextureIndex(0)
		, PointLightBufferID(0)
		, DirectionalLightBufferID(0)
		, CameraDataUniformBufferID(0)
		, TransformDataUniformBufferID(0)
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

	void RendererGL::LoadMeshData(Mesh& mesh)
	{
		if (mesh.MeshID == 0)
		{
			mesh.MeshID= CreateLowLevelMesh();
		}

		Meshes[mesh.MeshID - 1].LoadMemoryGPU(mesh.Vertices, mesh.Indices);
	}

	void RendererGL::FreeMeshData(Mesh& mesh)
	{
		Meshes[mesh.MeshID - 1].FreeMemoryGPU();
	}

	void RendererGL::LoadLightingData(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights)
	{
		std::vector<PointLightData> pointLightDatas;
		TranslateToShaderData(pointLights, pointLightDatas);

		std::vector<DirectionalLightData> dirLightDatas;
		TranslateToShaderData(dirLights, dirLightDatas);

		std::vector<size_t> sizesBytes;
		sizesBytes.emplace_back(sizeof(size_t) * 2); // Because of 16 bytes alignment of the array of lights inside the ssbo (std140)
		sizesBytes.emplace_back(pointLightDatas.size() * sizeof(PointLightData));
		std::vector<void*> datas;
		size_t vectorSize = pointLightDatas.size();
		datas.emplace_back((void*)&vectorSize);
		datas.emplace_back((void*)pointLightDatas.data());

		if (PointLightBufferID == 0)
		{
			// first loading
			PointLightBufferID = LoadBufferData( GL_SHADER_STORAGE_BUFFER, 0, GL_STATIC_DRAW, sizesBytes, datas);
		}
		else
		{
			LoadBufferData(PointLightBufferID, GL_SHADER_STORAGE_BUFFER, 0, GL_STATIC_DRAW, sizesBytes, datas);
		}

		sizesBytes[1] = dirLightDatas.size() * sizeof(DirectionalLightData);
		vectorSize = dirLightDatas.size();
		datas[0] = (void*)&vectorSize;
		datas[1] = (void*)dirLightDatas.data();

		if (DirectionalLightBufferID == 0)
		{
			// first loading
			DirectionalLightBufferID = LoadBufferData(GL_SHADER_STORAGE_BUFFER, 1, GL_STATIC_DRAW, sizesBytes, datas);
		}
		else
		{
			LoadBufferData(DirectionalLightBufferID, GL_SHADER_STORAGE_BUFFER, 1, GL_STATIC_DRAW, sizesBytes, datas);
		}
	}

	void RendererGL::UpdateLightingData(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& dirLights)
	{
		std::vector<PointLightData> pointLightDatas;
		TranslateToShaderData(pointLights, pointLightDatas);

		std::vector<DirectionalLightData> dirLightDatas;
		TranslateToShaderData(dirLights, dirLightDatas);

		std::vector<size_t> sizesBytes;
		sizesBytes.emplace_back(sizeof(size_t) * 2); // Because of 16 bytes alignment of the array of lights inside the ssbo (std140)
		sizesBytes.emplace_back(pointLightDatas.size() * sizeof(PointLightData));
		std::vector<void*> datas;
		size_t vectorSize = pointLightDatas.size();
		datas.emplace_back((void*)&vectorSize);
		datas.emplace_back((void*)pointLightDatas.data());

		UpdateBufferData(PointLightBufferID, GL_SHADER_STORAGE_BUFFER, 0, GL_STATIC_DRAW, sizesBytes, datas);

		sizesBytes[1] = dirLightDatas.size() * sizeof(DirectionalLightData);
		vectorSize = dirLightDatas.size();
		datas[0] = (void*)&vectorSize;
		datas[1] = (void*)dirLightDatas.data();

		UpdateBufferData(DirectionalLightBufferID, GL_SHADER_STORAGE_BUFFER, 1, GL_STATIC_DRAW, sizesBytes, datas);
	}

	void RendererGL::FreeLightingData()
	{
		FreeBuffer(PointLightBufferID);
		FreeBuffer(DirectionalLightBufferID);
	}

	CompilationResult RendererGL::CompileShaderPipeline(ShaderPipeline& pipeline)
	{
		if (pipeline.ShaderProgramID == 0)
		{
			pipeline.ShaderProgramID = CreateLowLevelShaderProgram();
		}

		ShaderProgramGL& program = Shaders[pipeline.ShaderProgramID - 1];

		// Read files

		std::string vertex, tessCont, tessEval, geometry, fragment;

		if (pipeline.VertexShader)
		{
			vertex = FileLoader::ReadTextFile(pipeline.VertexShader->ShaderFilePath.data());
		}
		if (pipeline.TessContShader)
		{
			tessCont = FileLoader::ReadTextFile(pipeline.TessContShader->ShaderFilePath.data());
		}
		if (pipeline.TessEvalShader)
		{
			tessEval = FileLoader::ReadTextFile(pipeline.TessEvalShader->ShaderFilePath.data());
		}
		if (pipeline.GeometryShader)
		{
			geometry = FileLoader::ReadTextFile(pipeline.GeometryShader->ShaderFilePath.data());
		}
		if (pipeline.FragmentShader)
		{
			fragment = FileLoader::ReadTextFile(pipeline.FragmentShader->ShaderFilePath.data());
		}

		return program.Compile(vertex, tessCont, tessEval, geometry, fragment);
	}

	CompilationResult RendererGL::CompileComputeShader(ComputeShader& compute)
	{
		if (compute.ShaderProgramID == 0)
		{
			compute.ShaderProgramID = CreateLowLevelShaderProgram();
		}

		ShaderProgramGL& program = Shaders[compute.ShaderProgramID - 1];

		std::string computeCode;

		if (compute.CompShader)
		{
			computeCode = FileLoader::ReadTextFile(compute.CompShader->ShaderFilePath.data());
		}

		return program.CompileCompute(computeCode);
	}

	void RendererGL::FreeShaderProgram(ShaderProgram* program)
	{
		ShaderProgramGL& prog = Shaders[program->ShaderProgramID - 1];
		prog.Delete();

		DestroyLowLevelShaderProgram(program->ShaderProgramID);
	}

	void RendererGL::LoadTexture(Texture& texture)
	{
		if (texture.TextureID == 0)
		{
			texture.TextureID = CreateLowLevelTexture();
		}

		TextureGL& tex = Textures[texture.TextureID - 1];
		tex.LoadMemoryGPU(texture.GetTextureData(), texture.GetTextureWidth(), texture.GetTextureHeight(), texture.GetNumberOfChannels(), texture.GetTextureFormat(), texture.NumberOfMipMaps);
	}

	void RendererGL::FreeTexture(Texture& texture)
	{
		TextureGL& tex = Textures[texture.TextureID - 1];
		tex.FreeMemoryGPU();

		DestroyLowLevelTexture(texture.TextureID);
	}

	void RendererGL::LoadMaterialData(Material& material)
	{
		std::vector<TextureDataGL> texs;
		std::vector<MaterialDataTexture> matTexs = material.Data->GetTextureData();
		unsigned int texUnit = 0;
		for (MaterialDataTexture& texData : matTexs)
		{
			TextureDataGL dataGL{};
			if (Texture* pointedTex = texData.TexParams.TextureSource)
			{
				dataGL.TextureGLObject = &Textures[pointedTex->TextureID - 1];
				dataGL.Parameters = &texData.TexParams;
				dataGL.TextureUnit = texUnit;
				dataGL.SamplerName = texData.Name.data();
				texs.emplace_back(dataGL);
			}

			++texUnit;
		}

		ShaderProgramGL& program = Shaders[material.MaterialShader->ShaderProgramID - 1];
		program.LoadMaterialData(material.MaterialShadingMode, *material.Data, texs);
	}

	void RendererGL::LoadCameraData(Camera& camera)
	{
		CameraData data = TranslateToShaderData(camera);

		if (CameraDataUniformBufferID == 0)
		{
			// first loading
			CameraDataUniformBufferID = LoadBufferData(GL_UNIFORM_BUFFER, 0, GL_STATIC_DRAW, sizeof(CameraData), &data);
		}
		else
		{
			LoadBufferData(CameraDataUniformBufferID, GL_UNIFORM_BUFFER, 0, GL_STATIC_DRAW, sizeof(CameraData), &data);
		}
	}

	void RendererGL::UpdateCameraData(Camera& camera)
	{
		CameraData data = TranslateToShaderData(camera);

		UpdateBufferData(CameraDataUniformBufferID, GL_UNIFORM_BUFFER, 0, GL_STATIC_DRAW, 0, sizeof(CameraData), &data);
	}

	void RendererGL::LoadTransformData()
	{
		if (TransformDataUniformBufferID == 0)
		{
			// first loading
			TransformDataUniformBufferID = LoadBufferData(GL_UNIFORM_BUFFER, 1, GL_STATIC_DRAW, sizeof(TransformData), nullptr);
		}
		else
		{
			LoadBufferData(TransformDataUniformBufferID, GL_UNIFORM_BUFFER, 1, GL_STATIC_DRAW, sizeof(TransformData), nullptr);
		}
	}

	void RendererGL::UpdateTransformData(TransformQuat& transform)
	{
		TransformData data = TranslateToShaderData(transform);

		UpdateBufferData(TransformDataUniformBufferID, GL_UNIFORM_BUFFER, 1, GL_STATIC_DRAW, 0, sizeof(TransformData), &data);
	}

	void RendererGL::Draw(Mesh& mesh, Material& material)
	{
		MeshGL& meshGL = Meshes[mesh.MeshID - 1];
		ShaderProgramGL& shaderGL = Shaders[material.MaterialShader->ShaderProgramID - 1];

		LoadMaterialData(material);
		meshGL.Draw(&shaderGL, mesh.PrimitiveType);
	}

	unsigned int RendererGL::LoadBufferData(GLenum bufferType, GLuint binding, GLenum usage, size_t totalSizeBytes, void* data)
	{
		unsigned int id = CreateBuffer();

		glBindBufferBase(bufferType, binding, Buffers[id - 1]);
		glBufferData(bufferType, totalSizeBytes, data, usage);

		return id;
	}

	unsigned int RendererGL::LoadBufferData(GLenum bufferType, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*>& data)
	{
		unsigned int id = CreateBuffer();

		// init
		glBindBufferBase(bufferType, binding, Buffers[id - 1]);
		size_t totalSizeBytes = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			totalSizeBytes += sizesBytes[i];
		}
		glBufferData(bufferType, totalSizeBytes, nullptr, usage);

		// fill
		size_t offset = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			glBufferSubData(bufferType, offset, sizesBytes[i], data[i]);
			offset += sizesBytes[i];
		}

		return id;
	}

	void RendererGL::LoadBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, size_t totalSizeBytes, void* data)
	{
		DestroyBuffer(bufferID);
		unsigned int newID = CreateBuffer();

		assert(bufferID == newID); // If this fails -> something wrong in the "adding/removing ids from vectors" algorithm

		glBindBufferBase(bufferType, binding, Buffers[bufferID - 1]);
		glBufferData(bufferType, totalSizeBytes, data, usage);
	}

	void RendererGL::LoadBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*>& data)
	{
		DestroyBuffer(bufferID);
		unsigned int newID = CreateBuffer();

		assert(bufferID == newID); // If this fails -> something wrong in the "adding/removing ids from vectors" algorithm

		// init
		glBindBufferBase(bufferType, binding, Buffers[bufferID - 1]);
		size_t totalSizeBytes = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			totalSizeBytes += sizesBytes[i];
		}
		glBufferData(bufferType, totalSizeBytes, nullptr, usage);

		// fill
		size_t offset = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			glBufferSubData(bufferType, offset, sizesBytes[i], data[i]);
			offset += sizesBytes[i];
		}
	}

	void RendererGL::UpdateBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, size_t offset, size_t totalSizeBytes, void* newData)
	{
		glBindBufferBase(bufferType, binding, Buffers[bufferID - 1]);
		glBufferSubData(bufferType, offset, totalSizeBytes, newData);
	}

	void RendererGL::UpdateBufferData(unsigned int bufferID, GLenum bufferType, GLuint binding, GLenum usage, std::vector<size_t>& sizesBytes, std::vector<void*>& newDatas)
	{
		glBindBufferBase(bufferType, binding, Buffers[bufferID - 1]);
		size_t offset = 0;
		for (unsigned int i = 0; i < sizesBytes.size(); ++i)
		{
			glBufferSubData(bufferType, offset, sizesBytes[i], newDatas[i]);
			offset += sizesBytes[i];
		}
	}

	void RendererGL::FreeBuffer(unsigned int bufferID)
	{
		glDeleteBuffers(1, &Buffers[bufferID - 1]);
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
		*(unsigned int*)(&Meshes[meshID - 1]) = MeshIndex;
		MeshIndex = meshID - 1;
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

	unsigned int RendererGL::CreateBuffer()
	{
		unsigned int id;

		if (BufferIndex >= Buffers.size())
		{
			GLuint ssbo;
			glGenBuffers(1, &ssbo);
			Buffers.push_back(ssbo);

			id = BufferIndex++;
		}
		else
		{
			unsigned int next = *(unsigned int*)(&Buffers[BufferIndex]);

			glGenBuffers(1, &Buffers[BufferIndex]);
			id = BufferIndex;
			BufferIndex = next;
		}

		return id + 1;
	}

	void RendererGL::DestroyBuffer(unsigned int bufferID)
	{
		FreeBuffer(bufferID);

		*(unsigned int*)(&Buffers[bufferID - 1]) = BufferIndex;
		BufferIndex = bufferID - 1;
	}

	void RendererGL::TranslateToShaderData(const std::vector<PointLight>& pointLights, std::vector<PointLightData>& outLightData)
	{
		for (const PointLight& pl : pointLights)
		{
			outLightData.emplace_back(
				PointLightData
				{
					glm::vec4(pl.Color, 1.0),
					pl.Transform.GetPosition(),
					pl.Intensity,
					pl.BulbSize,
					pl.Radius
				});
		}
	}

	void RendererGL::TranslateToShaderData(const std::vector<DirectionalLight>& dirLights, std::vector<DirectionalLightData>& outLightData)
	{
		for (const DirectionalLight& dl : dirLights)
		{
			outLightData.emplace_back(
				DirectionalLightData
				{
					glm::vec4(dl.Color, 1.0),
					dl.Transform.GetPosition(),
					dl.Intensity,
					dl.GetDirection()
				});
		}
	}
	CameraData RendererGL::TranslateToShaderData(const Camera& camera)
	{
		return CameraData
		{
			camera.GetViewMatrix(),
			camera.GetProjectionMatrix(),
			camera.Transform.GetPosition()
		};
	}

	TransformData RendererGL::TranslateToShaderData(const TransformQuat& transform)
	{
		glm::mat4 ModelMatrix = transform.GetModelMatrix();
		glm::mat4 NormalMatrix = glm::inverse(glm::transpose(ModelMatrix));

		return TransformData
		{
			ModelMatrix,
			NormalMatrix
		};
	}
}

