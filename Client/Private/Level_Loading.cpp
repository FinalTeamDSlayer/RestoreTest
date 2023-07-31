#include "pch.h"
#include "..\Public\Level_Loading.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_Village.h"
#include "Level_House.h"
#include "Level_Train.h"
#include "Level_FinalBoss.h"
#include "GameInstance.h"
#include "BackGround.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel(pDevice, pContext)
{

}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    m_pBackGround = CBackGround::Create(m_pDevice, m_pContext);
    if (nullptr == m_pBackGround)
        return E_FAIL;

    m_pBackGround->Initialize(nullptr);

    m_eNextLevelID = eNextLevelID;

    m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
    if (nullptr == m_pLoader)
        return E_FAIL;

    return S_OK;
}

void CLevel_Loading::Tick(_double dTimeDelta)
{
    __super::Tick(dTimeDelta);

    m_pBackGround->Set_TextureIndex(g_iLoadingTextureIndex % 3);
  
    m_pBackGround->Tick(dTimeDelta);

    m_pBackGround->LateTick(dTimeDelta);

    if (true == m_pLoader->Get_Finished())
    {
        if (GetKeyState(VK_SPACE) & 0x8000)
        {
            CLevel* pLevel = { nullptr };

            switch (m_eNextLevelID)
            {
            case LEVEL_LOGO:
                pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
                break;
            case LEVEL_GAMEPLAY:
                pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
                break;
            case LEVEL_VILLAGE:
                pLevel = CLevel_Village::Create(m_pDevice, m_pContext);
                break;
            case LEVEL_HOUSE:
                pLevel = CLevel_House::Create(m_pDevice, m_pContext);
                break;
            case LEVEL_TRAIN:
                pLevel = CLevel_Train::Create(m_pDevice, m_pContext);
                break;
            case LEVEL_FINALBOSS:
                pLevel = CLevel_FinalBoss::Create(m_pDevice, m_pContext);
                break;
            }

            if (nullptr == pLevel)
                return;

            g_iLoadingTextureIndex += 2;

            HRESULT hr = 0;

            CGameInstance* pGameInstance = CGameInstance::GetInstance();
            Safe_AddRef(pGameInstance);

            hr = pGameInstance->Open_Level(m_eNextLevelID, pLevel, true);
            Safe_Release(pGameInstance);

            if (FAILED(hr))
                return;

        }
    }
}

HRESULT CLevel_Loading::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
    CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);
    
    if (FAILED(pInstance->Initialize(eNextLevelID)))
    {
        MSG_BOX("Failed to Created : CLevel_Loading");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Loading::Free()
{
    __super::Free();

    Safe_Release(m_pBackGround);
    Safe_Release(m_pLoader);
}
