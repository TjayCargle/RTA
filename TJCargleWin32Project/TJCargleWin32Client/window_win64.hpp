#pragma once

#ifndef NDEBUG
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <cassert>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")

namespace
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
}

namespace fsgd
{
	size_t window::_create(window* window, const properties& _props)
	{
		static TCHAR szWindowClass[] = _T("win32app");
		static TCHAR szTitle[] = _T("ENDApp");

		HINSTANCE hInst = GetModuleHandle(NULL);

		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_OWNDC;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(size_t) * 2 + sizeof(int);
		wcex.hInstance = hInst;
		wcex.hIcon = LoadIcon(hInst, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(0);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("ENDApp"),
				NULL);

			return 0;
		}

		HWND hWnd = CreateWindow(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX, 0, 0, _props.width, _props.height, 0, 0, hInst, 0);

		SetWindowLongPtr(hWnd, sizeof(size_t), (LONG_PTR)window);

		return (size_t)hWnd;
	}

	void window::show()
	{
		ShowWindow((HWND)native_handle, SW_SHOW);
	}

	void window::destroy()
	{
		if (native_handle != 0)
		{
			gfx.finalize();
			DestroyWindow((HWND)native_handle);
			native_handle = 0;
		}
	}

	bool window::pump_events()
	{
		MSG msg = { 0 };

		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				return false;
		}

		return true;
	}

	void window::set_cursor_pos(point p)
	{
		int x, y, w, h;
		get_window_pos(x, y, w, h);

		//p.y = h - p.y;

		POINT pos = { p.x, p.y };

		ClientToScreen((HWND)native_handle, &pos);
		SetCursorPos(pos.x, pos.y);
	}

	bool window::is_focus()const
	{
		return GetForegroundWindow() == (HWND)native_handle;
	}

	bool window::move_window(int x, int y, int w, int h)
	{
		return MoveWindow((HWND)native_handle, x, y, w, h, false) ? true : false;
	}

	void window::get_window_pos(int& x, int& y, int& w, int& h)
	{
		RECT rect;
		GetWindowRect((HWND)native_handle, &rect);

		x = rect.left;
		y = rect.top;
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}
}

namespace
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		fsgd::window* win = (fsgd::window*)GetWindowLongPtr(hWnd, sizeof(size_t));

		switch (message)
		{
		case WM_RBUTTONDOWN:
		{
			SetCapture(hWnd);

			win->cursor_props.rbutton_up = false;
			win->cursor_props.rbutton_down = true;
		}
		break;
		case WM_RBUTTONUP:
		{
			win->cursor_props.rbutton_down = false;
			win->cursor_props.rbutton_up = true;
			ReleaseCapture();
		}
		break;
		case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			win->cursor_props.lbutton_up = false;
			win->cursor_props.lbutton_down = true;
		}
		break;
		case WM_LBUTTONUP:
		{
			win->cursor_props.lbutton_down = false;
			win->cursor_props.lbutton_up = true;
			ReleaseCapture();
		}
		break;
		case WM_MOUSEMOVE:
		{
			fsgd::window::point pos{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

			int x, y, w, h;
			win->get_window_pos(x, y, w, h);

			//pos.y = h - pos.y;

			win->cursor_props.last_pos = win->cursor_props.pos;
			win->cursor_props.pos = pos;
		}
		break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case 'W':
				win->key_down[0] = true;
				break;
			case 'S':
				win->key_down[1] = true;
				break;
			case 'A':
				win->key_down[2] = true;
				break;
			case 'D':
				win->key_down[3] = true;
				break;
			default:
				break;
			}
		}
		break;
		case WM_KEYUP:
		{
			switch (wParam)
			{
			case 'W':
				win->key_down[0] = false;
				break;
			case 'S':
				win->key_down[1] = false;
				break;
			case 'A':
				win->key_down[2] = false;
				break;
			case 'D':
				win->key_down[3] = false;
				break;
			default:
				break;
			}
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}