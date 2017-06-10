#include "PacManGame.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	PacManGame pacManGame(hInstance);
	return pacManGame.Start(nCmdShow);
}