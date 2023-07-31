#include "pch.h"
#include "..\Public\Level_Loading_Tool.h"

#include "Loader_Tool.h"
#include "Level_Logo_Tool.h"
#include "Level_Tool.h"
#include "Level_Village_Tool.h"
#include "Level_House_Tool.h"
#include "Level_Train_Tool.h"
#include "Level_FinalBoss_Tool.h"

#include "GameInstance.h"

CLevel_Loading_Tool::CLevel_Loading_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading_Tool::Initialize(LEVELID eNextLevelID)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevelID = eNextLevelID;

	m_pLoader = CLoader_Tool::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;


	return S_OK;
}

void CLevel_Loading_Tool::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (true == m_pLoader->Get_Finished())
	{   
		CLevel* pLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_TOOL:
			pLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_VILLAGE:
			pLevel = CLevel_Village_Tool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_HOUSE:
			pLevel = CLevel_House_Tool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_TRAIN:
			pLevel = CLevel_Train_Tool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_FINALBOSS:
			pLevel = CLevel_FinalBoss_Tool::Create(m_pDevice, m_pContext);
			break;
		}

		if (nullptr == pLevel)
			return;

		HRESULT hr = 0;

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		hr = pGameInstance->Open_Level(m_eNextLevelID, pLevel, true);

		Safe_Release(pGameInstance);

		if (FAILED(hr))
			return;
	}
}

HRESULT CLevel_Loading_Tool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading_Tool* CLevel_Loading_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
	CLevel_Loading_Tool* pInstance = new CLevel_Loading_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading_Tool::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
