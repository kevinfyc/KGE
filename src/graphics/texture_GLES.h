//
//  texture_GLES.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_TEXTURE_GLES_H__
#define __KGE_GRAPHICS_TEXTURE_GLES_H__

#include "core/object.h"
#include "common.h"
#include "util/byte_buffer.h"

namespace kge
{
	class TextureGLES : public Object
	{
	public:
		virtual ~TextureGLES();
		GLuint GetTexture() const{ return _texture; }
		void UpdateSampler();

	protected:
		TextureGLES();
		void CreateColorRenderTexture();
		void CreateDepthRenderTexture();
		void CreateTexture2D();
		void UpdateTexture2D(uint32 x, uint32 y, uint32 w, uint32 h, const ByteBuffer& color);

	private:
		void Create(GLenum format, GLenum type, void* pixels, bool mipmap);
		uint32 GetMipCount();

	private:
		GLuint _texture;
		GLint _format;
	};
}

#endif // __KGE_GRAPHICS_TEXTURE_GLES_H__