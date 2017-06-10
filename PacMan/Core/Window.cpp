#include "Window.h"
#include <cassert>

Window::Window(HINSTANCE hInstance, WNDPROC pfnWndProc, LPCSTR pWndClassName, LPCSTR pWndName,
			   LONG x, LONG y, LONG width, LONG height, DWORD style)
	: m_hInstance(hInstance)
	, m_WndClassName(pWndClassName)
	, m_hWnd(nullptr)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = pfnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = pWndClassName;
	wcex.hIconSm = nullptr;

	if (RegisterClassEx(&wcex))
	{
		RECT rect = {0, 0, width, height};
		if (!AdjustWindowRect(&rect, style, FALSE))
			assert(false);

		m_hWnd = CreateWindow(pWndClassName, pWndName, style, x, y,
			rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, m_hInstance, nullptr);
	}
}

Window::~Window()
{
	UnregisterClass(m_WndClassName.c_str(), m_hInstance);
}

HWND Window::GetHWND()
{
	return m_hWnd;
}

void Window::Show(int nCmdShow)
{
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);
}

RECT Window::GetClientRect() const
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	return rect;
}
