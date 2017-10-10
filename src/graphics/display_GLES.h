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

#include "index_buffer.h"

namespace kge
{
	class VertexBuffer;
	class Shader;

    class DisplayGLES : public DisplayWin
    {
    public:
		virtual bool Init(uint32 width, uint32 height) override;
		virtual void Fini() override;

		void BeginFrame() { }
		void EndFrame() { }
		void WaitQueueIdle() { }
		void BindVertexBuffer(const VertexBuffer* buffer);
		void BindIndexBuffer(const IndexBuffer* buffer, IndexType index_type);
		void BindVertexArray(const Ref<Shader>& shader, int pass_index);
		void DrawIndexed(int start, int count, IndexType index_type);
		void DisableVertexArray(const Ref<Shader>& shader, int pass_index);
		void SubmitQueue(void* cmd) { }

		void FlushContext();
		void SwapBuffers();

		uint32 GetDefaultDepthRenderBuffer();

		int32 GetMinUniformBufferOffsetAlignment() const { return _uniform_buffer_offset_alignment; }

	private:

		GLuint _default_depth_render_buffer;

		int32 _uniform_buffer_offset_alignment;
		std::string _extensions;
		std::string _device_name;
    };
}

#endif // __KGE_DISPLAY_GLES_H__
