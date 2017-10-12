//
//  font.cpp
//  libkge
//
//  Created by kevin on 17-10-11.
//
//
// 

#include "font.h"

#include "graphics/Texture2D.h"

#include "io/file_tool.h"

#include "util/log.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftoutln.h>

namespace kge
{
	static FT_Library g_ft_lib;


	bool Font::Init()
	{
		return FT_Init_FreeType(&g_ft_lib) == 0;
	}

	void Font::Fini()
	{
		FT_Done_FreeType(g_ft_lib);
	}

	Ref<Font> Font::LoadFromFile(const std::string& file)
	{
		Ref<Font> font;

		if (!FileSystemMgr::GetInstance()->getFileSystem()->isFileExist(file))
		{
			KGE_LOG_ERROR("Failed to load font file '%s'", file.c_str());
			return font;
		}
		
		std::string path = FileSystemMgr::GetInstance()->getFileSystem()->getFullPath(file);

		FT_Face face;
		auto err = FT_New_Face(g_ft_lib, path.c_str(), 0, &face);
		if (!err)
		{
			font = Ref<Font>(new Font());
			font->_font = (void*)face;
		}

		return font;
	}

	Font::Font() :
		_font(nullptr),
		_texture_x(0),
		_texture_y(0),
		_texture_line_h_max(0)
	{
		auto buffer = ByteBuffer(FONT_TEXTURE_SIZE_MAX * FONT_TEXTURE_SIZE_MAX);
		memset(buffer.Bytes(), 0, buffer.Size());
		_texture = Texture2D::Create(
			FONT_TEXTURE_SIZE_MAX, FONT_TEXTURE_SIZE_MAX,
			TextureFormat::Alpha8,
			TextureWrapMode::Clamp, FilterMode::Point,
			false,
			buffer);
	}

	Font::~Font()
	{
		if (_font)
			FT_Done_Face((FT_Face)_font);
	}

	GlyphInfo Font::GetGlyph(char32_t c, int size, bool bold, bool italic, bool mono)
	{
		int size_key = size | (bold ? (1 << 24) : 0) | (italic ? (1 << 16) : 0);

		std::map<int, GlyphInfo>* p_size_glyphs = nullptr;
		auto iter_glphy = _glyphs.find(c);
		if (iter_glphy == _glyphs.end())
		{
			std::map<int, GlyphInfo> size_glyphs;
			_glyphs.insert(std::pair<char32_t, std::map<int, GlyphInfo>>(c, size_glyphs));

			p_size_glyphs = &_glyphs[c];
		}
		else
		{
			p_size_glyphs = &iter_glphy->second;
		}

		GlyphInfo* p_glyph = nullptr;
		auto iter = p_size_glyphs->find(size_key);
		if (iter == p_size_glyphs->end())
		{
			GlyphInfo glyph;
			p_size_glyphs->insert(std::pair<int, GlyphInfo>(size_key, glyph));

			p_glyph = &(*p_size_glyphs)[size_key];
		}
		else
		{
			p_glyph = &iter->second;
			return *p_glyph;
		}

		p_glyph->c = c;
		p_glyph->size = size;
		p_glyph->bold = bold;
		p_glyph->italic = italic;
		p_glyph->mono = mono;

		FT_Face face = (FT_Face)_font;
		FT_Set_Pixel_Sizes(face, 0, size);

		FT_GlyphSlot slot = face->glyph;
		auto glyph_index = FT_Get_Char_Index(face, c);

		if (mono)
		{
			FT_Load_Char(face, c, FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO);
		}
		else
		{
			FT_Load_Char(face, c, FT_LOAD_DEFAULT);
		}

		if (bold)
		{
			FT_Outline_Embolden(&face->glyph->outline, 1 << 6);
		}

		if (italic)
		{
			float lean = 0.5f;
			FT_Matrix matrix;
			matrix.xx = 1 << 16;
			matrix.xy = (int)(lean * (1 << 16));
			matrix.yx = 0;
			matrix.yy = (1 << 16);
			FT_Outline_Transform(&face->glyph->outline, &matrix);
		}

		if (mono)
		{
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
		}
		else
		{
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		}

		p_glyph->glyph_index = glyph_index;
		p_glyph->uv_pixel_w = slot->bitmap.width;
		p_glyph->uv_pixel_h = slot->bitmap.rows;
		p_glyph->bearing_x = slot->bitmap_left;
		p_glyph->bearing_y = slot->bitmap_top;
		p_glyph->advance_x = (int)(slot->advance.x >> 6);
		p_glyph->advance_y = (int)(slot->advance.y >> 6);

		if (_texture_y + p_glyph->uv_pixel_h <= FONT_TEXTURE_SIZE_MAX)
		{
			auto colors = _texture->GetColors();

			//insert one white pixel for underline
			if (_texture_x == 0 && _texture_y == 0)
			{
				ByteBuffer buffer(1);
				buffer[0] = 0xff;
				colors[0] = 0xff;
				_texture->UpdateTexture(0, 0, 1, 1, buffer);
				_texture_x += 1;
			}

			if (_texture_x + p_glyph->uv_pixel_w > FONT_TEXTURE_SIZE_MAX)
			{
				_texture_y += _texture_line_h_max;
				_texture_x = 0;
				_texture_line_h_max = 0;
			}

			if (_texture_line_h_max < p_glyph->uv_pixel_h)
			{
				_texture_line_h_max = p_glyph->uv_pixel_h;
			}

			ByteBuffer char_pixels;

			if (mono)
			{
				char_pixels = ByteBuffer(p_glyph->uv_pixel_w * p_glyph->uv_pixel_h);

				for (int i = 0; i < p_glyph->uv_pixel_h; i++)
				{
					for (int j = 0; j < p_glyph->uv_pixel_w; j++)
					{
						unsigned char bit = slot->bitmap.buffer[i * slot->bitmap.pitch + j / 8] & (0x1 << (7 - j % 8));
						bit = bit == 0 ? 0 : 255;
						char_pixels[i * p_glyph->uv_pixel_w + j] = bit;
					}
				}
			}
			else
			{
				char_pixels = ByteBuffer(slot->bitmap.buffer, p_glyph->uv_pixel_w * p_glyph->uv_pixel_h);
			}

			for (int i = 0; i < p_glyph->uv_pixel_h; i++)
			{
				::memcpy(&colors[FONT_TEXTURE_SIZE_MAX * (_texture_y + i) + _texture_x], &char_pixels[p_glyph->uv_pixel_w * i], p_glyph->uv_pixel_w);
			}

			if (p_glyph->uv_pixel_w > 0 && p_glyph->uv_pixel_h > 0)
			{
				_texture->UpdateTexture(_texture_x, _texture_y, p_glyph->uv_pixel_w, p_glyph->uv_pixel_h, char_pixels);
			}

			p_glyph->uv_pixel_x = _texture_x;
			p_glyph->uv_pixel_y = _texture_y;

			_texture_x += p_glyph->uv_pixel_w;
		}
		else
		{
			KGE_LOG_ERROR("font texture size over than %d", FONT_TEXTURE_SIZE_MAX);
		}

		return *p_glyph;
	}
}
