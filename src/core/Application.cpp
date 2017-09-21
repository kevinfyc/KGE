//
//  Application.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "application.h"
#include <assert.h>


namespace kge
{
	bool IApplication::Init()
	{
		return true;
	}

	void IApplication::Fini()
	{
	
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
		assert(!Init());

		Tick();

		Fini();

        return 0;
    }
    
	void IApplication::OnUpdate()
	{

	}

	void IApplication::OnDraw()
	{

	}

} // end namespace ora
