//
//  graphics.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "graphics.h"

#include "display.h"

namespace kge
{
	Ref<Display> Graphics::s_display;

	/*static*/bool Graphics::Init(uint32 width, uint32 height)
	{
		s_display = RefMake<Display>();
		s_display->Init(width, height);

		s_display->ProcessEvent();

		return true;
	}

}

