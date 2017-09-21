//
//  main_def.h
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#ifndef __KGE_MAIN_DEF_H__
#define __KGE_MAIN_DEF_H__

#include "Application.h"
#if WIN32
#include <Windows.h>
#define KGE_MAIN(app_class) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) \
{ \
	app_class app; \
	app.Run(); \
    return 0; \
}
#endif


#endif // __KGE_MAIN_DEF_H__
