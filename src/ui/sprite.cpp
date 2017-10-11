//
//  sprite.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#include "sprite.h"

namespace kge
{
	Ref<Sprite> Sprite::Create(const Rect& rect, const Vector4& border)
	{
		Ref<Sprite> sprite;

		sprite = Ref<Sprite>(new Sprite());
		sprite->_rect = rect;
		sprite->_border = border;

		return sprite;
	}

	Sprite::Sprite():
		_rect(0, 0, 1, 1),
		_border(0, 0, 0, 0)
	{
	}
}
