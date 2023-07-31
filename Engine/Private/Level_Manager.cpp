#include "..\Public\Level_Manager.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::Reserve_Containers(_uint iNumLevels)
{
	m_pLoadedLevels.resize(iNumLevels);

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, CLevel* pNextLevel, _bool isStage, _bool isRelease)
{
	if (nullptr == pNextLevel)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear(m_iLevelIndex);

	Safe_Release(pGameInstance);

	if (true == isRelease)
	{
		Safe_Release(m_pCurrentLevel);
	}
		
	m_pCurrentLevel = pNextLevel;
	
	if (isStage)
	{
		m_pCurrentLevel->Set_IsStage(true);

		m_pLoadedLevels[iLevelIndex] = m_pCurrentLevel;
	}

	m_iLevelIndex = iLevelIndex;

	return S_OK;
}

HRESULT CLevel_Manager::Swap_Level(_uint iLevelIndex)
{
	if (nullptr == m_pLoadedLevels[iLevelIndex])
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear(m_iLevelIndex);

	Safe_Release(pGameInstance);

	m_pCurrentLevel = m_pLoadedLevels[iLevelIndex];

	if (FAILED(m_pCurrentLevel->Initialize()))
	{
		MSG_BOX("Failed to Swap_Level : CLevel_Manager");
	}

	m_iLevelIndex = iLevelIndex;

	return S_OK;
}

void CLevel_Manager::Tick_Level(_double dTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;
	
	m_pCurrentLevel->Tick(dTimeDelta);
}

void CLevel_Manager::Free()
{
	for (auto& pLevel : m_pLoadedLevels)
	{
		Safe_Release(pLevel);
	}
	m_pLoadedLevels.clear();

	Safe_Release(m_pCurrentLevel);
}
