//
//  image.h
//  libkge
//
//  Created by kevin on 17-9-26.
//
//

#ifndef __KGE_GRAPHICS_IMAGE_H__
#define __KGE_GRAPHICS_IMAGE_H__

#include <string.h>
#include "util/byte_buffer.h"

namespace kge
{
	class Texture2D;

	class Image
	{
	public:
		static ByteBuffer LoadJPEG(const ByteBuffer& jpeg, int& width, int& height, int& bpp);
		static ByteBuffer LoadPNG(const ByteBuffer& png, int& width, int& height, int& bpp);
		static void EncodeToPNG(Texture2D *tex, int bpp, const std::string& file);
	};
}

#endif // __KGE_GRAPHICS_IMAGE_H__