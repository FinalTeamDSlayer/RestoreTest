#pragma once

#include "Tool_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;

END

BEGIN(Tool)

class CMainApp_Tool final : public CBase
{
private:
	CMainApp_Tool();
	virtual	~CMainApp_Tool() = default;

public:
	HRESULT		Initialize();
	void		Tick(_double dTimeDelta);
	HRESULT		Render();

private:
	CGameInstance*			m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CImGui_Manager_Tool*	m_pGUI = { nullptr };


private:
	CRenderer*				m_pRenderer = { nullptr };
	_float4					m_vTestBG = { 0.5f, 0.8f, 0.5f, 0.f };

#ifdef _DEBUG
private:
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_uint					m_iRenderCnt = { 0 };
	_double					m_TimeAcc = { 0.0 };

	_bool					m_isRenderFPS = { true };
#endif

private:
	HRESULT	Ready_Prototype_Component_For_Static();
	HRESULT SetUp_StartLevel(LEVELID eLevelID);

public:
	static CMainApp_Tool* Create();
	virtual void		Free();
};

END