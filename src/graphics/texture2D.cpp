﻿//
//  texture2D.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "texture2D.h"

#include <assert.h>

#include "io/file_tool.h"
#include "image.h"

namespace kge
{
	Texture2D::Texture2D()
	{
		SetName("Texture2D");
	}

	Ref<Texture2D> Texture2D::LoadFromData(const ByteBuffer& buffer,
		TextureWrapMode wrap_mode,
		FilterMode filter_mode,
		bool mipmap)
	{
		Ref<Texture2D> texture;

		const byte JPG_HEAD[] = { 0xff, 0xd8, 0xff };
		const byte PNG_HEAD[] = { 0x89, 0x50, 0x4e, 0x47 };

		int width = 0, height = 0, bpp = 0;
		ByteBuffer colors;
		if (::memcmp(buffer.Bytes(), (void*)JPG_HEAD, 3) == 0)
		{
			colors = Image::LoadJPEG(buffer, width, height, bpp);
		}
		else if (::memcmp(buffer.Bytes(), (void*)PNG_HEAD, 4) == 0)
		{
			colors = Image::LoadPNG(buffer, width, height, bpp);
		}
		else
		{
			assert(!"invalid image file format");
		}

		TextureFormat format;
		if (bpp == 32)
		{
			format = TextureFormat::RGBA32;
		}
		else if (bpp == 24)
		{
			format = TextureFormat::RGB24;
		}
		else if (bpp == 8)
		{
			format = TextureFormat::Alpha8;
		}
		else
		{
			assert(!"invalid image file bpp");
			format = TextureFormat::RGBA32;
		}

		texture = Create(width, height, format, wrap_mode, filter_mode, mipmap, colors);

		return texture;
	}

	Ref<Texture2D> Texture2D::LoadFromFile(const std::string& file,
		TextureWrapMode wrap_mode,
		FilterMode filter_mode,
		bool mipmap)
	{
		Ref<Texture2D> texture;

		ByteBuffer bytes = ReadFile(file, true);
		if (bytes.Size() > 0)
		{
			texture = LoadFromData(bytes, wrap_mode, filter_mode, mipmap);
		}

		return texture;
	}

	void Texture2D::EncodeToPNG(const std::string& file)
	{
		int bpp;
		auto format = this->GetFormat();
		switch (format)
		{
		case TextureFormat::RGBA32:
			bpp = 32;
			break;
		case TextureFormat::RGB24:
			bpp = 24;
			break;
		case TextureFormat::Alpha8:
			bpp = 8;
			break;
		default:
			bpp = 0;
			break;
		}

		//Image::EncodeToPNG(this, bpp, file);
	}

	Ref<Texture2D> Texture2D::Create(
		int width,
		int height,
		TextureFormat format,
		TextureWrapMode wrap_mode,
		FilterMode filter_mode,
		bool mipmap,
		const ByteBuffer& colors)
	{
		Ref<Texture2D> texture = Ref<Texture2D>(new Texture2D());
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFormat(format);
		texture->SetWrapMode(wrap_mode);
		texture->SetFilterMode(filter_mode);
		texture->SetMipmap(mipmap);
		texture->_colors = colors;

		texture->CreateTexture2D();

		return texture;
	}

	void Texture2D::UpdateTexture(int x, int y, int w, int h, const ByteBuffer& colors)
	{
		this->UpdateTexture2D(x, y, w, h, colors);
	}
}

