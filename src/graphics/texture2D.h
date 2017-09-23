//
//  texture2D.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_TEXTURE2D_H__
#define __KGE_GRAPHICS_TEXTURE2D_H__

#include "texture.h"
#include "texture_format.h"

namespace kge
{
	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> LoadFromFile(const std::string& file,
			TextureWrapMode wrap_mode = TextureWrapMode::Clamp,
			FilterMode filter_mode = FilterMode::Bilinear,
			bool mipmap = false);

		static Ref<Texture2D> LoadFromData(const ByteBuffer& buffer,
			TextureWrapMode wrap_mode = TextureWrapMode::Clamp,
			FilterMode filter_mode = FilterMode::Bilinear,
			bool mipmap = false);

		static Ref<Texture2D> Create(
			int width,
			int height,
			TextureFormat format,
			TextureWrapMode wrap_mode,
			FilterMode filter_mode,
			bool mipmap,
			const ByteBuffer& colors);

		ByteBuffer GetColors() const { return _colors; }
		void UpdateTexture(int x, int y, int w, int h, const ByteBuffer& colors);
		void EncodeToPNG(const std::string& file);
		TextureFormat GetFormat() const { return _format; }
		bool IsMipmap() const { return _mipmap; }

	private:
		Texture2D();
		void SetFormat(TextureFormat format) { _format = format; }
		void SetMipmap(bool mipmap) { _mipmap = mipmap; }

	private:
		TextureFormat _format;
		bool _mipmap;
		ByteBuffer _colors;
	};
}

#endif // __KGE_GRAPHICS_TEXTURE2D_H__