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


		return true;
	}

	/*static*/void Graphics::Tick()
	{
		s_display->ProcessEvent();
	}

	/*static*/void Graphics::Fini()
	{
		s_display->Fini();
	}

	/*static*/void Graphics::Draw()
	{
		s_display->SwapBuffers();
	}

}

