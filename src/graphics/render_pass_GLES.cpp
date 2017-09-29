//
//  render_pass_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "render_pass_GLES.h"
#include "util/log.h"

#include "render_pass.h"
#include "render_texture.h"
#include <assert.h>
#include "display_GLES.h"
#include "graphics.h"

namespace kge
{
	RenderPassGLES::RenderPassGLES() :_frameBuffer(0)
	{

	}

	RenderPassGLES::~RenderPassGLES()
	{
		if (_frameBuffer)
			GL_ASSERT( glDeleteFramebuffers(1, &_frameBuffer) );
	}

	void RenderPassGLES::_Create()
	{
		RenderPass* pass = dynamic_cast<RenderPass*>(this);

		bool is_default = !pass->HasFrameBuffer();

		if (is_default)
		{
			_frameBuffer = 0;
		}
		else
		{
			FrameBuffer frame_buffer = pass->_frame_buffer;

			GLuint depth_texture = 0;
			GLenum attachment = 0;
			if (frame_buffer.depth_texture)
			{
				depth_texture = frame_buffer.depth_texture->GetTexture();

				DepthBuffer depth = frame_buffer.depth_texture->GetDepth();
				switch (depth)
				{
				case kge::DepthBuffer::Depth_16:
				case kge::DepthBuffer::Depth_24:
				case kge::DepthBuffer::Depth_32:
					attachment = GL_DEPTH_ATTACHMENT;
					break;
				case kge::DepthBuffer::Depth_24_Stencil_8:
					attachment = GL_DEPTH_STENCIL_ATTACHMENT;
					break;
				default:
					assert(!"invalid depth texture");
					break;
				}
			}
			else
			{
				depth_texture = (GLuint)((DisplayGLES*)Graphics::GetDisplay())->GetDefaultDepthRenderBuffer();

				attachment = GL_DEPTH_STENCIL_ATTACHMENT;
			}

			GL_ASSERT( glGenFramebuffers(1, &_frameBuffer) );
			GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );

			if (frame_buffer.color_texture)
			{
				GLuint color_texture = frame_buffer.color_texture->GetTexture();
				GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ALPHA_PAIRING_ATI, GL_TEXTURE_2D, color_texture, 0) );
			}

			if (frame_buffer.depth_texture)
				GL_ASSERT( glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, depth_texture, 0) );
			else
				GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, depth_texture) );

			auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
				KGE_LOG_ERROR("glCheckFramebufferStatus error:%d", status);

			GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		}
	}

	void RenderPassGLES::Begin(const Color& clear_color)
	{
		RenderPass* pass = dynamic_cast<RenderPass*>(this);
		CameraClearFlags clear_flag = pass->_clear_flag;

		std::vector<GLenum> attachments;
		bool has_stencil = false;

		uint32 width = pass->GetFrameBufferWidth();
		uint32 height = pass->GetFrameBufferHeight();

		glViewport(0, 0, width, height);

		if (_frameBuffer == 0)
		{
			GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, 0) );

			attachments.push_back(GL_COLOR);
			attachments.push_back(GL_DEPTH);
			attachments.push_back(GL_STENCIL);

			has_stencil = true;
		}
		else
		{
			FrameBuffer frame_buffer = pass->_frame_buffer;

			if (frame_buffer.color_texture)
				attachments.push_back(GL_COLOR_ATTACHMENT0);

			if (frame_buffer.depth_texture)
			{
				DepthBuffer depth = frame_buffer.depth_texture->GetDepth();
				switch (depth)
				{
				case kge::DepthBuffer::Depth_16:
				case kge::DepthBuffer::Depth_24:
				case kge::DepthBuffer::Depth_32:
					attachments.push_back(GL_DEPTH_ATTACHMENT);
					has_stencil = false;
					break;
				case kge::DepthBuffer::Depth_24_Stencil_8:
					attachments.push_back(GL_DEPTH_ATTACHMENT);
					attachments.push_back(GL_STENCIL_ATTACHMENT);
					has_stencil = true;
					break;
				default:
					assert(!"invalid depth texture");
					break;
				}
			}
			else
			{
				attachments.push_back(GL_DEPTH_ATTACHMENT);
				attachments.push_back(GL_STENCIL_ATTACHMENT);
				has_stencil = true;
			}

			GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer) );
		}

		switch (clear_flag)
		{
		case kge::CameraClearFlags::Invalidate:
			GL_ASSERT( glInvalidateFramebuffer(GL_FRAMEBUFFER, attachments.size(), &attachments[0]) );
			break;

		case kge::CameraClearFlags::Color:
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);

			GLbitfield clear_bit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
			if (has_stencil)
				clear_bit |= GL_STENCIL_BUFFER_BIT;

			glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
			glClear(clear_bit);
			break;
		}
		case kge::CameraClearFlags::Depth:
		{
			glDepthMask(GL_TRUE);

			GLbitfield clear_bit = GL_DEPTH_BUFFER_BIT;
			if (has_stencil)
				clear_bit |= GL_STENCIL_BUFFER_BIT;

			glClear(clear_bit);
			break;
		}
		case kge::CameraClearFlags::Nothing:
			break;
		default:
			break;
		}
	}

	void RenderPassGLES::End()
	{

	}
}

