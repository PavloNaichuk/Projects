#pragma once

#include "../Utilities/Graph.h"
#include "../Utilities/OptimizeTriangleData.h"
#include <memory>

class Application 
{
public:
	Application(const Application&) = delete;
	Application& operator= (const Application&) = delete;
	
	static Application& instance();

	int launch(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
	LRESULT handleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	Application() = default;
	~Application() = default;

	ATOM registerWndClass(HINSTANCE hInstance, const WCHAR* szWndClassName);
	BOOL initInstance(HINSTANCE hInstance, const WCHAR* szWndClassName, const WCHAR* szWndTitle, int nCmdShow);
	void handleCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void handleCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void loadFile(HWND hWnd);
	void findPath(HWND hWnd);

private:
	HINSTANCE mInstance;
	HWND mWnd;
	HWND mFilePathStatic;
	HWND mStartVertexCombo;
	HWND mEndVertexCombo;
	HWND mFilePathEdit;
	HWND mFoundPathEdit;
	HWND mLoadFileButton;
	HWND mStartVertexStatic;
	HWND mEndVertexStatic;
	HWND mFindPathButton;

	VertexBuffer mVertexBuffer;
	std::unique_ptr<Graph> mGraph;
};
