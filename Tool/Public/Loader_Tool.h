#pragma once

#include"Tool_Defines.h"
#include "Base.h"

BEGIN(Tool)

class CLoader_Tool final : public CBase
{
private:
	CLoader_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader_Tool() = default;

public:
    LEVELID             Get_LevelID() const { return m_eLevelID; }
    CRITICAL_SECTION*    Get_CS() { return  &m_CS; }
    const _tchar*       Get_LoadingText() const { return m_szLoadingText; }
    _bool               Get_Finished() const { return m_isFinished; }

public:
    HRESULT Initialize(LEVELID eLevelID);

    HRESULT LoadingForAllStage();

    HRESULT LoadingForLogo(_bool isLoad);
    HRESULT LoadingForTool(_bool isLoad);
    HRESULT LoadingForVillage(_bool isLoad);
    HRESULT LoadingForHouse(_bool isLoad);
    HRESULT LoadingForTrain(_bool isLoad);
    HRESULT LoadingForFinalBoss(_bool isLoad);

private:
    HRESULT Load_MapObjectModel_AllStage(CGameInstance* pGameInstance);
    HRESULT Load_MapObjectModel_House();
    HRESULT Load_MapObjectModel_FinalBoss();

private:
    ID3D11Device*           m_pDevice = { nullptr };
    ID3D11DeviceContext*    m_pContext = { nullptr };

private:
    HANDLE              m_hThread = { 0 };
    LEVELID             m_eLevelID = { LEVEL_END };
    CRITICAL_SECTION    m_CS;
    _tchar              m_szLoadingText[MAX_PATH] = TEXT("");
    _bool               m_isFinished = { false };

public:
	static CLoader_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eLevelID);
	virtual void Free() override;
};

END