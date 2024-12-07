
#include "RendererGL.h"
#include <Renderer/Common.h>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <glm/gtx/quaternion.hpp>

#include <Utils/Log.h>

#include <Core/Material.h>
#include <Core/ShaderProgram.h>
#include <Core/Shader.h>
#include <Core/FileLoader.h>
#include <Core/Camera.h>
#include <Core/Transform.h>
#include <Renderer/MeshData.h>
#include <Renderer/BufferData.h>
#include <Renderer/TextureData.h>

namespace GaladHen
{
	GLenum RendererGL::PrimitiveTypes[3] =
	{
		GL_POINTS,
		GL_LINES,
		GL_TRIANGLES
	};

	GLenum RendererGL::TextureFormatAssociations[61] =
	{
		GL_RGB, // (int)TextureFormat::RGB
		GL_RGB8, // (int)TextureFormat::RGB8
		GL_SRGB, // (int)TextureFormat::SRGB
		GL_SRGB8 // (int)TextureFormat::SRGB8
	};

	GLenum RendererGL::PixelChannelsAssociations[4] =
	{
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_RGBA
	};

	GLenum RendererGL::PixelChannelDepthAssociations[19] =
	{
		GL_UNSIGNED_BYTE
	};

	GLint RendererGL::WrappingAssociations[4] =
	{
		GL_REPEAT,
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_EDGE,
		GL_MIRRORED_REPEAT
	};

	GLint RendererGL::FilteringAssociations[6] =
	{
		GL_LINEAR,
		GL_NEAREST,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR
	};

	GLenum RendererGL::TextureUnits[32] =
	{
		GL_TEXTURE0,
		GL_TEXTURE1,
		GL_TEXTURE2,
		GL_TEXTURE3,
		GL_TEXTURE4,
		GL_TEXTURE5,
		GL_TEXTURE6,
		GL_TEXTURE7,
		GL_TEXTURE8,
		GL_TEXTURE9,
		GL_TEXTURE10,
		GL_TEXTURE11,
		GL_TEXTURE12,
		GL_TEXTURE13,
		GL_TEXTURE14,
		GL_TEXTURE16,
		GL_TEXTURE17,
		GL_TEXTURE18,
		GL_TEXTURE19,
		GL_TEXTURE20,
		GL_TEXTURE21,
		GL_TEXTURE22,
		GL_TEXTURE23,
		GL_TEXTURE24,
		GL_TEXTURE25,
		GL_TEXTURE26,
		GL_TEXTURE27,
		GL_TEXTURE28,
		GL_TEXTURE29,
		GL_TEXTURE30,
		GL_TEXTURE31
	};

	GLenum RendererGL::BufferTypesAssociations[14]
	{
		GL_UNIFORM_BUFFER,
		GL_SHADER_STORAGE_BUFFER,
		GL_ARRAY_BUFFER,
		GL_ATOMIC_COUNTER_BUFFER,
		GL_COPY_READ_BUFFER,
		GL_COPY_WRITE_BUFFER,
		GL_DISPATCH_INDIRECT_BUFFER,
		GL_DRAW_INDIRECT_BUFFER,
		GL_ELEMENT_ARRAY_BUFFER,
		GL_PIXEL_PACK_BUFFER,
		GL_PIXEL_UNPACK_BUFFER,
		GL_QUERY_BUFFER,
		GL_TEXTURE_BUFFER,
		GL_TRANSFORM_FEEDBACK_BUFFER
	};

	GLenum RendererGL::BufferUsageAssociations[9]
	{
		 GL_STATIC_READ,
		 GL_STATIC_DRAW,
		 GL_STATIC_COPY,
		 GL_STREAM_READ,
		 GL_STREAM_DRAW,
		 GL_STREAM_COPY,
		 GL_DYNAMIC_READ,
		 GL_DYNAMIC_DRAW,
		 GL_DYNAMIC_COPY
	};

	RendererGL::RendererGL()
	{}

	void RendererGL::Init()
	{
		if (!gl3wInit())
		{
			Log::Error("RendererGL", "Error: GL3W failed to initialize the context");
		}
	}

	unsigned int RendererGL::CreateRenderBuffer(const TextureData& textureData)
	{
		unsigned int id = RenderBuffers.AddWithId();
		RenderBufferGLTest& rb = RenderBuffers.GetObjectWithId(id);

		glGenFramebuffers(1, &rb.FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID);

		TextureParameters params{};
		params.MinFiltering = TextureFiltering::Linear;
		params.MagFiltering = TextureFiltering::Linear;

		assert(textureData.AccessType == TextureAccessType::ReadWrite); // For a render target we want to be able to read and write from it

		rb.ColorTextureID = CreateTexture(textureData);
		rb.DepthStencilTextureID = CreateDepthStencilTexture(textureData.Width, textureData.Height);

		TextureGLTest& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);
		TextureGLTest& depthStencilTexture = Textures.GetObjectWithId(rb.DepthStencilTextureID);

		SetTextureParameters(colorTexture, params);
		SetTextureParameters(depthStencilTexture, params);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.TextureID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexture.TextureID, 0);

		// Check status
		glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID);
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			Log::Error("RendererGL", "Framebuffer is not complete!\n");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return 0;
	}

	void RendererGL::ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor)
	{
		RenderBufferGLTest& rb = RenderBuffers.GetObjectWithId(renderBufferID);
		TextureGLTest& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);

		glBindTexture(GL_TEXTURE_2D, colorTexture.TextureID);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID);
		GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		glClear(mask);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RendererGL::Draw(CommandBuffer<RenderCommand>& renderCommandBuffer)
	{
		// Material's parameters shouldn't be loaded here but in a MemoryTransferCommand send by higher level renderer. While we are not using bindless textures, however, we should do it here

		for (RenderCommand& rc : renderCommandBuffer)
		{
			MeshGLTest& mesh = Meshes.GetObjectWithId(rc.DataSourceID);

			GLuint program = Shaders.GetObjectWithId(rc.ShaderSourceID);
			glUseProgram(program);

			for (ShaderScalarData& data : rc.ShaderData.ScalarData)
			{
				glProgramUniform1f(program, glGetUniformLocation(program, data.DataName.data()), data.Scalar);
			}

			for (ShaderVec2Data& data : rc.ShaderData.Vec2Data)
			{
				glProgramUniform2f(program, glGetUniformLocation(program, data.DataName.data()), data.Vec2.x, data.Vec2.y);
			}

			for (ShaderVec3Data& data : rc.ShaderData.Vec3Data)
			{
				glProgramUniform3f(program, glGetUniformLocation(program, data.DataName.data()), data.Vec3.x, data.Vec3.y, data.Vec3.z);
			}

			for (ShaderVec4Data& data : rc.ShaderData.Vec4Data)
			{
				glProgramUniform4f(program, glGetUniformLocation(program, data.DataName.data()), data.Vec4.x, data.Vec4.y, data.Vec4.z, data.Vec4.w);
			}

			for (ShaderIntegerData& data : rc.ShaderData.IntegerData)
			{
				glProgramUniform1i(program, glGetUniformLocation(program, data.DataName.data()), data.Integer);
			}

			//unsigned int unit = 0;
			for (ShaderTextureData& data : rc.ShaderData.TextureData)
			{
				// select texture unit for the binded texture
				glActiveTexture(TextureUnits[data.TextureUnit]);
				glBindTexture(GL_TEXTURE_2D, data.TextureID);
				// create uniform sampler
				int loc = glGetUniformLocation(program, data.DataName.data());
				glUniform1i(loc, data.TextureUnit);

				//++unit;
			}

			std::vector<GLuint> locations;
			locations.reserve(rc.ShaderData.FunctionsData.size() + 1);
			//locations.emplace_back((GLuint)shadingMode);
			for (ShaderData& data : rc.ShaderData.FunctionsData)
			{
				locations.emplace_back(glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, data.DataName.data()));
			}
			// subroutine selection
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, locations.size(), locations.data());

			// Bind buffer data to shader pipeline
			for (ShaderBufferData& data : rc.ShaderData.BufferData)
			{
				BufferGLTest& buffer = Buffers.GetObjectWithId(data.BufferID);
				glBindBufferBase(buffer.Target, data.BufferBinding, buffer.BufferID);
			}

			// draw
			glBindVertexArray(mesh.VAO);
			glDrawElements(mesh.PrimitiveType, mesh.NumberOfIndices, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	void RendererGL::TransferData(CommandBuffer<MemoryTransferCommand>& memoryCommandBuffer)
	{
		for (MemoryTransferCommand& mtc : memoryCommandBuffer)
		{
			switch (mtc.TargetType)
			{
			case MemoryTargetType::Mesh:
			{
				// Load mesh request

				MeshData* meshData = static_cast<MeshData*>(mtc.Data);
				unsigned int meshID = mtc.MemoryTargetID;

				if (meshID == 0)
				{
					// New OpenGL resource
					meshID = CreateMesh(*meshData);
				}
				else
				{
					MeshGLTest& mesh = Meshes.GetObjectWithId(meshID);
					LoadMesh(mesh, *meshData);
				}

				break;
			}
			case MemoryTargetType::Buffer:
			{
				BufferData* bufferData = static_cast<BufferData*>(mtc.Data);
				unsigned int bufferID = mtc.MemoryTargetID;

				if (bufferID == 0)
				{
					// New OpenGL resource
					bufferID = CreateBuffer(*bufferData);
				}
				else
				{
					LoadBuffer(bufferID, *bufferData);
				}

				break;
			}
			case MemoryTargetType::Texture:
			{


				break;
			}

			default:
				break;
			}
		}
	}

	bool RendererGL::Compile(CommandBuffer<CompileCommand&> compileCommandBuffer)
	{
		for (CompileCommand cc : compileCommandBuffer)
		{
			unsigned int shaderID = cc.ShaderPipelineID;

			if (shaderID == 0)
			{
				// New OpenGL resource

			}
		}
	}

	RendererGL::~RendererGL()
	{

	}

	unsigned int RendererGL::CreateTexture(const TextureData& textureData)
	{
		unsigned int id = Textures.AddWithId();
		TextureGLTest& texture = Textures.GetObjectWithId(id);

		// create new texture object
		glGenTextures(1, &texture.TextureID);

		texture.AllocationType = TextureAllocationType::Mutable; // TODO: allocation type basing on access type
		texture.TextureFormat = TextureFormatAssociations[(int)textureData.Format];
		texture.PixelChannels = PixelChannelsAssociations[textureData.NumberOfChannels - 1];
		texture.PixelChannelDepth = PixelChannelDepthAssociations[0];
		texture.NumberOfMipMaps = textureData.NumberOfMipMaps;

		LoadTexture(texture, textureData);

		return id;
	}

	unsigned int RendererGL::CreateDepthStencilTexture(unsigned int width, unsigned int height)
	{
		unsigned int id = Textures.AddWithId();
		TextureGLTest& texture = Textures.GetObjectWithId(id);

		glGenTextures(1, &texture.TextureID);
		glBindTexture(GL_TEXTURE_2D, texture.TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return id;
	}

	void RendererGL::SetTextureParameters(const TextureGLTest& texture, const TextureParameters& params)
	{
		glBindTexture(GL_TEXTURE_2D, texture.TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrappingAssociations[(int)params.HorizontalWrapping]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrappingAssociations[(int)params.VerticalWrapping]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringAssociations[(int)params.MinFiltering]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringAssociations[(int)params.MagFiltering]);
	}

	void RendererGL::FreeTexture(unsigned int textureID)
	{
		TextureGLTest& texture = Textures.GetObjectWithId(textureID);

		glDeleteTextures(1, &texture.TextureID);

		Textures.RemoveWithId(textureID);
	}

	void RendererGL::LoadTexture(const TextureGLTest& texture, const TextureData& textureData)
	{
		// bind new texture object to texture target
		glBindTexture(GL_TEXTURE_2D, texture.TextureID);

		switch (texture.AllocationType)
		{
		case TextureAllocationType::Mutable:
		{
			glTexImage2D(GL_TEXTURE_2D, texture.NumberOfMipMaps, texture.TextureFormat, textureData.Width, textureData.Height, 0, texture.PixelChannels, texture.PixelChannelDepth, textureData.Data);
			break;
		}
		case TextureAllocationType::Immutable:
		{
			// allocate immutable storage basing on number of channels and on bit depth
			// IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
			// levels are the number of mipmaps
			glTexStorage2D(GL_TEXTURE_2D, texture.NumberOfMipMaps, texture.TextureFormat, textureData.Width, textureData.Height);

			// copy texture data to texture object
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureData.Width, textureData.Height, texture.PixelChannels, texture.PixelChannelDepth, textureData.Data);
		}
		default:
			break;
		}

		if (texture.NumberOfMipMaps > 0)
			glGenerateTextureMipmap(texture.TextureID);
	}

	unsigned int RendererGL::CreateMesh(const MeshData& meshData)
	{
		unsigned int id = Meshes.AddWithId();
		MeshGLTest mesh = Meshes.GetObjectWithId(id);
		mesh.NumberOfIndices = meshData.Indices.size();
		mesh.PrimitiveType = PrimitiveTypes[(int)meshData.PrimitiveType];
		
		// we create the buffers
		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
		glGenBuffers(1, &mesh.EBO);

		LoadMesh(mesh, meshData);

		return id;
	}

	void RendererGL::LoadMesh(MeshGLTest& mesh, const MeshData& meshData)
	{
		// VAO is made "active"
		glBindVertexArray(mesh.VAO);
		// we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, meshData.Vertices.size() * sizeof(VertexData), &meshData.Vertices[0], GL_STATIC_DRAW);
		// we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.Indices.size() * sizeof(GLuint), &meshData.Indices[0], GL_STATIC_DRAW);

		// we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
		// vertex positions
		// these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
		// Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Normal));
		// Texture Coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, UV));
		// Tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Tangent));
		// Bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Bitangent));
		// Vertex color
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Color));

		glBindVertexArray(0);

		mesh.NumberOfIndices = meshData.Indices.size();
	}

	void RendererGL::FreeMesh(unsigned int meshID)
	{
		MeshGLTest& mesh = Meshes.GetObjectWithId(meshID);

		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.EBO);

		Meshes.RemoveWithId(meshID);
	}

	unsigned int RendererGL::CreateShaderPipeline(CompileCommand& compileCommand)
	{
		unsigned int id = Shaders.AddWithId();

		GLuint& program = Shaders.GetObjectWithId(id);
		program = glCreateProgram();
		
		CompileShaderPipeline(id, compileCommand);
	}

	bool RendererGL::CompileShaderPipeline(unsigned int shaderID, CompileCommand& compileCommand)
	{
		// Compilation results
		char log[1000];
		compileCommand.Result.Succeed = true;

		GLuint& program = Shaders.GetObjectWithId(shaderID);

		const char* vCode = compileCommand.VertexCode.c_str();
		const char* tcCode = compileCommand.TessContCode.c_str();
		const char* teCode = compileCommand.TessEvalCode.c_str();
		const char* gCode = compileCommand.GeometryCode.c_str();
		const char* fCode = compileCommand.FragmentCode.c_str();

		// previous shader program delete
		FreeShaderPipeline(shaderID);

		// shader program creation
		program = glCreateProgram();

		// compile the shaders
		GLuint vShader, tcShader, teShader, gShader, fShader;

		// Vertex Shader
		if (compileCommand.VertexCode.length() > 0)
		{
			vShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vShader, 1, &vCode, NULL);
			glCompileShader(vShader);

			// check compilation errors
			if (!CheckShaderPipelineCompilation(vShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in vertex shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			glAttachShader(program, vShader);
		}

		// Tesselation control Shader
		if (compileCommand.TessContCode.length() > 0)
		{
			tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tcShader, 1, &tcCode, NULL);
			glCompileShader(tcShader);
			
			// check compilation errors
			if (!CheckShaderPipelineCompilation(tcShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in tesselation control shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			glAttachShader(program, tcShader);
		}

		// Tesselation evaluation Shader
		if (compileCommand.TessEvalCode.length() > 0)
		{
			teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(teShader, 1, &teCode, NULL);
			glCompileShader(teShader);

			// check compilation errors
			if (!CheckShaderPipelineCompilation(teShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in tesselation evaluation shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			glAttachShader(program, teShader);
		}

		// Geometry Shader
		if (compileCommand.GeometryCode.length() > 0)
		{
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &gCode, NULL);
			glCompileShader(gShader);
			
			// check compilation errors
			if (!CheckShaderPipelineCompilation(gShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in geometry shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			glAttachShader(program, gShader);
		}

		// Fragment Shader
		if (compileCommand.FragmentCode.length() > 0)
		{
			fShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fShader, 1, &fCode, NULL);
			glCompileShader(fShader);
			
			// check compilation errors
			if (!CheckShaderPipelineCompilation(fShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in fragment shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			glAttachShader(program, fShader);
		}

		// link
		glLinkProgram(program);

		// check linking errors
		if (!CheckShaderPipelineLinking(program, log, 1000))
		{
			std::string error = "[ERROR] Linking error for shader pipeline: ";
			error.append(log);
			error.append("\n");

			compileCommand.Result.Succeed = false;
			compileCommand.Result.Description.append(error);
		}

		// delete the shaders because they are linked to the Shader Program, and we do not need them anymore
		if (compileCommand.VertexCode.length() > 0)
		{
			glDeleteShader(vShader);
		}
		if (compileCommand.TessContCode.length() > 0)
		{
			glDeleteShader(tcShader);
		}
		if (compileCommand.TessEvalCode.length() > 0)
		{
			glDeleteShader(teShader);
		}
		if (compileCommand.GeometryCode.length() > 0)
		{
			glDeleteShader(gShader);
		}
		if (compileCommand.FragmentCode.length() > 0)
		{
			glDeleteShader(fShader);
		}

		return compileCommand.Result.Succeed;
	}

	bool RendererGL::CheckShaderPipelineCompilation(GLuint shaderProgram, char* outLog, unsigned int outLogLength)
	{
		GLint success;
		glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shaderProgram, outLogLength, NULL, outLog);
		}

		return success;
	}

	bool RendererGL::CheckShaderPipelineLinking(GLuint shaderProgram, char* outLog, unsigned int outLogLength)
	{
		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, outLogLength, NULL, outLog);
		}

		return success;
	}

	void RendererGL::FreeShaderPipeline(unsigned int shaderID)
	{
		glDeleteProgram(Shaders.GetObjectWithId(shaderID));

		Shaders.RemoveWithId(shaderID);
	}

	unsigned int RendererGL::CreateBuffer(const BufferData& bufferData)
	{
		unsigned int id = Buffers.AddWithId();
		BufferGLTest& buffer = Buffers.GetObjectWithId(id);
		glCreateBuffers(1, &buffer.BufferID);
		LoadBuffer(id, bufferData);
	}

	void RendererGL::LoadBuffer(unsigned int bufferID, const BufferData& bufferData)
	{
		// We assume data arrays are created correctly
		assert(bufferData.Sizes.size() == bufferData.Datas.size());

		// init
		//glBindBufferBase(bufferType, binding, Buffers.GetObjectWithId(id));
		BufferGLTest& buffer = Buffers.GetObjectWithId(bufferID);
		buffer.Target = BufferTypesAssociations[(int)bufferData.Type];
		glBindBuffer(buffer.Target, buffer.BufferID);

		// We need to calculate for each data its gpu occupancy (using std430 OpenGL buffer layout: https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec3.html)
		// Assuming the order of the data inside Datas array matches gpu buffer data order
		size_t totalSizeBytes = 0;
		for (unsigned int i = 0; i < bufferData.Datas.size(); ++i)
		{	
			totalSizeBytes += bufferData.Sizes[i];
		}
		glBufferData(buffer.Target, totalSizeBytes, nullptr, BufferUsageAssociations[(int)bufferData.AccessType]); // reallocation of memory

		// fill
		size_t offset = 0;
		for (unsigned int i = 0; i < bufferData.Sizes.size(); ++i)
		{
			glBufferSubData(buffer.Target, offset, bufferData.Sizes[i], bufferData.Datas[i]);
			offset += bufferData.Sizes[i];
		}
	}

	void RendererGL::FreeBuffer(unsigned int bufferID)
	{
		BufferGLTest& buffer = Buffers.GetObjectWithId(bufferID);
		glDeleteBuffers(1, &buffer.BufferID);

		Buffers.RemoveWithId(bufferID);
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

