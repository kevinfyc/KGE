﻿//
//  display_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "display_GLES.h"

#include "util/log.h"

namespace kge
{
	bool DisplayGLES::Init(uint32 width, uint32 height)
	{
#if WIN32
		DisplayWin::Init(width, height);
#endif

		_default_depth_render_buffer = 0;

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			KGE_LOG_ERROR("Cannot initialize OpenGL");
			return false;
		}

		if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil)
		{
			KGE_LOG_ERROR("EXT_framebuffer_object and EXT_packed_depth_stencil OpenGL extensions are required");
			return false;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		GLint maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
		KGE_LOG_DEBUG("GL_ES max texture size is %d", maxTexSize);

		return true;
	}

	void DisplayGLES::Fini()
	{
		if (_default_depth_render_buffer == 0)
			glDeleteRenderbuffers(1, &_default_depth_render_buffer);

#if WIN32
		DisplayWin::Fini();
#endif
	}

	void DisplayGLES::SwapBuffers()
	{
#if WIN32
		::SwapBuffers(_hdc);
#endif
	}

	uint32 DisplayGLES::GetDefaultDepthRenderBuffer()
	{
		if (_default_depth_render_buffer == 0)
		{
			glGenRenderbuffers(1, &_default_depth_render_buffer);
			glBindRenderbuffer(GL_RENDERBUFFER, _default_depth_render_buffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		return _default_depth_render_buffer;
	}
}

