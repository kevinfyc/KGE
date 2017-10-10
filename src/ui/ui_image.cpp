//
//  ui_image.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//


#include "ui_image.h"
#include "graphics/Material.h"

namespace kge
{
	DEFINE_COM_CLASS(UIImage);

	UIImage::UIImage():
		_sprite_type(SpriteType::Simple),
		_fill_method(SpriteFillMethod::Horizontal),
		_fill_origin((uint32) SpriteFillOriginHorizontal::Left),
		_fill_amount(1),
		_fill_clock_wise(false)
	{
	}

	void UIImage::DeepCopy(const Ref<Object>& source)
	{
		UIView::DeepCopy(source);

		auto src = RefCast<UIImage>(source);
		_sprite_group = src->_sprite_group;
		_sprite_name = src->_sprite_name;
		_sprite_type = src->_sprite_type;
		_fill_method = src->_fill_method;
		_fill_origin = src->_fill_origin;
		_fill_amount = src->_fill_amount;
		_fill_clock_wise = src->_fill_clock_wise;
	}

	void UIImage::SetSpriteGroup(const Ref<SpriteGroup>& atlas)
	{
		if (_sprite_group != atlas)
		{
			_sprite_group = atlas;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::SetSpriteName(const std::string& name)
	{
		if (_sprite_name != name)
		{
			_sprite_name = name;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::SetSpriteType(SpriteType type)
	{
		if (_sprite_type != type)
		{
			_sprite_type = type;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::SetFillMethod(SpriteFillMethod fill_method)
	{
		if (_fill_method != fill_method)
		{
			_fill_method = fill_method;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::SetFillOrigin(uint32 fill_origin)
	{
		if (_fill_origin != fill_origin)
		{
			_fill_origin = fill_origin;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::SetFillAmount(float fill_amount)
	{
		if (_fill_amount != fill_amount)
		{
			_fill_amount = fill_amount;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::SetFillClockWise(bool fill_clock_wise)
	{
		if (_fill_clock_wise != fill_clock_wise)
		{
			_fill_clock_wise = fill_clock_wise;
			_dirty = true;
			MarkRendererDirty();
		}
	}

	void UIImage::FillVerticesSimple(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices)
	{
		auto vertex_array = GetBoundsVertices();
		auto old_size = vertices.size();
		vertices.resize(old_size + 4);

		for (uint32 i = 0; i < 4; i++)
		{
			vertices[old_size + i] = vertex_array[i];
		}

		Vector2 uv_array[4];
		if (_sprite_group)
		{
			auto sprite = _sprite_group->GetSprite(_sprite_name);
			auto rect = sprite->GetRect();

			uv_array[0] = Vector2(rect.x, 1 - rect.y);
			uv_array[1] = Vector2(rect.x + rect.width, 1 - rect.y);
			uv_array[2] = Vector2(rect.x + rect.width, 1 - (rect.y + rect.height));
			uv_array[3] = Vector2(rect.x, 1 - (rect.y + rect.height));
		}
		else
		{
			uv_array[0] = Vector2(0, 1);
			uv_array[1] = Vector2(1, 1);
			uv_array[2] = Vector2(1, 0);
			uv_array[3] = Vector2(0, 0);
		}

		old_size = uv.size();
		uv.resize(old_size + 4);

		for (uint32 i = 0; i < 4; i++)
		{
			uv[old_size + i] = uv_array[i];
		}

		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);
		colors.push_back(_color);

		uint32 index_begin = vertices.size() - 4;
		indices.push_back(index_begin + 0);
		indices.push_back(index_begin + 1);
		indices.push_back(index_begin + 2);
		indices.push_back(index_begin + 0);
		indices.push_back(index_begin + 2);
		indices.push_back(index_begin + 3);
	}

	void UIImage::FillVertices(std::vector<Vector3>& vertices, std::vector<Vector2>& uv, std::vector<Color>& colors, std::vector<uint16>& indices)
	{
		switch (_sprite_type)
		{
			case SpriteType::Simple:
				this->FillVerticesSimple(vertices, uv, colors, indices);
				break;
			default:
				break;
		}
	}

	void UIImage::FillMaterial(Ref<Material>& mat)
	{
		if (_sprite_group)
		{
			mat->SetMainTexture(_sprite_group->GetTexture());
		}
	}
}
