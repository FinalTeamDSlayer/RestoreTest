#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CLoader final : public CBase
{
private:
    CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLoader() = default;
public:
    LEVELID             Get_LevelID() const { return m_eLevelID; }

    CRITICAL_SECTION*   Get_CS() { return &m_CS; }

    const _tchar*       Get_LoadingText() const { return m_szLoadingText; }

    _bool               Get_Finished() const { return m_isFinished; }

public:
    HRESULT Initialize(LEVELID eLevelID);

    HRESULT LoadingForLogo();
    HRESULT LoadingForGamePlay();

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
    static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eLevelID);
    virtual void Free() override;
    
};

END