
#include "RendererGL.h"
#include <Renderer/Common.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <glm/gtx/quaternion.hpp>

#include <Utils/Log.h>

#include <Renderer/GPUResourceInspector.h>
#include <Renderer/Entities/Texture.h>
#include <Renderer/Entities/Mesh.h>
#include <Renderer/Entities/Buffer.h>

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
		if (gl3wInit() != GL3W_OK)
		{
			Log::Error("RendererGL", "Error: GL3W failed to initialize the context");

			return;
		}
	}

	unsigned int RendererGL::CreateRenderBuffer(unsigned int width, unsigned int height)
	{
		unsigned int id = RenderBuffers.AddWithId();
		RenderBufferGLTest& rb = RenderBuffers.GetObjectWithId(id);

		glGenFramebuffers(1, &rb.FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID);

		Texture texture{ nullptr, width, height, 4, 0, TextureFormat::RGB8 };
		texture.SetFiltering(TextureFiltering::Linear);

		//assert(textureData.AccessType == TextureAccessType::ReadWrite); // For a render target we want to be able to read and write from it

		rb.ColorTextureID = CreateTexture(texture);
		rb.DepthStencilTextureID = CreateDepthStencilTexture(width, height);

		TextureGLTest& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);
		TextureGLTest& depthStencilTexture = Textures.GetObjectWithId(rb.DepthStencilTextureID);

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

		return id;
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

			std::shared_ptr<Material> material = std::shared_ptr<Material>(rc.Material);

			for (auto& scalar : material->ScalarData)
			{
				glProgramUniform1f(program, glGetUniformLocation(program, scalar.first.data()), scalar.second);
			}

			for (auto& vec2 : material->Vec2Data)
			{
				glProgramUniform2f(program, glGetUniformLocation(program, vec2.first.data()), vec2.second.x, vec2.second.y);
			}

			for (auto& vec3 : material->Vec3Data)
			{
				glProgramUniform3f(program, glGetUniformLocation(program, vec3.first.data()), vec3.second.x, vec3.second.y, vec3.second.z);
			}

			for (auto& vec4 : material->Vec4Data)
			{
				glProgramUniform4f(program, glGetUniformLocation(program, vec4.first.data()), vec4.second.x, vec4.second.y, vec4.second.z, vec4.second.w);
			}

			//for (ShaderIntegerData& data : rc.ShaderData.IntegerData)
			//{
			//	glProgramUniform1i(program, glGetUniformLocation(program, data.DataName.data()), data.Integer);
			//}

			unsigned int unit = 0;
			for (auto& texture : material->TextureData)
			{
				// select texture unit for the binded texture
				glActiveTexture(TextureUnits[unit]);
				glBindTexture(GL_TEXTURE_2D, GPUResourceInspector::GetResourceID(texture.second.get()));
				// create uniform sampler
				int loc = glGetUniformLocation(program, texture.first.data());
				glUniform1i(loc, unit);

				++unit;
			}

			std::vector<GLuint> locations;
			locations.reserve(material->FunctionsData.size() + 1);
			//locations.emplace_back((GLuint)shadingMode);
			for (std::string& function : material->FunctionsData)
			{
				locations.emplace_back(glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, function.data()));
			}
			// subroutine selection
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, locations.size(), locations.data());

			// Bind buffer data to shader pipeline
			for (auto& buffer : material->BufferData)
			{
				BufferGLTest& bufferGL = Buffers.GetObjectWithId(GPUResourceInspector::GetResourceID(buffer.second.get()));
				GLuint ssbIndex = glGetProgramResourceIndex(program, bufferGL.ResourceProgramInterface, buffer.first.data());
				glBindBufferBase(bufferGL.Target, ssbIndex, bufferGL.BufferID);
			}
			for (auto& buffer : rc.AdditionalBufferData)
			{
				BufferGLTest& bufferGL = Buffers.GetObjectWithId(GPUResourceInspector::GetResourceID(buffer.second.get()));
				GLuint ssbIndex = glGetProgramResourceIndex(program, bufferGL.ResourceProgramInterface, buffer.first.data());
				glBindBufferBase(bufferGL.Target, ssbIndex, bufferGL.BufferID);
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
				Mesh* mesh = static_cast<Mesh*>(mtc.Data);
				unsigned int& meshID = mtc.MemoryTargetID; // update memory target id

				switch (mtc.TransferType)
				{
				case MemoryTransferType::Load:

					if (meshID == 0)
					{
						// New OpenGL resource
						meshID = CreateMesh(*mesh);
					}
					else
					{
						LoadMesh(meshID, *mesh);
					}

					break;

				case MemoryTransferType::Free:

					FreeMesh(meshID);

					break;

				default:
					break;
				}

				break;
			}
			case MemoryTargetType::Buffer:
			{
				Buffer* buffer = static_cast<Buffer*>(mtc.Data);
				unsigned int& bufferID = mtc.MemoryTargetID; // update memory target id

				switch (mtc.TransferType)
				{
				case MemoryTransferType::Load:

					if (bufferID == 0)
					{
						// New OpenGL resource
						bufferID = CreateBuffer(*buffer);
					}
					else
					{
						LoadBuffer(bufferID, *buffer);
					}

					break;

				case MemoryTransferType::Free:

					FreeBuffer(bufferID);

					break;

				default:
					break;
				}

				break;
			}
			case MemoryTargetType::Texture:
			{
				Texture* texture = static_cast<Texture*>(mtc.Data);
				unsigned int& textureID = mtc.MemoryTargetID; // update memory target id

				if (textureID == 0)
				{
					// New opengl resource
					textureID = CreateTexture(*texture);
				}
				else
				{
					LoadTexture(textureID, *texture);
				}

				break;
			}

			default:
				break;
			}
		}
	}

	bool RendererGL::Compile(CommandBuffer<CompileCommand>& compileCommandBuffer)
	{
		bool success = true;

		for (CompileCommand& cc : compileCommandBuffer)
		{
			unsigned int& shaderID = cc.ShaderPipelineID;

			if (shaderID == 0)
			{
				// New OpenGL resource
				shaderID = CreateShaderPipeline(cc);
			}
			else
			{
				CompileShaderPipeline(shaderID, cc);
			}

			success &= cc.Result.Succeed;
		}

		return success;
	}

	void RendererGL::EnableDepthTest(bool enable)
	{
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	unsigned int RendererGL::GetTextureApiID(unsigned int resourceID)
	{
		TextureGLTest& texture = Textures.GetObjectWithId(resourceID);

		return texture.TextureID;
	}

	RendererGL::~RendererGL()
	{

	}

	unsigned int RendererGL::CreateTexture(const Texture& texture)
	{
		unsigned int id = Textures.AddWithId();
		TextureGLTest& textureGL = Textures.GetObjectWithId(id);

		// create new texture object
		glGenTextures(1, &textureGL.TextureID);

		LoadTexture(id, texture);

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

	void RendererGL::FreeTexture(unsigned int textureID)
	{
		TextureGLTest& texture = Textures.GetObjectWithId(textureID);

		glDeleteTextures(1, &texture.TextureID);

		Textures.RemoveWithId(textureID);
	}

	void RendererGL::LoadTexture(unsigned int textureID, const Texture& texture)
	{
		TextureGLTest& textureGL = Textures.GetObjectWithId(textureID);

		// Copy texture data in opengl texture data
		textureGL.AllocationType = TextureAllocationType::Mutable; // TODO: allocation basing on texture access type
		textureGL.Filtering = FilteringAssociations[(int)texture.GetFiltering()];
		textureGL.Wrapping = WrappingAssociations[(int)texture.GetWrapping()];
		textureGL.TextureFormat = TextureFormatAssociations[(int)texture.GetFormat()];
		textureGL.PixelChannels = PixelChannelsAssociations[texture.GetNumberOfChannels() - 1];
		textureGL.PixelChannelDepth = PixelChannelDepthAssociations[0];
		textureGL.NumberOfMipMaps = texture.GetNumberOfMipMaps();

		// bind new texture object to texture target
		glBindTexture(GL_TEXTURE_2D, textureGL.TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureGL.Wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureGL.Wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureGL.Filtering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureGL.Filtering);

		switch (textureGL.AllocationType)
		{
		case TextureAllocationType::Mutable:
		{
			glm::uvec2 textureSize;
			texture.GetSize(textureSize);
			std::shared_ptr<unsigned char> data = texture.GetData().lock(); // need to convert to shared ptr to use a weak ptr
			glTexImage2D(GL_TEXTURE_2D, textureGL.NumberOfMipMaps, textureGL.TextureFormat, textureSize.x, textureSize.y, 0, textureGL.PixelChannels, textureGL.PixelChannelDepth, data.get());
			break;
		}
		case TextureAllocationType::Immutable:
		{
			// allocate immutable storage basing on number of channels and on bit depth
			// IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
			// levels are the number of mipmaps
			glm::uvec2 textureSize;
			texture.GetSize(textureSize);
			glTexStorage2D(GL_TEXTURE_2D, textureGL.NumberOfMipMaps, textureGL.TextureFormat, textureSize.x, textureSize.y);

			// copy texture data to texture object
			std::shared_ptr<unsigned char> data = texture.GetData().lock();
			assert(data.get() != nullptr); // always valid texture arrives here
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureSize.x, textureSize.y, textureGL.PixelChannels, textureGL.PixelChannelDepth, data.get());
		}
		default:
			break;
		}

		if (textureGL.NumberOfMipMaps > 0)
			glGenerateTextureMipmap(textureGL.TextureID);
	}

	unsigned int RendererGL::CreateMesh(const Mesh& mesh)
	{
		unsigned int id = Meshes.AddWithId();
		MeshGLTest& meshGL = Meshes.GetObjectWithId(id);

		// we create the buffers
		glGenVertexArrays(1, &meshGL.VAO);
		glGenBuffers(1, &meshGL.VBO);
		glGenBuffers(1, &meshGL.EBO);

		LoadMesh(id, mesh);

		return id;
	}

	void RendererGL::LoadMesh(unsigned int meshID, const Mesh& mesh)
	{
		MeshGLTest& meshGL = Meshes.GetObjectWithId(meshID);
		
		// Copy mesh data to opengl mesh data
		meshGL.NumberOfIndices = mesh.GetIndices().size();
		meshGL.PrimitiveType = PrimitiveTypes[(int)mesh.GetPrimitive()];

		// VAO is made "active"
		glBindVertexArray(meshGL.VAO);
		// we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
		glBindBuffer(GL_ARRAY_BUFFER, meshGL.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(MeshVertexData), &mesh.GetVertices()[0], GL_STATIC_DRAW);
		// we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGL.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(GLuint), &mesh.GetIndices()[0], GL_STATIC_DRAW);

		// we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
		// vertex positions
		// these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)0);
		// Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Normal));
		// Texture Coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, UV));
		// Tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Tangent));
		// Bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Bitangent));
		// Vertex color
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Color));

		glBindVertexArray(0);
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
		
		CompileShaderPipeline(id, compileCommand);

		return id;
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

	unsigned int RendererGL::CreateBuffer(const Buffer& buffer)
	{
		unsigned int id = Buffers.AddWithId();
		BufferGLTest& bufferGL = Buffers.GetObjectWithId(id);
		glCreateBuffers(1, &bufferGL.BufferID);
		LoadBuffer(id, buffer);

		return id;
	}

	void RendererGL::LoadBuffer(unsigned int bufferID, const Buffer& buffer)
	{
		// We assume data arrays are created correctly
		assert(buffer.GetSizes().size() == buffer.GetDatas().size());

		// init
		//glBindBufferBase(bufferType, binding, Buffers.GetObjectWithId(id));
		BufferGLTest& bufferGL = Buffers.GetObjectWithId(bufferID);

		// Copy buffer data to opengl buffer data
		bufferGL.Target = BufferTypesAssociations[(int)buffer.GetType()];
		switch (buffer.GetType())
		{
		case BufferType::Uniform:
			bufferGL.ResourceProgramInterface = GL_UNIFORM_BLOCK;
			break;

		case BufferType::ShaderStorage:
			bufferGL.ResourceProgramInterface = GL_SHADER_STORAGE_BLOCK;
			break;

		default:
			break;
		}

		glBindBuffer(bufferGL.Target, bufferGL.BufferID);

		// We need to calculate for each data its gpu occupancy (using std430 OpenGL buffer layout: https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec3.html)
		// Assuming the order of the data inside Datas array matches gpu buffer data order
		const std::vector<std::shared_ptr<void>>& datas = buffer.GetDatas();
		const std::vector<size_t>& sizes = buffer.GetSizes();
		
		size_t totalSizeBytes = 0;
		for (unsigned int i = 0; i < datas.size(); ++i)
		{	
			totalSizeBytes += sizes[i];
		}
		glBufferData(bufferGL.Target, totalSizeBytes, nullptr, BufferUsageAssociations[(int)buffer.GetAccessType()]); // reallocation of memory

		// fill
		size_t offset = 0;
		for (unsigned int i = 0; i < sizes.size(); ++i)
		{
			glBufferSubData(bufferGL.Target, offset, sizes[i], datas[i].get());
			offset += sizes[i];
		}
	}

	void RendererGL::FreeBuffer(unsigned int bufferID)
	{
		BufferGLTest& buffer = Buffers.GetObjectWithId(bufferID);
		glDeleteBuffers(1, &buffer.BufferID);

		Buffers.RemoveWithId(bufferID);
	}
}

