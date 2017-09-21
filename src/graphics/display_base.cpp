//
//  display_base.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "display_base.h"

namespace kge
{
	bool DisplayBase::Init(uint32 width, uint32 height)
	{
		_width = width; _height = height;

		return true;
	}

	void DisplayBase::Fini()
	{
	
	}
}

