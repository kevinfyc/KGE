//
//  display_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "display_GLES.h"

#include "util/log.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "graphics.h"

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

		glClearDepthf(1.0f);
		glClearStencil(0);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);

		auto vender = (char *)glGetString(GL_VENDOR);
		auto renderer = (char *)glGetString(GL_RENDERER);
		auto version = (char *)glGetString(GL_VERSION);
		_device_name = std::string(vender) + "/" + std::string(renderer) + "/" + std::string(version);
		_extensions = (char *)glGetString(GL_EXTENSIONS);

		GLint max_vertex_uniform_vectors;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &max_vertex_uniform_vectors);
		GLint max_uniform_block_size;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &max_uniform_block_size);
		GLint uniform_buffer_offset_alignment;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniform_buffer_offset_alignment);

		_uniform_buffer_offset_alignment = (int32)uniform_buffer_offset_alignment;

		GLint maxTexSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);

		KGE_LOG_INFO("GL_ES device_name: %s", _device_name.c_str());
		KGE_LOG_INFO("GL_ES extensions: %s", _extensions.c_str());
		KGE_LOG_INFO("GL_ES max_vertex_uniform_vectors:%d", max_vertex_uniform_vectors);
		KGE_LOG_INFO("GL_ES max_uniform_block_size:%d", max_uniform_block_size);
		KGE_LOG_INFO("GL_ES uniform_buffer_offset_alignment:%d", uniform_buffer_offset_alignment);
		KGE_LOG_INFO("GL_ES max texture size is %d", maxTexSize);

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

	void DisplayGLES::BindVertexBuffer(const VertexBuffer* buffer)
	{
		GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, buffer->GetBuffer()) );
	}

	void DisplayGLES::BindIndexBuffer(const IndexBuffer* buffer, IndexType index_type)
	{
		GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GetBuffer()) );
	}

	void DisplayGLES::BindVertexArray(const Ref<Shader>& shader, int pass_index)
	{
		const VertexShader* vs = shader->GetVertexShaderInfo(pass_index);
		for (const auto& attr : vs->attrs)
		{
			GL_ASSERT( glEnableVertexAttribArray(attr.location) );
			GL_ASSERT( glVertexAttribPointer(attr.location, attr.size / 4, GL_FLOAT, GL_FALSE, vs->stride, (const GLvoid*)attr.offset) );
		}
	}

	void DisplayGLES::DrawIndexed(int start, int count, IndexType index_type)
	{
		GLenum type = index_type == IndexType::U16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
		uint32 type_size = index_type == IndexType::U16 ? 2 : 4;

		GL_ASSERT( glDrawElements(GL_TRIANGLES, count, type, (const GLvoid*)(start * type_size)) );

		Graphics::draw_call++;
	}

	void DisplayGLES::DisableVertexArray(const Ref<Shader>& shader, int pass_index)
	{
		const VertexShader* vs = shader->GetVertexShaderInfo(pass_index);
		for (const auto& attr : vs->attrs)
			GL_ASSERT( glDisableVertexAttribArray(attr.location) );
	}

	void DisplayGLES::FlushContext()
	{
		GL_ASSERT( glFlush() );
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

