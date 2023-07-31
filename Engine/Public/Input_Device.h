#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInput_Device 
	: public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_WHEEL, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_WHEEL, DIMS_END };

private:
	explicit CInput_Device();
	virtual ~CInput_Device();

public:
	_byte		Get_DIKeyState(_ubyte ubyKeyID) 
	{
		return m_byKeyState[ubyKeyID];
	}
	_bool		Get_DIKeyDown(_ubyte ubyKeyID);
	_bool		Get_DIKeyUp(_ubyte ubyKeyID);

	_byte		Get_DIMouseState(MOUSEKEYSTATE eMouseID)
	{
		return m_MouseState.rgbButtons[eMouseID];
	}
	_bool		Get_DIMouseDown(MOUSEKEYSTATE eMouseID);
	_bool		Get_DIMouseUp(MOUSEKEYSTATE eMouseID);
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveID)
	{
		return *(((_long*)&m_MouseState) + eMouseMoveID);
	}
	
	_bool		Get_AnyKeyPressing();	//어떤 키든 눌렀을 때 true 반환

public:	
	HRESULT			Ready_DInput(HINSTANCE hInst, HWND hWnd);
	void			Update_DInput();

private:
	LPDIRECTINPUT8				m_pInputSDK;

	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

private:
	_bool					m_isKeyDown[256];
	_byte					m_byKeyState[256];
	DIMOUSESTATE			m_MouseState;
	_bool					m_isMouseDown[DIM_END];
	_bool					m_isAnyKeyPressing = { false };

public:
	virtual void		Free();

};

END