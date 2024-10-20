
#include "FrameBufferGL.h"

namespace GaladHen
{
	FrameBufferGL::FrameBufferGL()
		: FrameBufferID(0)
	{}

	void FrameBufferGL::Create()
	{
		glGenFramebuffers(1, &FrameBufferID);
	}

	void FrameBufferGL::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	}

	GLenum FrameBufferGL::CheckStatus()
	{
		Bind();
		GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		Unbind();

		return result;
	}

	void FrameBufferGL::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBufferGL::Free()
	{
		glDeleteFramebuffers(1, &FrameBufferID);
	}
}
