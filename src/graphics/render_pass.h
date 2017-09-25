//
//  render_pass.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_RENDER_PASS_H__
#define __KGE_GRAPHICS_RENDER_PASS_H__

#if KGE_GLES
#include "render_pass_GLES.h"
#endif

#include "frame_buffer.h"

#include "math/rect.h"

namespace kge
{
#if KGE_GLES
	class RenderPass : public RenderPassGLES
	{
		friend class RenderPassGLES;
#endif
	public:
		static RenderPass* GetRenderPassBinding() { return s_render_pass_binding; }
		static Ref<RenderPass> Create(Ref<RenderTexture> color_texture, Ref<RenderTexture> depth_texture, bool need_depth, Rect rect);

		virtual ~RenderPass();

		virtual void Begin(const Color& clear_color) override;
		virtual void End() override;

		void Bind();
		void UnBind();

		FrameBuffer GetFrameBuffer() const { return _frame_buffer; }

		uint32 GetFrameBufferWidth() const;
		uint32 GetFrameBufferHeight() const;

		Rect GetRect()const { return _rect; }

		bool HasFrameBuffer() const;

	private:
		RenderPass();

	private:
		static RenderPass* s_render_pass_binding;
		FrameBuffer _frame_buffer;
		bool _need_depth;
		Rect _rect;
	};
}

#endif // __KGE_GRAPHICS_RENDER_PASS_H__