//
//  sprite_group.h
//  libkge
//
//  Created by kevin on 17-10-10.
//
//
// 

#ifndef __KGE_SPRITE_GROUP_H__
#define __KGE_SPRITE_GROUP_H__


#include "sprite.h"

namespace kge
{
	class SpriteGroup: public Object
	{
	public:
		static Ref<SpriteGroup> Create();

		void AddSprite(const std::string& name, const Ref<Sprite>& sprite);
		void RemoveSprite(const std::string& name);
		Ref<Sprite> GetSprite(const std::string& name);
		void SetTexture(const Ref<Texture>& texture) { _texture = texture; }
		const Ref<Texture>& GetTexture() const { return _texture; }

	private:
		SpriteGroup();

		std::map<std::string, Ref<Sprite>> _sprites;
		Ref<Texture> _texture;
	};
}

#endif // __KGE_SPRITE_GROUP_H__
