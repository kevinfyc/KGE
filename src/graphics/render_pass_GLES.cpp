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

namespace kge
{
	RenderPassGLES::RenderPassGLES() :_frameBuffer(0)
	{

	}

	RenderPassGLES::~RenderPassGLES()
	{

	}

	void RenderPassGLES::_Create()
	{
		KGE_LOG_GL_ERROR();

		RenderPass* pass = dynamic_cast<RenderPass*>(this);

		bool is_default = !pass->HasFrameBuffer();

		if (is_default)
		{
			_frameBuffer = 0;
		}
		else
		{

		}
	}
}

