#pragma once

#include <process.h>

namespace Client
{
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;
	static int				g_iLoadingTextureIndex = { 0 };

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO
				, LEVEL_GAMEPLAY, LEVEL_VILLAGE, LEVEL_HOUSE, LEVEL_TRAIN, LEVEL_FINALBOSS, LEVEL_END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

#include <io.h>

using namespace Client;