//
//  render_pass.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "render_pass.h"
#include "camera.h"
#include "render_texture.h"

namespace kge
{
	RenderPass* RenderPass::s_render_pass_binding;

	Ref<RenderPass> RenderPass::Create(Ref<RenderTexture> color_texture, Ref<RenderTexture> depth_texture, CameraClearFlags clear_flag, bool need_depth, Rect rect)
	{
		Ref<RenderPass> pass = Ref<RenderPass>(new RenderPass());
		pass->_frame_buffer.color_texture = color_texture;
		pass->_frame_buffer.depth_texture = depth_texture;
		pass->_clear_flag = clear_flag;
		pass->_need_depth = need_depth;
		pass->_rect = rect;

		pass->_Create();

		return pass;
	}

	RenderPass::RenderPass()
	{

	}

	RenderPass::~RenderPass()
	{

	}

	void RenderPass::Begin(const Color& clear_color)
	{
		Bind();

#if KGE_GLES
		RenderPassGLES::Begin(clear_color);
#endif
	}

	void RenderPass::End()
	{

#if KGE_GLES
		RenderPassGLES::End();
#endif

		UnBind();
	}

	void RenderPass::Bind()
	{
		s_render_pass_binding = this;
	}

	void RenderPass::UnBind()
	{
		s_render_pass_binding = nullptr;
	}

	bool RenderPass::HasFrameBuffer()const
	{
		return _frame_buffer.color_texture || _frame_buffer.depth_texture;
	}

	uint32 RenderPass::GetFrameBufferWidth() const
	{
		if (!HasFrameBuffer())
		{
			return Camera::Current()->GetTargetWidth();
		}
		else
		{
			if (_frame_buffer.color_texture)
				return _frame_buffer.color_texture->GetWidth();

			if (_frame_buffer.depth_texture)
				return _frame_buffer.depth_texture->GetWidth();
		}

		return -1;
	}	
	
	uint32 RenderPass::GetFrameBufferHeight() const
	{
		if (!HasFrameBuffer())
		{
			return Camera::Current()->GetTargetHeight();
		}
		else
		{
			if (_frame_buffer.color_texture)
				return _frame_buffer.color_texture->GetHeight();

			if (_frame_buffer.depth_texture)
				return _frame_buffer.depth_texture->GetHeight();
		}

		return -1;
	}
}

