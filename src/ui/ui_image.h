//
//  ui_image.h
//  libkge
//
//  Created by kevin on 17-10-10.
//

#ifndef __KGE_UI_IMAGE_H__
#define __KGE_UI_IMAGE_H__

#include "ui_view.h"
#include "sprite_group.h"

namespace kge
{
	enum class SpriteType
	{
		Simple = 0,
		Sliced = 1,
		Tiled = 2,
		Filled = 3
	};

	enum class SpriteFillMethod
	{
		Horizontal = 0,
		Vertical = 1,
		Radial90 = 2,
		Radial180 = 3,
		Radial360 = 4
	};

	enum class SpriteFillOrigin180
	{
		Bottom = 0,
		Top = 2,
		Right = 3
	};

	enum class SpriteFillOrigin360
	{
		Bottom = 0,
		Right = 1,
		Top = 2,
		Left = 3
	};

	enum class SpriteFillOrigin90
	{
		BottomLeft = 0,
		TopLeft = 1,
		TopRight = 2,
		BottomRight = 3
	};

	enum class SpriteFillOriginHorizontal
	{
		Left = 0,
		Right = 1
	};

	enum class SpriteFillOriginVertical
	{
		Bottom = 0,
		Top = 1
	};

	class UIImage: public UIView
	{
		DECLARE_COM_CLASS(UIImage, UIView);
	public:
		void SetSpriteGroup(const Ref<SpriteGroup>& atlas);
		void SetSpriteName(const std::string& name);
		void SetSpriteType(SpriteType type);
		void SetFillMethod(SpriteFillMethod fill_method);
		void SetFillOrigin(uint32 fill_origin);
		void SetFillAmount(float fill_amount);
		void SetFillClockWise(bool fill_clock_wise);
		const Ref<SpriteGroup>& GetSpriteGroup() const { return _sprite_group; }
		const std::string& GetSpriteName() const { return _sprite_name; }
		SpriteType GetSpriteType() const { return _sprite_type; }
		SpriteFillMethod GetFillMethod() const { return _fill_method; }
		uint32 GetFillOrigin() const { return _fill_origin; }
		float GetFillAmount() const { return _fill_amount; }
		bool GetFillClockWise() const { return _fill_clock_wise; }
		virtual void FillVertices(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices);
		virtual void FillMaterial(Ref<Material>& mat);

	protected:
		UIImage();
		void FillVerticesSimple(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices);

		Ref<SpriteGroup> _sprite_group;
		std::string _sprite_name;
		SpriteType _sprite_type;
		SpriteFillMethod _fill_method;
		uint32 _fill_origin;
		float _fill_amount;
		bool _fill_clock_wise;
	};
}

#endif // __KGE_UI_IMAGE_H__
