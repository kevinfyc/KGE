//
//  graphics.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "graphics.h"

#include "display.h"

#include "camera.h"
#include "core/Application.h"

namespace kge
{
	Ref<Display> Graphics::s_display;
	int Graphics::draw_call = 0;

	/*static*/bool Graphics::Init(uint32 width, uint32 height)
	{
		s_display = RefMake<Display>();
		s_display->Init(width, height);

		IApplication::GetInstance()->RunTaskInPreLoop(TaskLoop::TaskNode([] {
			s_display->ProcessEvent();
		}, false));

		return true;
	}

	/*static*/void Graphics::Fini()
	{
		s_display->Fini();
	}

	/*static*/void Graphics::Draw()
	{
		Graphics::draw_call = 0;

		Camera::PrepareAll();
		Camera::RenderAll();

		s_display->SwapBuffers();
	}

}

