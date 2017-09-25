//
//  render_pass_GLES.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_RENDER_PASS_GLES_H__
#define __KGE_GRAPHICS_RENDER_PASS_GLES_H__

#include "common.h"
#include "color.h"

namespace kge
{
#if KGE_GLES
	class RenderPassGLES
#endif
	{
	public:
		virtual ~RenderPassGLES();
		virtual void Begin(const Color& clear_color);
		virtual void End();

	protected:
		RenderPassGLES();
		void _Create();

	private:
		GLuint _frameBuffer;
	};
}

#endif // __KGE_GRAPHICS_RENDER_PASS_GLES_H__