#include "stdafx.h"
#include "Application.h"
#include "../Utilities/ShortestPath.h"
#include "../Utilities/STLFile.h"
#include "../Utilities/GraphUtilities.h"

#include <string>

enum
{
	ID_LOAD_FILE_BUTTON = 1,
	ID_FIND_PATH_BUTTON,
	ID_FILE_PATH_EDIT,
	ID_START_VERTEX_STATIC,
	ID_END_VERTEX_STATIC,
	ID_FOUND_PATH_EDIT,
	ID_FILE_PATH_STATIC,
	ID_START_VERTEX_COMBOBOX,
	ID_END_VERTEX_COMBOBOX
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Application::instance().handleMessage(hWnd, msg, wParam, lParam);
}

Application& Application::instance()
{
	static Application instance;
	return instance;
}

int Application::launch(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	const WCHAR* wndClassName = L"Materialise Window Class";
	const WCHAR* wndTitle = L"Materialise Test";

	registerWndClass(hInstance, wndClassName);
	if (!initInstance(hInstance, wndClassName, wndTitle, nCmdShow))
		return 1;

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

ATOM Application::registerWndClass(HINSTANCE hInstance, const WCHAR* szWndClassName)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWndClassName;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

BOOL Application::initInstance(HINSTANCE hInstance, const WCHAR* szWndClassName, const WCHAR* szWndTitle, int nCmdShow)
{
	mInstance = hInstance;

	mWnd = CreateWindow(szWndClassName, szWndTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 600/*nWidth*/, 600/*nHeight*/, nullptr, nullptr, hInstance, nullptr);

	if (!mWnd)
		return FALSE;

	ShowWindow(mWnd, nCmdShow);
	UpdateWindow(mWnd);

	return TRUE;
}

LRESULT Application::handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			handleCreate(hWnd, wParam, lParam);
			break;
		}
		case WM_COMMAND:
		{
			handleCommand(hWnd, wParam, lParam);
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void Application::handleCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mFilePathStatic = CreateWindow(L"Static", L"File Path:",
		WS_CHILD | WS_VISIBLE,
		15, 30, 63, 20,
		hWnd, (HMENU)ID_FILE_PATH_STATIC,
		mInstance, NULL);

	mFilePathEdit = CreateWindow(L"Edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		80, 30, 400, 25, hWnd, (HMENU)ID_FILE_PATH_EDIT,
		mInstance, NULL);

	mLoadFileButton = CreateWindow(L"Button", L"Load File",
		WS_VISIBLE | WS_CHILD,
		490, 30, 80, 25, hWnd, (HMENU)ID_LOAD_FILE_BUTTON,
		mInstance, NULL);

	mStartVertexStatic = CreateWindow(L"Static", L"Start Vertex:",
		WS_CHILD | WS_VISIBLE,
		15, 70, 80, 20,
		hWnd, (HMENU)ID_START_VERTEX_STATIC,
		mInstance, NULL);

	mStartVertexCombo = CreateWindow(L"Combobox", NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_DISABLED,
		97, 70, 120, 110, hWnd, (HMENU)ID_START_VERTEX_COMBOBOX,
		mInstance, NULL);

	mEndVertexStatic = CreateWindow(L"Static", L"End Vertex:",
		WS_CHILD | WS_VISIBLE,
		230, 70, 75, 20,
		hWnd, (HMENU)ID_END_VERTEX_STATIC,
		mInstance, NULL);

	mEndVertexCombo = CreateWindow(L"Combobox", NULL,
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_DISABLED,
		307, 70, 120, 110, hWnd, (HMENU)ID_START_VERTEX_COMBOBOX,
		mInstance, NULL);

	mFindPathButton = CreateWindow(L"Button", L"Find Path",
		WS_VISIBLE | WS_CHILD | WS_DISABLED,
		490, 70, 80, 25,
		hWnd, (HMENU)ID_FIND_PATH_BUTTON,
		mInstance, NULL);

	mFoundPathEdit = CreateWindow(L"Edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
		10, 130, 563, 420,
		hWnd, (HMENU)ID_FOUND_PATH_EDIT,
		mInstance, NULL);
}

void Application::handleCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == ID_LOAD_FILE_BUTTON)
	{
		if (HIWORD(wParam) == BN_CLICKED)
			loadFile(hWnd);
		return;
	}
	if (LOWORD(wParam) == ID_FIND_PATH_BUTTON)
	{
		if (HIWORD(wParam) == BN_CLICKED)
			findPath(hWnd);
		return;
	}
	if (LOWORD(wParam) == ID_START_VERTEX_COMBOBOX)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			SendMessage(mStartVertexCombo, CBS_DROPDOWNLIST, (WPARAM)TRUE, 0);
		}
		else if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			if (SendMessage(mEndVertexCombo, CB_GETCURSEL, 0, 0) != CB_ERR)
				EnableWindow(mFindPathButton, TRUE);
		}
		return;
	}
	if (LOWORD(wParam) == ID_END_VERTEX_COMBOBOX)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			SendMessage(mEndVertexCombo, CBS_DROPDOWNLIST, (WPARAM)TRUE, 0);
		}
		else if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			if (SendMessage(mStartVertexCombo, CB_GETCURSEL, 0, 0) != CB_ERR)
				EnableWindow(mFindPathButton, TRUE);
		}
		return;
	}
}

void Application::loadFile(HWND hWnd)
{
	WCHAR fileNameBuffer[MAX_PATH];

	OPENFILENAME file;
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.lpstrFile = fileNameBuffer;
	file.lpstrFile[0] = '\0';
	file.hwndOwner = hWnd;
	file.nMaxFile = MAX_PATH;
	file.lpstrFilter = TEXT("All files(*.*)\0*.*\0");
	file.nFilterIndex = 1;
	file.lpstrInitialDir = NULL;
	file.lpstrFileTitle = NULL;
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&file) == FALSE)
		return;

	SendMessage(mStartVertexCombo, CB_RESETCONTENT, 0, NULL);
	SendMessage(mEndVertexCombo, CB_RESETCONTENT, 0, NULL);
	SetWindowText(mFilePathEdit, L"");
	SetWindowText(mFoundPathEdit, L"");

	auto loadResult = loadSTLFile(fileNameBuffer);
	if (loadResult.second)
	{
		auto optimizedData = OptimizeTriangleData(loadResult.first);

		mVertexBuffer = std::move(optimizedData.first);
		mGraph = createGraph(mVertexBuffer, optimizedData.second);

		for (const Vector& vertex : mVertexBuffer)
		{
			const auto vertexStr = toWString(vertex);
			SendMessage(mStartVertexCombo, CB_ADDSTRING, 0, (LPARAM)vertexStr.c_str());
			SendMessage(mEndVertexCombo, CB_ADDSTRING, 0, (LPARAM)vertexStr.c_str());
		}

		EnableWindow(mStartVertexCombo, TRUE);
		EnableWindow(mEndVertexCombo, TRUE);
		SetWindowText(mFilePathEdit, fileNameBuffer);
	}
	else
	{
		mGraph = nullptr;

		EnableWindow(mStartVertexCombo, FALSE);
		EnableWindow(mEndVertexCombo, FALSE);
		EnableWindow(mFindPathButton, FALSE);

		MessageBox(hWnd, L"Could not load the file", L"Error", MB_OK | MB_ICONERROR);
	}
}

void Application::findPath(HWND hWnd)
{
	auto startIndex = SendMessage(mStartVertexCombo, CB_GETCURSEL, 0, 0);
	auto endIndex = SendMessage(mEndVertexCombo, CB_GETCURSEL, 0, 0);
	assert(startIndex != CB_ERR);
	assert(endIndex != CB_ERR);

	auto result = findShortestPath(*mGraph, startIndex, endIndex);
	if (!result.second)
	{
		SetWindowText(mFoundPathEdit, L"");
		MessageBox(hWnd, L"Path not found", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	Path path = std::move(result.first);

	std::wstring pathString;
	pathString += L"Distance: " + std::to_wstring(path.mDistance);
	pathString += L"\r\n";

	for (VertexIndex index : path.mVertexIndices)
		pathString += toWString(mVertexBuffer[index]) + L"\r\n";

	SetWindowText(mFoundPathEdit, pathString.c_str());
}