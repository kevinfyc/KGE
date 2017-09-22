//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "application.h"
#include "graphics/graphics.h"

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
	}

	bool IApplication::Init()
	{
		if (!Graphics::Init(_width, _height))
			return false;

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
	}

	void IApplication::OnDraw()
	{
		Graphics::Draw();
	}

} // end namespace kge
