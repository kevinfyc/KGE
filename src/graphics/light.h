//
//  light.h
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#ifndef __KGE_GRAPHICS_LIGHT_H__
#define __KGE_GRAPHICS_LIGHT_H__

#include "core/component.h"
#include "color.h"

namespace kge
{
	enum class LightType
	{
		Spot,
		Direction,
		Point
	};

	class Light : public Component
	{
		DECLARE_COM_CLASS(Light, Component);

	public:
		virtual ~Light();

	private:
		Light() :_type(LightType::Direction)
			, _color(1, 1, 1, 1)
			, _intensity(1)
		{

		}

	public:
		static WeakRef<Light> s_main;
		LightType _type;
		Color _color;
		float _intensity;
	};
}

#endif // __KGE_GRAPHICS_LIGHT_H__