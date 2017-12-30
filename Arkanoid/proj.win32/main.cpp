#include "main.h"
#include "cocos2d.h"
#include "AppDelegate.h"

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPTSTR /*lpCmdLine*/,
	int /*nCmdShow*/)
{
    AppDelegate appDelegate;
    return Application::getInstance()->run();
}
