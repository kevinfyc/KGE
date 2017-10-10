//
//  display.cpp
//  libkge
//
//  Created by kevin on 17-9-21.
//
//

#include "display_win.h"

#include "core/Application.h"
#include "core/input.h"

#include <string>
#include <assert.h>
#include <windowsx.h>

#include "graphics.h"
#include "display.h"
#include "util/time.h"

extern std::vector<kge::Touch> g_input_touches;
extern std::list<kge::Touch> g_input_touch_buffer;
extern bool g_key_down[(kge::uint32)kge::KeyCode::COUNT];
extern bool g_key[(kge::uint32)kge::KeyCode::COUNT];
extern bool g_key_up[(kge::uint32)kge::KeyCode::COUNT];
extern bool g_key_held[(kge::uint32)kge::KeyCode::COUNT];
extern bool g_mouse_button_down[3];
extern bool g_mouse_button_up[3];
extern kge::Vector3 g_mouse_position;
extern bool g_mouse_button_held[3];

namespace kge
{
	static bool g_input_down = false;

	static uint32 get_key_code(int wParam)
	{
		uint32 key = (uint32)KeyCode::None;

		if (wParam >= 48 && wParam < 48 + 10)
			key = (uint32)KeyCode::Alpha0 + wParam - 48;
		else if (wParam >= 96 && wParam < 96 + 10)
			key = (uint32)KeyCode::Keypad0 + wParam - 96;
		else if (wParam >= 65 && wParam < 65 + 'z' - 'a')
			key = (uint32)KeyCode::A + wParam - 65;
		else if (wParam == VK_BACK)
			key = (uint32)KeyCode::Backspace;
		else if (wParam == VK_SPACE)
			key = (uint32)KeyCode::Space;
		else if (wParam == VK_ESCAPE)
			key = (uint32)KeyCode::Escape;
		else if (wParam == VK_RETURN)
			key = (uint32)KeyCode::Return;

		return key;
	}

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
				uint32 width = lParam & 0xffff;
				uint32 height = (lParam & 0xffff0000) >> 16;

				IApplication::GetInstance()->OnResize(width, height);
			}
			break;

		case WM_KEYDOWN:
		{
			uint32 key = get_key_code((uint32)wParam);

			if (key > 0)
			{
				if (!g_key_held[key])
				{
					g_key_down[key] = true;
					g_key_held[key] = true;
					g_key[key] = true;
				}
			}
		}
		break;

		case WM_KEYUP:
		{
			uint32 key = get_key_code((uint32)wParam);

			if (key > 0)
			{
				g_key_up[key] = true;
				g_key_held[key] = false;
				g_key[key] = false;
			}
		}
		break;

		case WM_SYSCHAR:
			if (wParam == VK_RETURN)
			{
				//Alt + Enter
				//switch_full_screen(hWnd);
			}
			break;

		case WM_LBUTTONDOWN:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			if (!g_input_down)
			{
				Touch t;
				t.deltaPosition = Vector2(0, 0);
				t.deltaTime = 0;
				t.fingerID = 0;
				t.phase = TouchPhase::Began;
				t.position = Vector2((float)x, (float)Graphics::GetDisplay()->GetHeight() - y - 1);
				t.tapCount = 1;
				t.time = Time::GetTime();

				if (!g_input_touches.empty())
					g_input_touch_buffer.push_back(t);
				else
					g_input_touches.push_back(t);

				g_input_down = true;
			}

			g_mouse_button_down[0] = true;
			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
			g_mouse_button_held[0] = true;
		}
		break;

		case WM_RBUTTONDOWN:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			g_mouse_button_down[1] = true;
			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
			g_mouse_button_held[1] = true;
		}
		break;

		case WM_MBUTTONDOWN:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			g_mouse_button_down[2] = true;
			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
			g_mouse_button_held[2] = true;
		}
		break;

		case WM_MOUSEMOVE:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			if (g_input_down)
			{
				Touch t;
				t.deltaPosition = Vector2(0, 0);
				t.deltaTime = 0;
				t.fingerID = 0;
				t.phase = TouchPhase::Moved;
				t.position = Vector2((float)x, (float)Graphics::GetDisplay()->GetHeight() - y - 1);
				t.tapCount = 1;
				t.time = Time::GetTime();

				if (!g_input_touches.empty())
				{
					if (g_input_touch_buffer.empty())
					{
						g_input_touch_buffer.push_back(t);
					}
					else
					{
						if (g_input_touch_buffer.back().phase == TouchPhase::Moved)
							g_input_touch_buffer.back() = t;
						else
							g_input_touch_buffer.push_back(t);
					}
				}
				else
				{
					g_input_touches.push_back(t);
				}
			}

			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
		}
		break;

		case WM_LBUTTONUP:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			if (g_input_down)
			{
				Touch t;
				t.deltaPosition = Vector2(0, 0);
				t.deltaTime = 0;
				t.fingerID = 0;
				t.phase = TouchPhase::Ended;
				t.position = Vector2((float)x, (float)Graphics::GetDisplay()->GetHeight() - y - 1);
				t.tapCount = 1;
				t.time = Time::GetTime();

				if (!g_input_touches.empty())
					g_input_touch_buffer.push_back(t);
				else
					g_input_touches.push_back(t);

				g_input_down = false;
			}

			g_mouse_button_up[0] = true;
			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
			g_mouse_button_held[0] = false;
		}
		break;

		case WM_RBUTTONUP:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			g_mouse_button_up[1] = true;
			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
			g_mouse_button_held[1] = false;
		}
		break;

		case WM_MBUTTONUP:
		{
			uint32 x = GET_X_LPARAM(lParam);
			uint32 y = GET_Y_LPARAM(lParam);

			g_mouse_button_up[2] = true;
			g_mouse_position.x = (float)x;
			g_mouse_position.y = (float)Graphics::GetDisplay()->GetHeight() - y - 1;
			g_mouse_button_held[2] = false;
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

