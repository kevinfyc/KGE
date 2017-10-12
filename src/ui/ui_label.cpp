//
//  ui_label.cpp
//  libkge
//
//  Created by kevin on 17-10-11.
//
//
// 

#include "ui_label.h"
#include "util/string_tool.h"

#include "graphics/texture2D.h"
#include "graphics/material.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftoutln.h>

namespace kge
{
	DEFINE_COM_CLASS(UILabel);

	enum class TagType
	{
		Color,
		Shadow,
		Outline,
		Underline,
		Bold,
		Italic
	};

	struct TagInfo
	{
		std::string tag;
		TagType type;
		std::string value;
		int begin;
		int end;
	};

	UILabel::UILabel() :
		_font_style(FontStyle::Normal),
		_font_size(20),
		_line_space(1),
		_rich(false),
		_alignment(TextAlignment::UpperLeft)
	{
	}

	void UILabel::DeepCopy(const Ref<Object>& source)
	{
		UIView::DeepCopy(source);

		auto src = RefCast<UILabel>(source);
		_font = src->_font;
		_font_style = src->_font_style;
		_font_size = src->_font_size;
		_text = src->_text;
		_line_space = src->_line_space;
		_rich = src->_rich;
		_alignment = src->_alignment;
	}

	void UILabel::SetFont(const Ref<Font>& font)
	{
		if (_font != font)
		{
			_font = font;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UILabel::SetFontStyle(FontStyle style)
	{
		if (_font_style != style)
		{
			_font_style = style;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UILabel::SetFontSize(int size)
	{
		if (_font_size != size)
		{
			_font_size = size;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UILabel::SetText(const std::string& text)
	{
		if (_text != text)
		{
			_text = text;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UILabel::SetLineSpace(int space)
	{
		if (_line_space != space)
		{
			_line_space = space;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UILabel::SetRich(bool rich)
	{
		if (_rich != rich)
		{
			_rich = rich;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UILabel::SetAlignment(TextAlignment alignment)
	{
		if (_alignment != alignment)
		{
			_alignment = alignment;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	static bool check_tag_begin(std::vector<char32_t>& str, int& char_index, const std::string& tag_str, int value_length, TagInfo& tag)
	{
		bool match = true;
		auto tag_cstr = tag_str.c_str();

		for (uint32 i = 0; i < tag_str.size(); i++)
		{
			if (tag_cstr[i] != str[char_index + i])
			{
				match = false;
				break;
			}
		}

		if (match)
		{
			if (value_length > 0)
			{
				std::vector<char32_t> value;
				for (int i = 0; i < value_length; i++)
				{
					value.push_back(str[char_index + tag_str.size() + i]);
				}
				value.push_back(0);

				tag.tag = tag_str.substr(1, tag_str.size() - 3);

				std::vector<char> _;
				for (int i = 0; (&value[0])[i] != 0; ++i)
				{
					char32_t c32 = (&value[0])[i];
					auto bytes = unicode322utf8(c32);

					if (bytes.size() > 0)
					{
						auto old_size = _.size();
						_.resize(old_size + bytes.size());

						for (uint32 j = 0; j < bytes.size(); j++)
						{
							_[old_size + j] = (&bytes[0])[j];
						}
					}
				}

				_.push_back(0);

				tag.value = &_[0];

				str.erase(str.begin() + char_index, str.begin() + char_index + tag_str.size() + value_length + 1);
			}
			else
			{
				tag.tag = tag_str.substr(1, tag_str.size() - 2);

				str.erase(str.begin() + char_index, str.begin() + char_index + tag_str.size());
			}

			tag.begin = char_index--;
		}

		return match;
	}

	static bool check_tag_end(std::vector<char32_t>& str, int& char_index, const std::string& tag_str, std::vector<TagInfo>& tag_find, std::vector<TagInfo>& tags)
	{
		bool match = true;
		auto tag_cstr = tag_str.c_str();

		for (uint32 i = 0; i < tag_str.size(); i++)
		{
			if (tag_cstr[i] != str[char_index + i])
			{
				match = false;
				break;
			}
		}

		if (match)
		{
			auto tag = tag_str.substr(2, tag_str.size() - 3);

			for (uint32 i = tag_find.size() - 1; i >= 0; i--)
			{
				auto &t = tag_find[i];

				if (t.tag == tag)
				{
					str.erase(str.begin() + char_index, str.begin() + char_index + tag_str.size());
					t.end = char_index--;
					tags.push_back(t);
					tag_find.erase(tag_find.begin() + i);
					break;
				}
			}
		}

		return match;
	}

	static const std::string TAG_COLOR_BEGIN = "<color=#";
	static const std::string TAG_COLOR_END = "</color>";
	static const std::string TAG_SHADOW_BEGIN = "<shadow>";
	static const std::string TAG_SHADOW_VALUE_BEGIN = "<shadow=#";
	static const std::string TAG_SHADOW_END = "</shadow>";
	static const std::string TAG_OUTLINE_BEGIN = "<outline>";
	static const std::string TAG_OUTLINE_VALUE_BEGIN = "<outline=#";
	static const std::string TAG_OUTLINE_END = "</outline>";
	static const std::string TAG_UNDERLINE_BEGIN = "<underline>";
	static const std::string TAG_UNDERLINE_END = "</underline>";
	static const std::string TAG_BOLD_BEGIN = "<bold>";
	static const std::string TAG_BOLD_END = "</bold>";
	static const std::string TAG_ITALIC_BEGIN = "<italic>";
	static const std::string TAG_ITALIC_END = "</italic>";

	static std::vector<TagInfo> parse_rich_tags(std::vector<char32_t>& str)
	{
		std::vector<TagInfo> tags;
		std::vector<TagInfo> tag_find;

		for (int i = 0; i < (int)str.size(); i++)
		{
			TagInfo tag;

			if (check_tag_begin(str, i, TAG_COLOR_BEGIN, 8, tag))
			{
				tag.type = TagType::Color;
				tag_find.push_back(tag);
			}
			else if (check_tag_end(str, i, TAG_COLOR_END, tag_find, tags))
			{
			}
			else if (check_tag_begin(str, i, TAG_SHADOW_BEGIN, 0, tag))
			{
				tag.type = TagType::Shadow;
				tag.value = "000000ff";
				tag_find.push_back(tag);
			}
			else if (check_tag_begin(str, i, TAG_SHADOW_VALUE_BEGIN, 8, tag))
			{
				tag.type = TagType::Shadow;
				tag_find.push_back(tag);
			}
			else if (check_tag_end(str, i, TAG_SHADOW_END, tag_find, tags))
			{
			}
			else if (check_tag_begin(str, i, TAG_OUTLINE_BEGIN, 0, tag))
			{
				tag.type = TagType::Outline;
				tag.value = "000000ff";
				tag_find.push_back(tag);
			}
			else if (check_tag_begin(str, i, TAG_OUTLINE_VALUE_BEGIN, 8, tag))
			{
				tag.type = TagType::Outline;
				tag_find.push_back(tag);
			}
			else if (check_tag_end(str, i, TAG_OUTLINE_END, tag_find, tags))
			{
			}
			else if (check_tag_begin(str, i, TAG_UNDERLINE_BEGIN, 0, tag))
			{
				tag.type = TagType::Underline;
				tag_find.push_back(tag);
			}
			else if (check_tag_end(str, i, TAG_UNDERLINE_END, tag_find, tags))
			{
			}
			else if (check_tag_begin(str, i, TAG_BOLD_BEGIN, 0, tag))
			{
				tag.type = TagType::Bold;
				tag_find.push_back(tag);
			}
			else if (check_tag_end(str, i, TAG_BOLD_END, tag_find, tags))
			{
			}
			else if (check_tag_begin(str, i, TAG_ITALIC_BEGIN, 0, tag))
			{
				tag.type = TagType::Italic;
				tag_find.push_back(tag);
			}
			else if (check_tag_end(str, i, TAG_ITALIC_END, tag_find, tags))
			{
			}
		}

		return tags;
	}

	static Color string_to_color(std::string str)
	{
		LowerString(str);

		std::stringstream ss;
		ss << std::hex << str.c_str();
		uint32 color_i = 0;
		ss >> color_i;

		uint32 r = (color_i & 0xff000000) >> 24;
		uint32 g = (color_i & 0xff0000) >> 16;
		uint32 b = (color_i & 0xff00) >> 8;
		uint32 a = (color_i & 0xff);

		float div = 1 / 255.f;
		return Color((float)r, (float)g, (float)b, (float)a) * div;
	}

	std::vector<LabelLine> UILabel::ProcessText(int& actual_width, int& actual_height)
	{
		std::vector<char32_t> chars;
		ToUnicode32(_text, chars);

		std::vector<TagInfo> tags;
		if (_rich)
			tags = parse_rich_tags(chars);

		auto face = (FT_Face)_font->GetFont();
		auto label_size = GetSize();
		float v_size = 1.0f / FONT_TEXTURE_SIZE_MAX;
		int vertex_count = 0;
		auto has_kerning = FT_HAS_KERNING(face);
		FT_UInt previous = 0;
		int pen_x = 0;
		int pen_y = 0;
		int x_max = 0;
		int y_min = 0;
		int y_max = INT_MIN;
		int line_x_max = 0;
		int line_y_min = 0;
		std::vector<LabelLine> lines;
		static LabelLine line;

		for (int i = 0; i < (int)chars.size(); i++)
		{
			char32_t c = chars[i];

			int font_size = _font_size;
			Color color = _color;
			bool bold = _font_style == FontStyle::Bold || _font_style == FontStyle::BoldAndItalic;
			bool italic = _font_style == FontStyle::Italic || _font_style == FontStyle::BoldAndItalic;
			bool mono = font_size <= 16;
			Ref<Color> color_shadow;
			Ref<Color> color_outline;
			bool underline = false;

			if (c == '\n')
			{
				line.width = line_x_max;
				line.height = pen_y - line_y_min;
				line_x_max = 0;
				line_y_min = 0;
				pen_x = 0;
				pen_y += -(font_size + _line_space);

				lines.push_back(line);
				line.Clear();

				continue;
			}

			if (_rich)
			{
				for (auto& j : tags)
				{
					if (i >= j.begin && i < j.end)
					{
						switch (j.type)
						{
						case TagType::Color:
							color = string_to_color(j.value);
							break;
						case TagType::Bold:
							bold = true;
							break;
						case TagType::Italic:
							italic = true;
							break;
						case TagType::Shadow:
							color_shadow = RefMake<Color>(string_to_color(j.value));
							break;
						case TagType::Outline:
							color_outline = RefMake<Color>(string_to_color(j.value));
							break;
						case TagType::Underline:
							underline = true;
							break;
						}
					}
				}
			}

			GlyphInfo info = _font->GetGlyph(c, font_size, bold, italic, mono);

			//	limit width
			if (pen_x + info.bearing_x + info.uv_pixel_w > label_size.x)
			{
				pen_x = 0;
				pen_y += -(font_size + _line_space);
				previous = 0;
			}

			//	kerning
			if (has_kerning && previous && info.glyph_index)
			{
				FT_Vector delta;
				FT_Get_Kerning(face, previous, info.glyph_index, FT_KERNING_UNFITTED, &delta);
				pen_x += delta.x >> 6;
			}

			auto base_info = _font->GetGlyph('A', font_size, bold, italic, mono);
			int base_y0 = base_info.bearing_y;
			int base_y1 = base_info.bearing_y - base_info.uv_pixel_h;
			
			float tmp = base_y0 + (font_size - base_y0 + base_y1) * 0.5f;
			int baseline = (int)((tmp > 0.0f) ? (tmp + 0.5f) : (tmp - 0.5f));

			int x0 = pen_x + info.bearing_x;
			int y0 = pen_y + info.bearing_y - baseline;
			int x1 = x0 + info.uv_pixel_w;
			int y1 = y0 - info.uv_pixel_h;

			if (x_max < x1)
				x_max = x1;
			if (y_min > y1)
				y_min = y1;
			if (y_max < y0)
				y_max = y0;

			if (line_x_max < x1)
				line_x_max = x1;
			if (line_y_min > y1)
				line_y_min = y1;

			int char_space = 0;
			pen_x += info.advance_x + char_space;

			int uv_x0 = info.uv_pixel_x;
			int uv_y0 = info.uv_pixel_y;
			int uv_x1 = uv_x0 + info.uv_pixel_w;
			int uv_y1 = uv_y0 + info.uv_pixel_h;

			if (color_shadow)
			{
				Vector2 offset = Vector2(1, -1);

				line.vertices.push_back(Vector2((float)x0, (float)y0) + offset);
				line.vertices.push_back(Vector2((float)x0, (float)y1) + offset);
				line.vertices.push_back(Vector2((float)x1, (float)y1) + offset);
				line.vertices.push_back(Vector2((float)x1, (float)y0) + offset);
				line.uv.push_back(Vector2(uv_x0 * v_size, uv_y0 * v_size));
				line.uv.push_back(Vector2(uv_x0 * v_size, uv_y1 * v_size));
				line.uv.push_back(Vector2(uv_x1 * v_size, uv_y1 * v_size));
				line.uv.push_back(Vector2(uv_x1 * v_size, uv_y0 * v_size));
				line.colors.push_back(*color_shadow);
				line.colors.push_back(*color_shadow);
				line.colors.push_back(*color_shadow);
				line.colors.push_back(*color_shadow);
				line.indices.push_back(vertex_count + 0);
				line.indices.push_back(vertex_count + 1);
				line.indices.push_back(vertex_count + 2);
				line.indices.push_back(vertex_count + 0);
				line.indices.push_back(vertex_count + 2);
				line.indices.push_back(vertex_count + 3);

				vertex_count += 4;
			}

			if (color_outline)
			{
				Vector2 offsets[4];
				offsets[0] = Vector2(-1, 1);
				offsets[1] = Vector2(-1, -1);
				offsets[2] = Vector2(1, -1);
				offsets[3] = Vector2(1, 1);

				for (int j = 0; j < 4; j++)
				{
					line.vertices.push_back(Vector2((float)x0, (float)y0) + offsets[j]);
					line.vertices.push_back(Vector2((float)x0, (float)y1) + offsets[j]);
					line.vertices.push_back(Vector2((float)x1, (float)y1) + offsets[j]);
					line.vertices.push_back(Vector2((float)x1, (float)y0) + offsets[j]);
					line.uv.push_back(Vector2(uv_x0 * v_size, uv_y0 * v_size));
					line.uv.push_back(Vector2(uv_x0 * v_size, uv_y1 * v_size));
					line.uv.push_back(Vector2(uv_x1 * v_size, uv_y1 * v_size));
					line.uv.push_back(Vector2(uv_x1 * v_size, uv_y0 * v_size));
					line.colors.push_back(*color_outline);
					line.colors.push_back(*color_outline);
					line.colors.push_back(*color_outline);
					line.colors.push_back(*color_outline);
					line.indices.push_back(vertex_count + 0);
					line.indices.push_back(vertex_count + 1);
					line.indices.push_back(vertex_count + 2);
					line.indices.push_back(vertex_count + 0);
					line.indices.push_back(vertex_count + 2);
					line.indices.push_back(vertex_count + 3);

					vertex_count += 4;
				}
			}

			line.vertices.push_back(Vector2((float)x0, (float)y0));
			line.vertices.push_back(Vector2((float)x0, (float)y1));
			line.vertices.push_back(Vector2((float)x1, (float)y1));
			line.vertices.push_back(Vector2((float)x1, (float)y0));
			line.uv.push_back(Vector2(uv_x0 * v_size, uv_y0 * v_size));
			line.uv.push_back(Vector2(uv_x0 * v_size, uv_y1 * v_size));
			line.uv.push_back(Vector2(uv_x1 * v_size, uv_y1 * v_size));
			line.uv.push_back(Vector2(uv_x1 * v_size, uv_y0 * v_size));
			line.colors.push_back(color);
			line.colors.push_back(color);
			line.colors.push_back(color);
			line.colors.push_back(color);
			line.indices.push_back(vertex_count + 0);
			line.indices.push_back(vertex_count + 1);
			line.indices.push_back(vertex_count + 2);
			line.indices.push_back(vertex_count + 0);
			line.indices.push_back(vertex_count + 2);
			line.indices.push_back(vertex_count + 3);

			vertex_count += 4;
			previous = info.glyph_index;

			if (underline)
			{
				int ux0 = pen_x - (info.advance_x + char_space);
				int uy0 = pen_y - baseline - 2;
				int ux1 = ux0 + info.advance_x + char_space;
				int uy1 = uy0 - 1;

				line.vertices.push_back(Vector2((float)ux0, (float)uy0));
				line.vertices.push_back(Vector2((float)ux0, (float)uy1));
				line.vertices.push_back(Vector2((float)ux1, (float)uy1));
				line.vertices.push_back(Vector2((float)ux1, (float)uy0));
				line.uv.push_back(Vector2(0 * v_size, 0 * v_size));
				line.uv.push_back(Vector2(0 * v_size, 1 * v_size));
				line.uv.push_back(Vector2(1 * v_size, 1 * v_size));
				line.uv.push_back(Vector2(1 * v_size, 0 * v_size));
				line.colors.push_back(color);
				line.colors.push_back(color);
				line.colors.push_back(color);
				line.colors.push_back(color);
				line.indices.push_back(vertex_count + 0);
				line.indices.push_back(vertex_count + 1);
				line.indices.push_back(vertex_count + 2);
				line.indices.push_back(vertex_count + 0);
				line.indices.push_back(vertex_count + 2);
				line.indices.push_back(vertex_count + 3);

				vertex_count += 4;
			}
		}

		//	最后一行
		if (!line.vertices.empty())
		{
			line.width = line_x_max;
			line.height = pen_y - line_y_min;

			lines.push_back(line);
			line.Clear();
		}

		actual_width = x_max;
		actual_height = -y_min;

		return lines;
	}

	void UILabel::FillVertices(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices)
	{
		if (!_font)
			return;

		Vector2 size = this->GetSize();
		Vector2 min = Vector2(-_pivot.x * size.x, -_pivot.y * size.y);
		Vector2 max = Vector2((1 - _pivot.x) * size.x, (1 - _pivot.y) * size.y);

		int actual_width;
		int actual_height;
		auto lines = ProcessText(actual_width, actual_height);

		auto mat = GetVertexMatrix();
		int index_begin = vertices.size();

		for (uint32 i = 0; i < lines.size(); i++)
		{
			auto line = lines[i];

			for (uint32 j = 0; j < line.vertices.size(); j++)
			{
				auto v = line.vertices[j];

				switch (_alignment)
				{
				case TextAlignment::UpperLeft:
				case TextAlignment::MiddleLeft:
				case TextAlignment::LowerLeft:
					v.x += min.x;
					break;
				case TextAlignment::UpperCenter:
				case TextAlignment::MiddleCenter:
				case TextAlignment::LowerCenter:
					v.x += min.x + (size.x - line.width) / 2;
					break;
				case TextAlignment::UpperRight:
				case TextAlignment::MiddleRight:
				case TextAlignment::LowerRight:
					v.x += min.x + (size.x - line.width);
					break;
				}

				switch (_alignment)
				{
				case TextAlignment::UpperLeft:
				case TextAlignment::UpperCenter:
				case TextAlignment::UpperRight:
					v.y += max.y;
					break;
				case TextAlignment::MiddleLeft:
				case TextAlignment::MiddleCenter:
				case TextAlignment::MiddleRight:
					v.y += max.y - (size.y - actual_height) / 2;
					break;
				case TextAlignment::LowerLeft:
				case TextAlignment::LowerCenter:
				case TextAlignment::LowerRight:
					v.y += max.y - (size.y - actual_height);
					break;
				}

				vertices.push_back(mat.applyPoint(Vector3(v.x, v.y, 0)));
			}

			if (!line.vertices.empty())
			{
				if (line.uv.size() > 0)
				{
					auto old_size = uv.size();
					uv.resize(old_size + line.uv.size());

					for (uint32 i = 0; i < line.uv.size(); i++)
					{
						uv[old_size + i] = line.uv[i];
					}
				}
				if (line.colors.size() > 0)
				{
					auto old_size = colors.size();
					colors.resize(old_size + line.colors.size());

					for (uint32 i = 0; i < line.colors.size(); i++)
					{
						colors[old_size + i] = line.colors[i];
					}
				}
			}

			for (uint32 j = 0; j < line.indices.size(); j++)
			{
				indices.push_back(line.indices[j] + index_begin);
			}
		}

		_font->GetTexture()->EncodeToPNG("xxx.png");
	}

	void UILabel::FillMaterial(Ref<Material>& mat)
	{
		if (_font)
		{
			mat->SetMainTexture(_font->GetTexture());
		}
	}
}
