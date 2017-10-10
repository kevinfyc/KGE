//
//  sprite.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_SPRITE_H__
#define __KGE_SPRITE_H__

#include "core/object.h"
#include "math/rect.h"
#include "math/vector2.hpp"
#include "math/vector4.hpp"
#include "graphics/texture2D.h"

namespace kge
{
	class Sprite: public Object
	{
	public:
		static Ref<Sprite> Create(const Rect& rect, const Vector4& border);

		const Rect& GetRect() const { return _rect; }
		const Vector4& GetBorder() const { return _border; }

	private:
		Sprite();

		Rect _rect;
		Vector4 _border;
	};
}

#endif // __KGE_SPRITE_H__
