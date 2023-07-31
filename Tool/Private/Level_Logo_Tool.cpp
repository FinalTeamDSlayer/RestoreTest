#include "pch.h"
#include "..\Public\Level_Logo_Tool.h"
#include "GameInstance.h"
#include "Level_Loading_Tool.h"

CLevel_Logo_Tool::CLevel_Logo_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Logo_Tool::Initialize()
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
        return E_FAIL;

    return S_OK;
}

void CLevel_Logo_Tool::Tick(_double dTimeDelta)
{
    __super::Tick(dTimeDelta);

    SetWindowText(g_hWnd, TEXT("Logo"));

    if (GetKeyState(VK_RETURN) & 0x8000)
    {
        HRESULT hr = 0;

        CGameInstance* pGameInstance = CGameInstance::GetInstance();
        Safe_AddRef(pGameInstance);

        hr = pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading_Tool::Create(m_pDevice, m_pContext, LEVEL_TOOL));

        Safe_Release(pGameInstance);

        if (FAILED(hr))
            return;
    }
}

HRESULT CLevel_Logo_Tool::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Logo_Tool::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    /* For.Sky */
    /*if (FAILED(pGameInstance->Add_GameObject(LEVEL_FINALBOSS, pLayerTag,
        TEXT("Prototype_GameObject_Sky"))))
    {
        MSG_BOX("Failed to Add_GameObject : Sky");
        return E_FAIL;
    }*/

    Safe_Release(pGameInstance);

    return S_OK;
}

CLevel_Logo_Tool* CLevel_Logo_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLevel_Logo_Tool* pInstance = new CLevel_Logo_Tool(pDevice, pContext);
    
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CLevel_Logo_Tool");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Logo_Tool::Free()
{
    __super::Free();
}
