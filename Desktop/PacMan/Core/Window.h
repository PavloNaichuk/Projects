#pragma once

#define NOMINMAX
#include <windows.h>
#include <string>

class Window
{
public:
	Window(HINSTANCE hInstance, WNDPROC pfnWndProc,
		LPCSTR pWndClassName, LPCSTR pWndName,
		LONG x = CW_USEDEFAULT, LONG y = CW_USEDEFAULT,
		LONG width = CW_USEDEFAULT, LONG height = CW_USEDEFAULT,
		DWORD style = WS_OVERLAPPEDWINDOW);

	~Window();

	HWND GetHWND();
	void Show(int nCmdShow);
	RECT GetClientRect() const;

private:
	HINSTANCE m_hInstance;
	std::string m_WndClassName;
	HWND m_hWnd;
};
