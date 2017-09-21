//
//  display_GLES.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "display_GLES.h"

namespace kge
{
	bool DisplayGLES::Init(uint32 width, uint32 height)
	{
#if WIN32
		DisplayWin::Init(width, height);
#endif

		return true;
	}

	void DisplayGLES::Fini()
	{
#if WIN32
		DisplayWin::Fini();
#endif
	}
}

