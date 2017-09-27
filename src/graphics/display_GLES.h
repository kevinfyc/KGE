//
//  display_GLES.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_DISPLAY_GLES_H__
#define __KGE_DISPLAY_GLES_H__

#if WIN32
#include "display_win.h"
#endif

namespace kge
{
    class DisplayGLES : public DisplayWin
    {
    public:
		virtual bool Init(uint32 width, uint32 height) override;
		virtual void Fini() override;

		void SwapBuffers();

		uint32 GetDefaultDepthRenderBuffer();

		int GetMinUniformBufferOffsetAlignment() const { return _uniform_buffer_offset_alignment; }

	private:

		GLuint _default_depth_render_buffer;

		int _uniform_buffer_offset_alignment;
		std::string _extensions;
		std::string _device_name;
    };
}

#endif // __KGE_DISPLAY_GLES_H__
