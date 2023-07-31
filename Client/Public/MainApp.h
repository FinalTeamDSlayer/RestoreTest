#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;

END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual	~CMainApp() = default;

public:
	HRESULT		Initialize();
	void		Tick(_double dTimeDelta);
	HRESULT		Render();

private:
	CGameInstance*			m_pGameInstance = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	CRenderer*				m_pRenderer = { nullptr };
	_float4					m_vTestBG = { 0.5f, 0.8f, 0.5f, 1.0f };

#ifdef _DEBUG
private:
	void Key_Input(_double dTimeDelta);

private:
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_uint					m_iRenderCnt = { 0 };
	_double					m_TimeAcc = { 0.0 };
	

	_bool					m_isRenderFPS = { true };
	_bool                   m_isRenderDebugInfo = { true };
#endif

private:
	HRESULT	Ready_Prototype_Component_For_Static();
	HRESULT SetUp_StartLevel(LEVELID eLevelID);

public:
	static CMainApp*	Create();
	virtual void		Free();
};

END