//
//  texture_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#include "texture_GLES.h"

#include "render_texture.h"
#include "texture2D.h"

#include "util/log.h"

#include "math/_math.hpp"

#include <assert.h>

namespace kge
{
	TextureGLES::TextureGLES() :_texture(0)
	{

	}

	TextureGLES::~TextureGLES()
	{
		glDeleteTextures(1, &_texture);
	}

	void TextureGLES::CreateColorRenderTexture()
	{
		RenderTexture* texture = dynamic_cast<RenderTexture*>(this);
		RenderTextureFormat texture_format = texture->GetFormat();

		GLenum format = 0;
		GLenum type = 0;
		if (texture_format == RenderTextureFormat::RGBA32)
		{
			_format = GL_RGBA;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
		}
		else
		{
			assert(!"color format not invalid");
		}

		Create(format, type, nullptr, false);
	}

	void TextureGLES::CreateDepthRenderTexture()
	{
		RenderTexture* texture = dynamic_cast<RenderTexture*>(this);
		DepthBuffer depth = texture->GetDepth();

		GLenum format = 0;
		GLenum type = 0;
		switch (depth)
		{
		case kge::DepthBuffer::Depth_16:
			_format = GL_DEPTH_COMPONENT16;
			format = GL_DEPTH_COMPONENT;
			type = GL_UNSIGNED_SHORT;
			break;
		case kge::DepthBuffer::Depth_24:
			_format = GL_DEPTH_COMPONENT24;
			format = GL_DEPTH_COMPONENT;
			type = GL_UNSIGNED_INT;
			break;
		case kge::DepthBuffer::Depth_24_Stencil_8:
			_format = GL_DEPTH24_STENCIL8;
			format = GL_DEPTH_STENCIL;
			type = GL_UNSIGNED_INT_24_8;
			break;
		case kge::DepthBuffer::Depth_32:
			_format = GL_DEPTH_COMPONENT32F;
			format = GL_DEPTH_COMPONENT;
			type = GL_FLOAT;
			break;
		default:
			assert(!"depth format not invalid");
			break;
		}

		Create(format, type, nullptr, false);
	}

	void TextureGLES::CreateTexture2D()
	{
		auto texture = (Texture2D*) this;
		auto texture_format = texture->GetFormat();
		auto colors = texture->GetColors();
		auto mipmap = texture->IsMipmap();

		GLenum type = 0;
		
		switch (texture_format)
		{
		case kge::TextureFormat::Alpha8:
			_format = GL_LUMINANCE;
			type = GL_UNSIGNED_BYTE;
			break;
		case kge::TextureFormat::RGB24:
			_format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			break;
		case kge::TextureFormat::RGBA32:
			_format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
		default:
			assert(!"texture format not implement");
			break;
		}


		Create(_format, type, colors.Bytes(), mipmap);
	}

	void TextureGLES::UpdateTexture2D(uint32 x, uint32 y, uint32 w, uint32 h, const ByteBuffer& colors)
	{
		Texture2D* texture = dynamic_cast<Texture2D*>(this);
		TextureFormat texture_format = texture->GetFormat();

		GLenum format = 0;
		GLenum type = 0;

		switch (texture_format)
		{
		case kge::TextureFormat::Alpha8:
			format = GL_LUMINANCE;
			type = GL_UNSIGNED_BYTE;
			break;
		case kge::TextureFormat::RGB24:
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			break;
		case kge::TextureFormat::RGBA32:
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
		default:
			assert(!"texture format not implement");
			break;
		}

		glBindTexture(GL_TEXTURE_2D, _texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, format, type, colors.Bytes());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextureGLES::Create(GLenum format, GLenum type, void* pixels, bool mipmap)
	{
		Texture* texture = dynamic_cast<Texture*>(this);
		uint32 width = texture->GetWidth();
		uint32 height = texture->GetHeight();

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, _format, width, height, 0, format, type, pixels);

		if (mipmap)
			GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D) );

		glBindTexture(GL_TEXTURE_2D, 0);

		UpdateSampler();
	}

	uint32 TextureGLES::GetMipCount()
	{
		uint32 mip_count = 0;

		RenderTexture * rt = dynamic_cast<RenderTexture*>(this);
		Texture2D* texture_2d = dynamic_cast<Texture2D*>(this);

		if (rt)
		{
			mip_count = 0;
		}
		else if (texture_2d)
		{
			int width = texture_2d->GetWidth();
			int height = texture_2d->GetHeight();
			bool mipmap = texture_2d->IsMipmap();

			mip_count = mipmap ? (uint32)floor(log2((float)max2<float>(width, height))) + 1 : 1;
		}

		return mip_count;
	}

	void TextureGLES::UpdateSampler()
	{
		bool mipmap = GetMipCount() > 1;

		glBindTexture(GL_TEXTURE_2D, _texture);

		Texture* texture = dynamic_cast<Texture*>(this);
		GLuint filter_min = 0;
		GLuint filter_mag = 0;
		FilterMode filter_mode = texture->GetFilterMode();
		switch (filter_mode)
		{
		case FilterMode::Point:
			filter_min = mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
			filter_mag = GL_NEAREST;
			break;
		case FilterMode::Bilinear:
		case FilterMode::Trilinear:
			filter_min = mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			filter_mag = GL_LINEAR;
			break;
		}

		GLuint address_mode = 0;
		TextureWrapMode wrap_mode = texture->GetWrapMode();
		switch (wrap_mode)
		{
		case TextureWrapMode::Repeat:
			address_mode = GL_REPEAT;
			break;
		case TextureWrapMode::Clamp:
			address_mode = GL_CLAMP_TO_EDGE;
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, address_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, address_mode);

		if (_format == GL_DEPTH_COMPONENT16 ||
			_format == GL_DEPTH_COMPONENT24 ||
			_format == GL_DEPTH24_STENCIL8 ||
			_format == GL_DEPTH_COMPONENT32F)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

