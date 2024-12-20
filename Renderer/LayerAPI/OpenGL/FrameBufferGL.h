
// Class for a generic OpenGL frame buffer

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

namespace GaladHen
{
	class FrameBufferGL
	{

	public:

		FrameBufferGL();

		FrameBufferGL(const FrameBufferGL& other) = delete;
		FrameBufferGL& operator=(const FrameBufferGL& other) = delete;
		FrameBufferGL& operator=(FrameBufferGL&& other) = delete;
		FrameBufferGL(FrameBufferGL&& other) = delete;

		void Create();

		virtual void Bind();

		GLenum CheckStatus();

		virtual void Unbind();

		virtual void Free();

	protected:

		GLuint FrameBufferID;

	};
}
