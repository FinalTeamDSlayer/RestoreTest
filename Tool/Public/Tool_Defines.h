#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

//#include "Engine_Defines.h"
//#include "Engine_Struct.h"
#include <process.h>

namespace Tool
{
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;

	static const unsigned int g_iGUI_LeftX = 12;
	static const unsigned int g_iGUI_LeftY = 788;
	
	static const unsigned int g_iGUI_RightX = 1304;
	static const unsigned int g_iGUI_RightY = 10;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO
				, LEVEL_TOOL, LEVEL_VILLAGE, LEVEL_HOUSE, LEVEL_TRAIN, LEVEL_FINALBOSS, LEVEL_END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

#include "ImGui_Manager_Tool.h"
#include "ImGui_Animation_Tool.h"

using namespace Tool;