//
//  ui_label.h
//  libkge
//
//  Created by kevin on 17-10-11.
//
//
// 

#ifndef __KGE_UI_LABEL_H__
#define __KGE_UI_LABEL_H__

#include "ui_view.h"
#include "font.h"

namespace kge
{
	enum class FontStyle
	{
		Normal,
		Bold,
		Italic,
		BoldAndItalic
	};

	enum class TextAlignment
	{
		UpperLeft,
		UpperCenter,
		UpperRight,
		MiddleLeft,
		MiddleCenter,
		MiddleRight,
		LowerLeft,
		LowerCenter,
		LowerRight
	};

	struct LabelLine
	{
		int width;
		int height;
		std::vector<Vector2> vertices;
		std::vector<Vector2> uv;
		std::vector<Color> colors;
		std::vector<uint16> indices;

		void Clear()
		{
			vertices.clear();
			uv.clear();
			colors.clear();
			indices.clear();
		}
	};

	class UILabel: public UIView
	{
		DECLARE_COM_CLASS(UILabel, UIView);
	public:
		void SetFont(const Ref<Font>& font);
		void SetFontStyle(FontStyle style);
		void SetFontSize(int size);
		void SetText(const std::string& text);
		void SetLineSpace(int space);
		void SetRich(bool rich);
		void SetAlignment(TextAlignment alignment);

		virtual void FillVertices(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices);
		virtual void FillMaterial(Ref<Material>& mat);

	protected:
		UILabel();
		std::vector<LabelLine> ProcessText(uint32& actual_width, uint32& actual_height);

		Ref<Font> m_font;
		FontStyle m_font_style;
		int m_font_size;
		std::string m_text;
		int m_line_space;
		bool m_rich;
		TextAlignment m_alignment;
	};
}

#endif // __KGE_UI_LABEL_H__
