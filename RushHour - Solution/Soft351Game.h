#ifndef _SOFT351GAME_H
#define _SOFT351GAME_H

#include <Windows.h>
#include "Soft351Input.h"
#include "Soft351Renderer.h"

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"

class Soft351Game
{
public:
	Soft351Game(int screenWdith, int screenHeight);
	~Soft351Game();

	int Run();
	void Release();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool SetupWindow();

	int m_defScrnHeight, m_defScrnWidth;
	bool m_fullScreen;

	Soft351Input* m_pInput;
	Soft351Renderer* m_pRenderer;

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Soft351Game* ApplicationHandle = 0;

#endif