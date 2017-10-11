//
//  font.h
//  libkge
//
//  Created by kevin on 17-10-11.
//
//
// 

#ifndef __KGE_FONT_H__
#define __KGE_FONT_H__

#include "core/object.h"

#define FONT_TEXTURE_SIZE_MAX 2048

namespace kge
{
	struct GlyphInfo
	{
		char32_t c;
		uint32 size;
		unsigned int glyph_index;
		uint32 uv_pixel_x;
		uint32 uv_pixel_y;
		uint32 uv_pixel_w;
		uint32 uv_pixel_h;
		uint32 bearing_x;
		uint32 bearing_y;
		uint32 advance_x;
		uint32 advance_y;
		bool bold;
		bool italic;
		bool mono;
	};

	class Texture2D;

	class Font : public Object
	{
	public:
		static bool Init();
		static void Fini();
		static Ref<Font> LoadFromFile(const std::string& file);
		~Font();
		void* GetFont() const { return _font; }
		GlyphInfo GetGlyph(char32_t c, uint32 size, bool bold, bool italic, bool mono);
		const Ref<Texture2D>& GetTexture() const { return _texture; }

	private:
		Font();

		void* _font;
		std::map<char32_t, std::map<uint32, GlyphInfo>> _glyphs;
		Ref<Texture2D> _texture;
		uint32 _texture_x;
		uint32 _texture_y;
		uint32 _texture_line_h_max;
	};
}

#endif // __KGE_FONT_H__
