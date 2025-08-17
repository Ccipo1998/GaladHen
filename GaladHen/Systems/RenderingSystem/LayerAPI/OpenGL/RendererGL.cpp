
#include "RendererGL.h"
#include <Systems/RenderingSystem/Common.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <glm/gtc/type_ptr.hpp> // for value_ptr() and stuff
#include <glm/gtx/quaternion.hpp>

#include <Utils/Log.h>

#include <Systems/RenderingSystem/GPUResourceInspector.h>
#include <Systems/RenderingSystem/Entities/Texture.h>
#include <Systems/RenderingSystem/Entities/Mesh.h>
#include <Systems/RenderingSystem/Entities/Buffer.hpp>
#include <Systems/RenderingSystem/Entities/RenderBuffer.h>
#include <Systems/RenderingSystem/Entities/TextureArray.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

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
		GL_R8, // (int)TextureFormat::R8
		GL_RG8, // (int)TextureFormat::RG8
		GL_RGB8, // (int)TextureFormat::RGB8
		GL_RGBA8, // (int)TextureFormat::RGBA8
		GL_SRGB8, // (int)TextureFormat::SRGB8
		GL_SRGB8_ALPHA8, // (int)TextureFormat::SRGBA8
	};

	GLenum RendererGL::TextureChannelsAssociations[61] =
	{
		GL_RED, // (int)TextureFormat::R8
		GL_RG, // (int)TextureFormat::RG8
		GL_RGB, // (int)TextureFormat::RGB8
		GL_RGBA, // (int)TextureFormat::RGBA8
		GL_RGB, // (int)TextureFormat::SRGB8
		GL_RGBA // (int)TextureFormat::SRGBA8
	};

	GLenum RendererGL::PixelDataTypeAssociations[19] =
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
		: Window(nullptr)
		, ImGuiContext(nullptr)
	{}

	void RendererGL::Init()
	{
		if (!glfwInit())
		{
			Log::Error("RendererGL", "GLFW failed to initialize the context");

			return;
		}

		// setting the minimum required version of OpenGL
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GH_GLSL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GH_GLSL_VERSION_MINOR);
		// core profile is a subset of OpenGL features (without the backward-compatible features)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// for MacOS:
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		CreateRenderingWindow(GH_DEFAULT_WINDOW_NAME, glm::uvec2(GH_DEFAULT_WINDOW_WIDTH, GH_DEFAULT_WINDOW_HEIGHT));

		if (gl3wInit() != GL3W_OK)
		{
			Log::Error("RendererGL", "Error: GL3W failed to initialize the context");

			return;
		}
	}

	void RendererGL::InitUI()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGuiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(ImGuiContext);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable docking

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		GLFWwindow* win = glfwGetCurrentContext();
		// Setup Platform/Renderer backends
		bool success = ImGui_ImplGlfw_InitForOpenGL(win, true);
		success &= ImGui_ImplOpenGL3_Init(GH_GLSL_VERSION);

		if (!success)
		{
			Log::Error("RendererGL", "ImGui failed to setup backends for OpenGL and GLFW");
		}
	}

	unsigned int RendererGL::CreateRenderBuffer(unsigned int width, unsigned int height, TextureFormat format, RenderBufferType renderBufferType, bool clampDepthToBorder)
	{
		unsigned int id = RenderBuffers.AddWithId();
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(id);

		GLCall([&rb] { glGenFramebuffers(1, &rb.FrameBufferID); });
		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });

		Texture texture{ nullptr, width, height, 0, format };

		// Create and attach depth buffer, if requested
		if (renderBufferType == RenderBufferType::ColorOnly || renderBufferType == RenderBufferType::ColorAndDepth)
		{
			rb.ColorTextureID = CreateTexture(texture, TextureAllocationType::Dynamic); // dynamic allocation for render buffers
			TextureGL& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);
			GLCall([&colorTexture] { glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.TextureID, 0); });
		}
		if (renderBufferType == RenderBufferType::DepthOnly || renderBufferType == RenderBufferType::ColorAndDepth)
		{
			rb.DepthTextureID = CreateDepthTexture(width, height, clampDepthToBorder);
			TextureGL& depthTexture = Textures.GetObjectWithId(rb.DepthTextureID);
			GLCall([&depthTexture] { glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.TextureID, 0); });
		}

		// Check status
		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			Log::Error("RendererGL", "Framebuffer is not complete!\n");
		}
		GLCall([] { glBindFramebuffer(GL_FRAMEBUFFER, 0); });

		return id;
	}

	unsigned int RendererGL::CreateRenderBufferArray(unsigned int width, unsigned int height, unsigned int depth, TextureFormat format, RenderBufferType renderBufferType, bool clampDepthToBorder /*= false*/)
	{
		unsigned int id = RenderBuffers.AddWithId();
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(id);

		GLCall([&rb] { glGenFramebuffers(1, &rb.FrameBufferID); });
		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });

		TextureArray textureArray{ nullptr, width, height, depth, 0, format };

		// NOTE: for now we are not considering color and depth texture array, because it would require two separate frame buffer for opengl

		// Create and attach depth buffer, if requested
		if (renderBufferType == RenderBufferType::ColorOnly)
		{
			rb.ColorTextureID = CreateTextureArray(textureArray, TextureAllocationType::Dynamic); // dynamic allocation for render buffers
			// glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture.TextureID, 0, i); <- needed for binding, not for texture array creation
		}
		if (renderBufferType == RenderBufferType::DepthOnly)
		{
			rb.DepthTextureID  = CreateDepthTextureArray(textureArray, clampDepthToBorder);

			for (unsigned int i = 0; i < depth; ++i)
			{
				GLCall([&rb, i] { glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, rb.DepthTextureID, 0, i); });
				/*glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);*/
			}
		}

		// Check status
		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			Log::Error("RendererGL", "Framebuffer is not complete!\n");
		}

		GLCall([] { glBindFramebuffer(GL_FRAMEBUFFER, 0); });

		return id;
	}

	void RendererGL::ClearRenderBuffer(unsigned int renderBufferID, glm::vec4 clearColor)
	{
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(renderBufferID);
		if (rb.ColorTextureID != 0)
		{
			TextureGL& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);

			GLCall([&colorTexture] { glBindTexture(colorTexture.Target, colorTexture.TextureID); });
			GLCall([clearColor] { glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w); });
			GLCall([&colorTexture] { glBindTexture(colorTexture.Target, 0); });

			GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });
			GLCall([]
				{
					GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
					glClear(mask);
				});
			GLCall([] { glBindFramebuffer(GL_FRAMEBUFFER, 0); });
		}
		if (rb.DepthTextureID != 0)
		{
			TextureGL& depthTexture = Textures.GetObjectWithId(rb.DepthTextureID);
			
			GLCall([&depthTexture] { glBindTexture(depthTexture.Target, depthTexture.TextureID); });
			GLCall([clearColor] { glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w); });
			GLCall([&depthTexture] { glBindTexture(depthTexture.Target, 0); });

			GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });
			GLCall([]
				{
					GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
					glClear(mask);
				});
			GLCall([] { glBindFramebuffer(GL_FRAMEBUFFER, 0); });
		}
	}

	void RendererGL::ClearRenderBufferArrayLayer(unsigned int renderBufferID, glm::vec4 clearColor, unsigned int width, unsigned int height, unsigned int layer)
	{
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(renderBufferID);
		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });

		GLuint clearColorGL[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
		if (rb.ColorTextureID != 0)
		{
			TextureGL& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);
			GLCall([&colorTexture, layer] { glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture.TextureID, 0, layer);  });
			GLCall([]
				{
					GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
					glClear(mask);
				});
		}
		if (rb.DepthTextureID != 0)
		{
			TextureGL& depthTexture = Textures.GetObjectWithId(rb.DepthTextureID);
			GLCall([&depthTexture, layer] { glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture.TextureID, 0, layer);  });
			GLCall([]
				{
					GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
					glClear(mask);
				});
		}

		GLCall([] { glBindFramebuffer(GL_FRAMEBUFFER, 0); });
	}

	void RendererGL::BindRenderBuffer(unsigned int renderBufferID)
	{
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(renderBufferID);
		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });
	}

	void RendererGL::BindRenderBufferArrayLayer(unsigned int renderBufferID, unsigned int layer)
	{
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(renderBufferID);

		GLCall([&rb] { glBindFramebuffer(GL_FRAMEBUFFER, rb.FrameBufferID); });

		// NOTE: current assumption that we are not considering render target array with both color and depth attachments
		if (rb.ColorTextureID != 0)
		{
			TextureGL& colorTexture = Textures.GetObjectWithId(rb.ColorTextureID);
			GLCall([&colorTexture, layer] { glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture.TextureID, 0, layer); });
		}
		else if (rb.DepthTextureID != 0)
		{
			TextureGL& depthTexture = Textures.GetObjectWithId(rb.DepthTextureID);
			GLCall([&depthTexture, layer] { glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture.TextureID, 0, layer); });

			/*GLCall([] { glDrawBuffer(GL_NONE); });
			GLCall([] { glReadBuffer(GL_NONE); });*/
		}
	}

	void RendererGL::UnbindActiveRenderBuffer()
	{
		GLCall([] { glBindFramebuffer(GL_FRAMEBUFFER, 0); });
	}

	void RendererGL::FreeRenderBuffer(unsigned int renderBufferID)
	{
		RenderBufferGL& rb = RenderBuffers.GetObjectWithId(renderBufferID);

		if (rb.ColorTextureID != 0)
		{
			FreeTexture(rb.ColorTextureID);
		}
		if (rb.DepthTextureID != 0)
		{
			FreeTexture(rb.DepthTextureID);
		}

		GLCall([&rb] { glDeleteFramebuffers(1, &rb.FrameBufferID); });

		RenderBuffers.RemoveWithId(rb.FrameBufferID);
	}

	void RendererGL::Draw(CommandBuffer<RenderCommand>& renderCommandBuffer)
	{
		// Material's parameters shouldn't be loaded here but in a MemoryTransferCommand send by higher level renderer. While we are not using bindless textures, however, we should do it here

		for (RenderCommand& rc : renderCommandBuffer)
		{
			MeshGL& mesh = Meshes.GetObjectWithId(rc.DataSourceID);

			if (!rc.ShaderSourceID)
				continue;

			GLuint program = Shaders.GetObjectWithId(rc.ShaderSourceID);
			GLCall([&program] { glUseProgram(program); });

			if (!rc.Material)
				continue;

			for (auto& scalar : rc.Material->ScalarData)
			{
				GLint location;
				GLCall([&program, &scalar, &location] { location = glGetUniformLocation(program, scalar.first.data()); });
				GLCall([&program, &scalar, &location] { glProgramUniform1f(program, location, scalar.second); });
			}

			for (auto& vec2 : rc.Material->Vec2Data)
			{
				GLint location;
				GLCall([&program, &vec2, &location] { location = glGetUniformLocation(program, vec2.first.data()); });
				GLCall([&program, &vec2, &location] { glProgramUniform2f(program, location, vec2.second.x, vec2.second.y); });
			}

			for (auto& vec3 : rc.Material->Vec3Data)
			{
				GLint location;
				GLCall([&program, &vec3, &location] { location = glGetUniformLocation(program, vec3.first.data()); });
				GLCall([&program, &vec3, &location] { glProgramUniform3f(program, location, vec3.second.x, vec3.second.y, vec3.second.z); });
			}

			for (auto& vec4 : rc.Material->Vec4Data)
			{
				GLint location;
				GLCall([&program, &vec4, &location] { location = glGetUniformLocation(program, vec4.first.data()); });
				GLCall([&program, &vec4, &location] { glProgramUniform4f(program, location, vec4.second.x, vec4.second.y, vec4.second.z, vec4.second.w); });
			}

			for (auto& mat3 : rc.Material->Mat4Data)
			{
				GLint location;
				GLCall([&program, &mat3, &location] { location = glGetUniformLocation(program, mat3.first.data()); });
				GLCall([&program, &mat3, &location] { glProgramUniformMatrix3fv(program, location, 1, GL_FALSE, (GLfloat*)&mat3.second); });
			}

			for (auto& mat4 : rc.Material->Mat4Data)
			{
				GLint location;
				GLCall([&program, &mat4, &location] { location = glGetUniformLocation(program, mat4.first.data()); });
				GLCall([&program, &mat4, &location] { glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, (GLfloat*)&mat4.second); });
			}

			//for (ShaderIntegerData& data : rc.ShaderData.IntegerData)
			//{
			//	glProgramUniform1i(program, glGetUniformLocation(program, data.DataName.data()), data.Integer);
			//}

			for (auto& mat4Data : rc.AdditionalMat4Data)
			{
				GLint location;
				GLCall([&program, &mat4Data, &location] { location = glGetUniformLocation(program, mat4Data.first.data()); });
				GLCall([&program, &mat4Data, &location] { glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, (GLfloat*)&mat4Data.second); });
			}

			unsigned int unit = 0;
			for (auto& texture : rc.Material->TextureData)
			{
				if (texture.second.expired())
				{
					// texture not valid
					continue;
				}

				// select texture unit for the binded texture
				std::shared_ptr<Texture> shTexture = texture.second.lock();
				unsigned int textureID = GPUResourceInspector::GetResourceID(shTexture.get());
				// we need to check id here, because texture is an external resource that can arrive as invalid here
				if (!textureID)
					continue;

				TextureGL& textureGL = Textures.GetObjectWithId(textureID);
				GLCall([unit] { glActiveTexture(TextureUnits[unit]); });
				GLCall([&textureGL] { glBindTexture(textureGL.Target, textureGL.TextureID); });

				// create uniform sampler
				GLint location;
				GLCall([&program, &texture, unit, &location] { location = glGetUniformLocation(program, texture.first.data()); });
				GLCall([&location, unit] { glUniform1i(location, unit); });

				++unit;
			}
			for (auto& renderBufferData : rc.AdditionalRenderBufferData)
			{
				if (renderBufferData.second == nullptr)
				{
					// render buffer not valid
					continue;
				}

				unsigned int renderBufferID = GPUResourceInspector::GetResourceID(renderBufferData.second);
				RenderBufferGL& renderBufferGL = RenderBuffers.GetObjectWithId(renderBufferID);
				if (renderBufferGL.ColorTextureID != 0)
				{
					TextureGL& textureGL = Textures.GetObjectWithId(renderBufferGL.ColorTextureID);
					GLCall([unit] { glActiveTexture(TextureUnits[unit]); });
					GLCall([&textureGL] { glBindTexture(textureGL.Target, textureGL.TextureID); });

					// create uniform sampler
					GLint location;
					GLCall([&program, &renderBufferData, unit, &location] { location = glGetUniformLocation(program, renderBufferData.first.data()); });
					GLCall([&location, unit] { glUniform1i(location, unit); });

					++unit;
				}
				if (renderBufferGL.DepthTextureID != 0)
				{
					TextureGL& textureGL = Textures.GetObjectWithId(renderBufferGL.DepthTextureID);
					GLCall([unit] { glActiveTexture(TextureUnits[unit]); });
					GLCall([&textureGL] { glBindTexture(textureGL.Target, textureGL.TextureID); });

					// create uniform sampler
					GLint location;
					GLCall([&program, &renderBufferData, unit, &location] { location = glGetUniformLocation(program, renderBufferData.first.data()); });
					GLCall([&location, unit] { glUniform1i(location, unit); });

					++unit;
				}
			}

			// Bind buffer data to shader pipeline
			const GLenum props[] = { GL_BUFFER_BINDING };
			for (auto& buffer : rc.Material->BufferData)
			{
				if (buffer.second.expired())
				{
					// buffer not valid
					continue;
				}

				std::shared_ptr<IBuffer> shBuffer = buffer.second.lock();
				unsigned int bufferID = GPUResourceInspector::GetResourceID(shBuffer.get());
				// we need to check id here, because buffer is an external resource that can arrive as invalid here
				if (!bufferID)
					continue;

				BufferGL& bufferGL = Buffers.GetObjectWithId(bufferID);

				GLuint resourceIndex;
				GLCall([this, &program, &bufferGL, &buffer, &props, &resourceIndex] { resourceIndex = glGetProgramResourceIndex(program, bufferGL.ResourceProgramInterface, buffer.first.data()); });
				if (resourceIndex != GL_INVALID_INDEX)
				{
					GLint bufferIndex;
					GLCall([&program, &bufferGL, resourceIndex, &props, &bufferIndex] { glGetProgramResourceiv(program, bufferGL.ResourceProgramInterface, resourceIndex, 1, props, 1, NULL, &bufferIndex); });
					GLCall([&bufferGL, &bufferIndex] { glBindBufferBase(bufferGL.Target, bufferIndex, bufferGL.BufferID); });
				}
			}
			for (auto& buffer : rc.AdditionalBufferData)
			{
				unsigned int bufferID = GPUResourceInspector::GetResourceID(buffer.second);
				// we need to check id here, because buffer is an external resource that can arrive as invalid here
				if (!bufferID)
					continue;

				BufferGL& bufferGL = Buffers.GetObjectWithId(bufferID);

				GLuint resourceIndex;
				GLCall([this, &program, &bufferGL, &buffer, &props, &resourceIndex] { resourceIndex = glGetProgramResourceIndex(program, bufferGL.ResourceProgramInterface, buffer.first.data()); });
				if (resourceIndex != GL_INVALID_INDEX)
				{
					GLint bufferIndex;
					GLCall([&program, &bufferGL, &resourceIndex, &props, &bufferIndex] { glGetProgramResourceiv(program, bufferGL.ResourceProgramInterface, resourceIndex, 1, props, 1, NULL, &bufferIndex); });
					GLCall([&bufferGL, &bufferIndex] { glBindBufferBase(bufferGL.Target, bufferIndex, bufferGL.BufferID); });
				}
			}

			// draw
			GLCall([&mesh] { glBindVertexArray(mesh.VAO); });
			GLCall([&mesh] { glDrawElements(mesh.PrimitiveType, mesh.NumberOfIndices, GL_UNSIGNED_INT, 0); });
			GLCall([] { glBindVertexArray(0); });
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
				IBuffer* buffer = static_cast<IBuffer*>(mtc.Data);
				unsigned int& bufferID = mtc.MemoryTargetID; // update memory target id

				switch (mtc.TransferType)
				{
				case MemoryTransferType::Load:

					if (bufferID == 0)
					{
						// New OpenGL resource
						bufferID = CreateBuffer(buffer);
					}
					else
					{
						LoadBuffer(bufferID, buffer);
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

				if (mtc.TransferType == MemoryTransferType::Load)
				{
					if (textureID == 0)
					{
						// New opengl resource
						textureID = CreateTexture(*texture);
					}
					else
					{
						LoadTexture(textureID, *texture);
					}
				}
				/*else if (mtc.TransferType == MemoryTransferType::Free)
				{
					FreeTexture(textureID);
				}*/

				break;
			}
			case MemoryTargetType::TextureArray:
			{
				TextureArray* textureArray = static_cast<TextureArray*>(mtc.Data);
				unsigned int& textureID = mtc.MemoryTargetID; // update memory target id

				if (textureID == 0)
				{
					// New opengl resource
					textureID = CreateTextureArray(*textureArray);
				}
				else
				{
					LoadTextureArray(textureID, *textureArray);
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
			GLCall([] { glEnable(GL_DEPTH_TEST); });
		else
			GLCall([] { glDisable(GL_DEPTH_TEST); });
	}

	void RendererGL::EnableBackFaceCulling(bool enable)
	{
		GLCall([] { glEnable(GL_CULL_FACE); });
		GLCall([] { glCullFace(GL_BACK); });
		GLCall([] { glFrontFace(GL_CCW); });
	}

	unsigned int RendererGL::GetRenderBufferColorApiID(unsigned int renderBufferID)
	{
		RenderBufferGL& renderBuffer = RenderBuffers.GetObjectWithId(renderBufferID);
		TextureGL& colorTexture = Textures.GetObjectWithId(renderBuffer.ColorTextureID);
		return colorTexture.TextureID;
	}

	void RendererGL::SetViewport(const glm::uvec2& position, const glm::uvec2& size)
	{
		GLCall([&position, &size] { glViewport(position.x, position.y, size.x, size.y); });
	}

	void RendererGL::CreateRenderingWindow(const char* name, glm::uvec2 size)
	{
		if (Window)
			return;

		Window = glfwCreateWindow(size.x, size.y, name, nullptr, nullptr);
		if (Window == nullptr)
		{
			Log::Error("RendererGL", "GLFW failed to create the window");

			return;
		}

		glfwMakeContextCurrent(Window);
	}

	void RendererGL::SwapWindowBuffers()
	{
		glfwSwapBuffers(Window);
	}

	void RendererGL::CloseRenderingWindow()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();

		Window = nullptr;
	}

	void RendererGL::BeforeDrawUI()
	{
		ImGui::SetCurrentContext(ImGuiContext);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RendererGL::DrawUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	RendererGL::~RendererGL()
	{
		// TODO

		QuitUI();
	}

	unsigned int RendererGL::CreateTexture(const Texture& texture, TextureAllocationType allocationType)
	{
		unsigned int id = Textures.AddWithId();
		TextureGL& textureGL = Textures.GetObjectWithId(id);

		// create new texture object
		GLCall([&textureGL] { glGenTextures(1, &textureGL.TextureID); });

		LoadTexture(id, texture, allocationType);

		return id;
	}

	unsigned int RendererGL::CreateTextureArray(const TextureArray& textureArray, TextureAllocationType allocationType /*= TextureAllocationType::Constant*/)
	{
		unsigned int id = Textures.AddWithId();
		TextureGL& textureGL = Textures.GetObjectWithId(id);

		// create new texture object
		GLCall([&textureGL] { glGenTextures(1, &textureGL.TextureID); });

		LoadTextureArray(id, textureArray, allocationType);

		return id;
	}

	unsigned int RendererGL::CreateDepthTexture(unsigned int width, unsigned int height, bool clampToBorder)
	{
		unsigned int id = Textures.AddWithId();
		TextureGL& textureGL = Textures.GetObjectWithId(id);
		textureGL.Target = GL_TEXTURE_2D;
		textureGL.PixelDataType = PixelDataTypeAssociations[0];

		GLCall([&textureGL] { glGenTextures(1, &textureGL.TextureID); });
		GLCall([&textureGL] { glBindTexture(textureGL.Target, textureGL.TextureID); });
		GLCall([&textureGL, width, height] { glTexImage2D(textureGL.Target, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL); });

		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR); });

		if (clampToBorder)
		{
			// Clamp the border and manually set maximum depth (1.0) on the border itself
			// This way we can avoid some shadow specific problems
			GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); });
			GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); });
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			GLCall([&textureGL, &borderColor] { glTexParameterfv(textureGL.Target, GL_TEXTURE_BORDER_COLOR, borderColor); });
		}

		return id;
	}

	unsigned int RendererGL::CreateDepthTextureArray(const TextureArray& textureArray, bool clampToBorder /*= false*/)
	{
		unsigned int id = Textures.AddWithId();
		TextureGL& texture = Textures.GetObjectWithId(id);
		texture.Target = GL_TEXTURE_2D_ARRAY;

		GLCall([&texture] { glGenTextures(1, &texture.TextureID); });
		GLCall([&texture] { glBindTexture(texture.Target, texture.TextureID); });

		glm::uvec2 size;
		textureArray.GetSize(size);
		GLCall([&texture, &size, &textureArray] { glTexImage3D(texture.Target, 0, GL_DEPTH_COMPONENT, size.x, size.y, textureArray.GetNumberOfLayers(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL); });

		GLCall([&texture] { glTexParameteri(texture.Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR); });
		GLCall([&texture] { glTexParameteri(texture.Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR); });

		if (clampToBorder)
		{
			// Clamp the border and manually set maximum depth (1.0) on the border itself
			// This way we can avoid some shadow specific problems
			GLCall([&texture] { glTexParameteri(texture.Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); });
			GLCall([&texture] { glTexParameteri(texture.Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); });
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			GLCall([&texture, &borderColor] { glTexParameterfv(texture.Target, GL_TEXTURE_BORDER_COLOR, borderColor); });
		}

		return id;
	}

	void RendererGL::FreeTexture(unsigned int textureID)
	{
		TextureGL& texture = Textures.GetObjectWithId(textureID);

		GLCall([&texture] { glDeleteTextures(1, &texture.TextureID); });

		Textures.RemoveWithId(textureID);
	}

	void RendererGL::LoadTexture(unsigned int textureID, const Texture& texture, TextureAllocationType allocationType)
	{
		TextureGL& textureGL = Textures.GetObjectWithId(textureID);
		// Copy texture data in opengl texture data
		textureGL.Target = GL_TEXTURE_2D;
		textureGL.AllocationType = allocationType;
		textureGL.Filtering = FilteringAssociations[(int)texture.GetFiltering()];
		textureGL.Wrapping = WrappingAssociations[(int)texture.GetWrapping()];
		textureGL.TextureFormat = TextureFormatAssociations[(int)texture.GetFormat()];
		textureGL.TextureChannels = TextureChannelsAssociations[(int)texture.GetFormat()];
		textureGL.PixelDataType = PixelDataTypeAssociations[0];

		// bind new texture object to texture target
		GLCall([&textureGL] { glBindTexture(textureGL.Target, textureGL.TextureID); });

		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_WRAP_S, textureGL.Wrapping); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_WRAP_T, textureGL.Wrapping); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_MIN_FILTER, textureGL.Filtering); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_MAG_FILTER, textureGL.Filtering); });

		switch (allocationType)
		{
		case TextureAllocationType::Dynamic:
		{
			textureGL.Levels = texture.GetNumberOfMipMaps();

			glm::uvec2 textureSize;
			texture.GetSize(textureSize);
			unsigned char* data = texture.GetData();
			GLCall([&textureGL, &textureSize, &data] { glTexImage2D(textureGL.Target, textureGL.Levels, textureGL.TextureFormat, textureSize.x, textureSize.y, 0, textureGL.TextureChannels, textureGL.PixelDataType, data); });

			break;
		}
		case TextureAllocationType::Constant:
		{
			textureGL.Levels = texture.GetNumberOfMipMaps() + 1;

			// allocate immutable storage basing on number of channels and on bit depth
			// IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
			// levels are the number of mipmaps
			glm::uvec2 textureSize;
			texture.GetSize(textureSize);
			GLCall([&textureGL, &textureSize] { glTexStorage2D(textureGL.Target, textureGL.Levels, textureGL.TextureFormat, textureSize.x, textureSize.y); });

			// copy texture data to texture object
			unsigned char* data = texture.GetData();
			GLCall([&textureGL, &textureSize, &data] { glTexSubImage2D(textureGL.Target, 0, 0, 0, textureSize.x, textureSize.y, textureGL.TextureChannels, textureGL.PixelDataType, data); });
		}
		default:
			break;
		}

		if (textureGL.Levels > 1)
			GLCall([&textureGL] { glGenerateTextureMipmap(textureGL.TextureID); });
	}

	void RendererGL::LoadTextureArray(unsigned int textureID, const TextureArray& textureArray, TextureAllocationType allocationType /*= TextureAllocationType::Constant*/)
	{
		TextureGL& textureGL = Textures.GetObjectWithId(textureID);

		// Copy texture data in opengl texture data
		textureGL.Target = GL_TEXTURE_2D_ARRAY;
		textureGL.AllocationType = allocationType;
		textureGL.Filtering = FilteringAssociations[(int)textureArray.GetFiltering()];
		textureGL.Wrapping = WrappingAssociations[(int)textureArray.GetWrapping()];
		textureGL.TextureFormat = TextureFormatAssociations[(int)textureArray.GetFormat()];
		textureGL.TextureChannels = TextureChannelsAssociations[(int)textureArray.GetFormat()];
		textureGL.PixelDataType = PixelDataTypeAssociations[0];

		// bind new texture object to texture target
		GLCall([&textureGL] { glBindTexture(textureGL.Target, textureGL.TextureID); });

		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_WRAP_S, textureGL.Wrapping); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_WRAP_T, textureGL.Wrapping); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_MIN_FILTER, textureGL.Filtering); });
		GLCall([&textureGL] { glTexParameteri(textureGL.Target, GL_TEXTURE_MAG_FILTER, textureGL.Filtering); });

		switch (allocationType)
		{
		case TextureAllocationType::Dynamic:
		{
			textureGL.Levels = textureArray.GetNumberOfMipMaps();

			glm::uvec2 textureSize;
			textureArray.GetSize(textureSize);
			unsigned char* data = textureArray.GetData();
			GLCall([&textureGL, &textureSize, &data, &textureArray] { glTexImage3D(textureGL.Target, textureGL.Levels, textureGL.TextureFormat, textureSize.x, textureSize.y, textureArray.GetNumberOfLayers(), 0, textureGL.TextureChannels, textureGL.PixelDataType, data); });

			break;
		}
		case TextureAllocationType::Constant:
		{
			textureGL.Levels = textureArray.GetNumberOfMipMaps() + 1;

			// allocate immutable storage basing on number of channels and on bit depth
			// IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
			// levels are the number of mipmaps
			glm::uvec2 textureSize;
			textureArray.GetSize(textureSize);
			GLCall([&textureGL, &textureSize, &textureArray] { glTexStorage3D(textureGL.Target, textureGL.Levels, textureGL.TextureFormat, textureSize.x, textureSize.y, textureArray.GetNumberOfLayers()); });

			// copy texture data to texture object
			unsigned char* data = textureArray.GetData();
			GLCall([&textureGL, &textureSize, &textureArray, &data] { glTexSubImage3D(textureGL.Target, 0, 0, 0, 0, textureSize.x, textureSize.y, textureArray.GetNumberOfLayers(), textureGL.TextureChannels, textureGL.PixelDataType, data); });
		}
		default:
			break;
		}

		if (textureGL.Levels > 1)
			GLCall([&textureGL] { glGenerateTextureMipmap(textureGL.TextureID); });
	}

	unsigned int RendererGL::CreateMesh(const Mesh& mesh)
	{
		unsigned int id = Meshes.AddWithId();
		MeshGL& meshGL = Meshes.GetObjectWithId(id);

		// we create the buffers
		GLCall([&meshGL] { glGenVertexArrays(1, &meshGL.VAO); });
		GLCall([&meshGL] { glGenBuffers(1, &meshGL.VBO); });
		GLCall([&meshGL] { glGenBuffers(1, &meshGL.EBO); });

		LoadMesh(id, mesh);

		return id;
	}

	void RendererGL::LoadMesh(unsigned int meshID, const Mesh& mesh)
	{
		MeshGL& meshGL = Meshes.GetObjectWithId(meshID);
		
		// Copy mesh data to opengl mesh data
		meshGL.NumberOfIndices = mesh.GetIndices().size();
		meshGL.PrimitiveType = PrimitiveTypes[(int)mesh.GetPrimitive()];

		// VAO is made "active"
		GLCall([&meshGL] { glBindVertexArray(meshGL.VAO); });
		// we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
		GLCall([&meshGL] { glBindBuffer(GL_ARRAY_BUFFER, meshGL.VBO); });
		GLCall([&mesh] { glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(MeshVertexData), &mesh.GetVertices()[0], GL_STATIC_DRAW); });
		// we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
		GLCall([&meshGL] { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGL.EBO); });
		GLCall([&mesh] { glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(GLuint), &mesh.GetIndices()[0], GL_STATIC_DRAW); });

		// we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
		// vertex positions
		// these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
		GLCall([] { glEnableVertexAttribArray(0); });
		GLCall([] { glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)0); });
		// Normals
		GLCall([] { glEnableVertexAttribArray(1); });
		GLCall([] { glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Normal)); });
		// Texture Coordinates
		GLCall([] { glEnableVertexAttribArray(2); });
		GLCall([] { glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, UV)); });
		// Tangent
		GLCall([] { glEnableVertexAttribArray(3); });
		GLCall([] { glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Tangent)); });
		// Bitangent
		GLCall([] { glEnableVertexAttribArray(4); });
		GLCall([] { glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Bitangent)); });
		// Vertex color
		GLCall([] { glEnableVertexAttribArray(5); });
		GLCall([] { glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertexData), (GLvoid*)offsetof(MeshVertexData, Color)); });

		GLCall([] { glBindVertexArray(0); });
	}

	void RendererGL::FreeMesh(unsigned int meshID)
	{
		MeshGL& mesh = Meshes.GetObjectWithId(meshID);

		GLCall([&mesh] { glDeleteVertexArrays(1, &mesh.VAO); });
		GLCall([&mesh] { glDeleteBuffers(1, &mesh.VBO); });
		GLCall([&mesh] { glDeleteBuffers(1, &mesh.EBO); });

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
		GLCall([&program] { program = glCreateProgram(); });

		// compile the shaders
		GLuint vShader, tcShader, teShader, gShader, fShader;

		// Vertex Shader
		if (compileCommand.VertexCode.length() > 0)
		{
			GLCall([&vShader] { vShader = glCreateShader(GL_VERTEX_SHADER); });
			GLCall([&vShader, &vCode] { glShaderSource(vShader, 1, &vCode, NULL); });
			GLCall([&vShader] { glCompileShader(vShader); });

			// check compilation errors
			if (!CheckShaderPipelineCompilation(vShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in vertex shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			GLCall([&program, &vShader] { glAttachShader(program, vShader); });
		}

		// Tesselation control Shader
		if (compileCommand.TessContCode.length() > 0)
		{
			GLCall([&tcShader] { tcShader = glCreateShader(GL_TESS_CONTROL_SHADER); });
			GLCall([&tcShader, tcCode] { glShaderSource(tcShader, 1, &tcCode, NULL); });
			GLCall([&tcShader] { glCompileShader(tcShader); });
			
			// check compilation errors
			if (!CheckShaderPipelineCompilation(tcShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in tesselation control shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			GLCall([&program, &tcShader] { glAttachShader(program, tcShader); });
		}

		// Tesselation evaluation Shader
		if (compileCommand.TessEvalCode.length() > 0)
		{
			GLCall([&teShader] { teShader = glCreateShader(GL_TESS_EVALUATION_SHADER); });
			GLCall([&teShader, &teCode] { glShaderSource(teShader, 1, &teCode, NULL); });
			GLCall([&teShader] { glCompileShader(teShader); });

			// check compilation errors
			if (!CheckShaderPipelineCompilation(teShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in tesselation evaluation shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			GLCall([&program, &teShader] { glAttachShader(program, teShader); });
		}

		// Geometry Shader
		if (compileCommand.GeometryCode.length() > 0)
		{
			GLCall([&gShader] { gShader = glCreateShader(GL_GEOMETRY_SHADER); });
			GLCall([&gShader, &gCode] { glShaderSource(gShader, 1, &gCode, NULL); });
			GLCall([&gShader] { glCompileShader(gShader); });
			
			// check compilation errors
			if (!CheckShaderPipelineCompilation(gShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in geometry shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			GLCall([&program, gShader] { glAttachShader(program, gShader); });
		}

		// Fragment Shader
		if (compileCommand.FragmentCode.length() > 0)
		{
			GLCall([&fShader] { fShader = glCreateShader(GL_FRAGMENT_SHADER); });
			GLCall([&fShader, &fCode] { glShaderSource(fShader, 1, &fCode, NULL); });
			GLCall([&fShader] { glCompileShader(fShader); });
			
			// check compilation errors
			if (!CheckShaderPipelineCompilation(fShader, log, 1000))
			{
				std::string error = "[ERROR] Compilation error in fragment shader: ";
				error.append(log);
				error.append("\n");

				compileCommand.Result.Succeed = false;
				compileCommand.Result.Description.append(error);
			}

			GLCall([&program, &fShader] { glAttachShader(program, fShader); });
		}

		// link
		GLCall([&program] { glLinkProgram(program); });

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
			GLCall([&vShader] { glDeleteShader(vShader); });
		}
		if (compileCommand.TessContCode.length() > 0)
		{
			GLCall([&tcShader] { glDeleteShader(tcShader); });
		}
		if (compileCommand.TessEvalCode.length() > 0)
		{
			GLCall([&teShader] { glDeleteShader(teShader); });
		}
		if (compileCommand.GeometryCode.length() > 0)
		{
			GLCall([&gShader] { glDeleteShader(gShader); });
		}
		if (compileCommand.FragmentCode.length() > 0)
		{
			GLCall([&fShader] { glDeleteShader(fShader); });
		}

		return compileCommand.Result.Succeed;
	}

	bool RendererGL::CheckShaderPipelineCompilation(GLuint shaderProgram, char* outLog, unsigned int outLogLength)
	{
		GLint success;
		GLCall([&success, &shaderProgram] { glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success); });

		if (!success)
		{
			GLCall([&shaderProgram, outLogLength, &outLog] { glGetShaderInfoLog(shaderProgram, outLogLength, NULL, outLog); });
		}

		return success;
	}

	bool RendererGL::CheckShaderPipelineLinking(GLuint shaderProgram, char* outLog, unsigned int outLogLength)
	{
		GLint success;
		GLCall([&success, &shaderProgram] { glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); });

		if (!success)
		{
			GLCall([&shaderProgram, outLogLength, &outLog] { glGetProgramInfoLog(shaderProgram, outLogLength, NULL, outLog); });
		}

		return success;
	}

	void RendererGL::FreeShaderPipeline(unsigned int shaderID)
	{
		GLuint id = Shaders.GetObjectWithId(shaderID);
		GLCall([id] { glDeleteProgram(id); });

		Shaders.RemoveWithId(shaderID);
	}

	void RendererGL::QuitUI()
	{
		ImGui::SetCurrentContext(ImGuiContext);

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	unsigned int RendererGL::CreateBuffer(const IBuffer* buffer)
	{
		unsigned int id = Buffers.AddWithId();
		BufferGL& bufferGL = Buffers.GetObjectWithId(id);
		bufferGL.BytesSize = 0;
		GLCall([&bufferGL] { glCreateBuffers(1, &bufferGL.BufferID); });
		LoadBuffer(id, buffer);

		return id;
	}

	void RendererGL::LoadBuffer(unsigned int bufferID, const IBuffer* buffer)
	{
		// init
		//glBindBufferBase(bufferType, binding, Buffers.GetObjectWithId(id));
		BufferGL& bufferGL = Buffers.GetObjectWithId(bufferID);

		// Copy buffer data to opengl buffer data
		bufferGL.Target = BufferTypesAssociations[(int)buffer->GetType()];
		switch (buffer->GetType())
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

		GLCall([&bufferGL] { glBindBuffer(bufferGL.Target, bufferGL.BufferID); });

		// We need to calculate for each data its gpu occupancy (using std430 OpenGL buffer layout: https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec3.html)
		// Assuming the order of the data inside Datas array matches gpu buffer data order
		const void* data = buffer->GetData();
		const size_t size = buffer->GetBytesSize();

		if (bufferGL.BytesSize != size)
		{
			// Size is changed, we need to reallocate buffer
			GLCall([&bufferGL, size, &data, &buffer] { glBufferData(bufferGL.Target, size, data, BufferUsageAssociations[(int)buffer->GetAccessType()]); }); // reallocation of memory});
		}
		else
		{
			GLCall([&bufferGL, size, &data] { glBufferSubData(bufferGL.Target, 0, size, data); }); // writing only
		}

		bufferGL.BytesSize = size;
	}

	void RendererGL::FreeBuffer(unsigned int bufferID)
	{
		BufferGL& buffer = Buffers.GetObjectWithId(bufferID);
		GLCall([&buffer] { glDeleteBuffers(1, &buffer.BufferID); });

		Buffers.RemoveWithId(bufferID);
	}
}

