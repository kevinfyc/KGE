//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "application.h"
#include "graphics/graphics.h"

#include "util/log.h"

#include <assert.h>

namespace kge
{
	IApplication* IApplication::_instance;
	/*static*/ IApplication* IApplication::GetInstance()
	{
		return _instance;
	}

	IApplication::IApplication():_width(1280)
		,_height(720)
	{
		_instance = this;
	}

	IApplication::~IApplication()
	{
		_instance = nullptr;
		World::Fini();
	}

	bool IApplication::Init()
	{
		if (!Graphics::Init(_width, _height))
		{
			KGE_LOG_ERROR("Graphics Init failed");
			return false;
		}

		if (!World::Init())
		{
			KGE_LOG_ERROR("World Init failed");
			return false;
		}

		Start();

		return true;
	}

	void IApplication::Fini()
	{
		Graphics::Fini();
	}
    
	void IApplication::Tick()
	{
		while (!_exit)
		{
			OnUpdate();
			OnDraw();
		}
	}

    int IApplication::Run()
    {
		assert(Init());

		Tick();

		Fini();

        return 0;
    }
    
	void IApplication::OnUpdate()
	{
		Graphics::Tick();

		Update();
		World::Tick();
	}

	void IApplication::OnDraw()
	{
		Graphics::Draw();
	}

} // end namespace kge
