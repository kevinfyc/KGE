//
//  sprite_group.cpp
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#include "sprite_group.h"

namespace kge
{
	Ref<SpriteGroup> SpriteGroup::Create()
	{
		return Ref<SpriteGroup>(new SpriteGroup());
	}

	SpriteGroup::SpriteGroup()
	{
	}

	void SpriteGroup::AddSprite(const std::string& name, const Ref<Sprite>& sprite)
	{
		_sprites.insert(std::pair<std::string, Ref<Sprite>>(name, sprite));
	}

	void SpriteGroup::RemoveSprite(const std::string& name)
	{
		_sprites.erase(name);
	}

	Ref<Sprite> SpriteGroup::GetSprite(const std::string& name)
	{
		return _sprites[name];
	}
}
