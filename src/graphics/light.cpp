//
//  light.cpp
//  libkge
//
//  Created by kevin on 17-10-16.
//
//

#include "light.h"
#include <assert.h>

namespace kge
{
	DEFINE_COM_CLASS(Light);

	WeakRef<Light> Light::s_main;

	Light::~Light()
	{

	}

	void Light::DeepCopy(const Ref<Object>& source)
	{
		assert(!"unimplemment");
	}
}

