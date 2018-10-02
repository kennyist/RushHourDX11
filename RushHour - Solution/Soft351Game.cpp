// Attempt of making the program more object oriented, Maybe work this in near the end of the work

#include "Soft351Game.h"

Soft351Game::Soft351Game(int screenWidth, int screenHeight)
{
	m_defScrnWidth = screenWidth;
	m_defScrnHeight = screenHeight;
	m_fullScreen = false;
	m_pInput = 0;
}


Soft351Game::~Soft351Game()
{
}

int Soft351Game::Run() {

	DXUTInit(true, true, NULL); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings(true, true);
	DXUTCreateWindow(L"Basic Thing3D");

	// Only require 10-level hardware, change to D3D_FEATURE_LEVEL_11_0 to require 11-class hardware
	// Switch to D3D_FEATURE_LEVEL_9_x for 10level9 hardware
	DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, 640, 480);

	DXUTMainLoop();

	return DXUTGetExitCode();
}

void Soft351Game::Release() {

}



LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	switch (uMessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, uMessage, wParam, lParam);
	}
	}
}

LRESULT CALLBACK Soft351Game::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KEYDOWN:
		{
			return 0;
		}

		case WM_KEYUP:
		{
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}