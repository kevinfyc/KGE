//
//  texture.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_TEXTURE_H__
#define __KGE_GRAPHICS_TEXTURE_H__

#if KGE_GLES
#include "texture_GLES.h"
#endif

#include "filter_mode.h"
#include "texture_wrap_mode.h"

namespace kge
{
#if KGE_GLES
	class Texture : public TextureGLES
	{
		friend class TextureGLES;
#endif
	public:
		Texture()
		{
			SetName("Texture");
			SetWidth(0);
			SetHeight(0);
		}

		uint32 GetWidth() const { return _width; }
		uint32 GetHeight() const { return _height; }
		TextureWrapMode GetWrapMode() const { return _wrap_mode; }
		void SetWrapMode(TextureWrapMode mode) { _wrap_mode = mode; }
		FilterMode GetFilterMode() const { return _filter_mode; }
		void SetFilterMode(FilterMode mode) { _filter_mode = mode; }

	protected:
		void SetWidth(uint32 witdh) { _width = witdh; }
		void SetHeight(uint32 height) { _height = height; }

	protected:
		uint32 _width;
		uint32 _height;
		TextureWrapMode _wrap_mode;
		FilterMode _filter_mode;
	};
}

#endif // __KGE_GRAPHICS_TEXTURE_H__