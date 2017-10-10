//
//  display.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "display_win.h"

#include "core/Application.h"

#include <string>
#include <assert.h>
#include <windowsx.h>

namespace kge
{
	DisplayWin::DisplayWin() :_hwnd(nullptr)
	{
	
	}

	DisplayWin::~DisplayWin()
	{
	}

	bool DisplayWin::Init(uint32 width, uint32 height)
	{
		if (!DisplayBase::Init(width, height))
			return false;

		if (!CreateView())
			return false;

		return true;
	}

	LRESULT CALLBACK win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				int width = lParam & 0xffff;
				int height = (lParam & 0xffff0000) >> 16;

				IApplication::GetInstance()->OnResize(width, height);
			}
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool DisplayWin::CreateView()
	{
		std::string app_name;
		IApplication::GetInstance()->GetName(app_name);
		assert(!app_name.empty());

		HINSTANCE inst = GetModuleHandle(nullptr);

		WNDCLASSEX win_class;
		ZeroMemory(&win_class, sizeof(win_class));

		win_class.cbSize = sizeof(WNDCLASSEX);
		win_class.style = CS_HREDRAW | CS_VREDRAW;
		win_class.lpfnWndProc = win_proc;
		win_class.cbClsExtra = 0;
		win_class.cbWndExtra = 0;
		win_class.hInstance = inst;
		win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		win_class.lpszMenuName = NULL;
		win_class.lpszClassName = app_name.c_str();
		win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

		if (!RegisterClassEx(&win_class))
		{
			return false;
		}

		DWORD style = WS_OVERLAPPEDWINDOW;
		DWORD style_ex = 0;

		RECT wr = { 0, 0, (LONG)_width, (LONG)_height };
		AdjustWindowRect(&wr, style, FALSE);

		int x = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2 + wr.left;
		int y = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2 + wr.top;

		HWND hwnd = CreateWindowEx(
			style_ex,			// window ex style
			app_name.c_str(),	// class name
			app_name.c_str(),	// app name
			style,			    // window style
			x, y,				// x, y
			wr.right - wr.left, // width
			wr.bottom - wr.top, // height
			NULL,				// handle to parent
			NULL,               // handle to menu
			inst,				// hInstance
			NULL);              // no extra parameters
		if (!hwnd)
		{
			return false;
		}

		ShowWindow(hwnd, SW_SHOW);

		_hwnd = hwnd;

#if KGE_GLES
		_hdc = GetDC(hwnd);

		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
			1,                     // version number
			PFD_DRAW_TO_WINDOW |   // support window
			PFD_SUPPORT_OPENGL |   // support OpenGL
			PFD_DOUBLEBUFFER,      // double buffered
			PFD_TYPE_RGBA,         // RGBA type
			24,                    // 24-bit color depth
			0, 0, 0, 0, 0, 0,      // color bits ignored
			0,                     // no alpha buffer
			0,                     // shift bit ignored
			0,                     // no accumulation buffer
			0, 0, 0, 0,            // accum bits ignored
			32,                    // 32-bit z-buffer
			0,                     // no stencil buffer
			0,                     // no auxiliary buffer
			PFD_MAIN_PLANE,        // main layer
			0,                     // reserved
			0, 0, 0                // layer masks ignored
		};

		int format_index = ChoosePixelFormat(_hdc, &pfd);
		SetPixelFormat(_hdc, format_index, &pfd);

		_context = wglCreateContext(_hdc);

		_shared_context = wglCreateContext(_hdc);
		wglShareLists(_context, _shared_context);

		wglMakeCurrent(_hdc, _context);

		//wglSwapIntervalEXT(0);
#endif

		return true;
	}

	void DisplayWin::CreateSharedContext()
	{
		wglMakeCurrent(_hdc, _shared_context);
	}

	void DisplayWin::DestroySharedContext()
	{
		wglMakeCurrent(nullptr, nullptr);
	}

	void DisplayWin::Fini()
	{
#if KGE_GLES
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(_shared_context);
		wglDeleteContext(_context);
		ReleaseDC(_hwnd, _hdc);
#endif
	}

	void DisplayWin::ProcessEvent()
	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
			{
				IApplication::GetInstance()->Quit();
			}
			else
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}
}

