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
		wchar_t c;
		int size;
		unsigned int glyph_index;
		int uv_pixel_x;
		int uv_pixel_y;
		int uv_pixel_w;
		int uv_pixel_h;
		int bearing_x;
		int bearing_y;
		int advance_x;
		int advance_y;
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
		GlyphInfo GetGlyph(wchar_t c, int size, bool bold, bool italic, bool mono);
		const Ref<Texture2D>& GetTexture() const { return _texture; }

	private:
		Font();

		void* _font;
		std::map<wchar_t, std::map<int, GlyphInfo>> _glyphs;
		Ref<Texture2D> _texture;
		int _texture_x;
		int _texture_y;
		int _texture_line_h_max;
	};
}

#endif // __KGE_FONT_H__
