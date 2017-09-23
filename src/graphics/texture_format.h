//
//  texture_format.h
//  libkge
//
//  Created by kevin on 17-9-23.
//
//

#ifndef __KGE_GRAPHICS_TEXTURE_FORMAT_H__
#define __KGE_GRAPHICS_TEXTURE_FORMAT_H__

namespace kge
{
	enum class TextureFormat
	{
		Alpha8,
		ARGB4444,
		RGB24,
		RGBA32,
		ARGB32,
		RGB565,
		DXT1,
		DXT5,
		RGBA4444,
		PVRTC_RGB2,
		PVRTC_RGBA2,
		PVRTC_RGB4,
		PVRTC_RGBA4,
		ETC_RGB4,
		ATC_RGB4,
		ATC_RGBA8,
		BGRA32,
		ATF_RGB_DXT1,
		ATF_RGBA_JPG,
		ATF_RGB_JPG,

		ETC_RGB4_X2,
		PVRTC_RGB4_X2,
	};
} // end of namespace kge

#endif // __KGE_GRAPHICS_RENDER_TEXTURE_H__
