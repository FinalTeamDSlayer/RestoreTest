#include "pch.h"
#include "..\Public\AtkCollManager.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CAtkCollManager)

CAtkCollManager::CAtkCollManager()
{
}

void CAtkCollManager::CreateAtkColl(const _tchar* pLayerTag, CAtkCollider::ATKCOLLDESC* pAtkCollDesc, CGameInstance* pGameInstance)
{
	CAtkCollider::ATKCOLLDESC		AtkCollDesc;
	ZeroMemory(&AtkCollDesc, sizeof AtkCollDesc);

	AtkCollDesc = *pAtkCollDesc;
	
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_AtkCollider"), &AtkCollDesc, true)))
	{
		MSG_BOX("Failed to CreateAtkColl");
		return;
	}
}


void CAtkCollManager::Collect_Collider(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;

	m_AtkCollPool.push_back(pGameObject);
}

void CAtkCollManager::Reuse_Collider(const _tchar* pLayerTag, CAtkCollider::ATKCOLLDESC* pAtkCollDesc)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pAtkCollider = nullptr;

	if (m_AtkCollPool.empty())
	{
		CreateAtkColl(pLayerTag, pAtkCollDesc, pGameInstance);
		++m_iCount;

	}
	else
	{
		pAtkCollider = m_AtkCollPool.front();
		m_AtkCollPool.pop_front();

		if (nullptr == pAtkCollider)
			return;

		dynamic_cast<CAtkCollider*>(pAtkCollider)->Reset_AtkCollider(pAtkCollDesc);

		pGameInstance->Add_GameObject(LEVEL_STATIC, pLayerTag, pAtkCollider, true);
	}

	Safe_Release(pGameInstance);
} 

void CAtkCollManager::Free()
{
	for (auto& pAtkColl : m_AtkCollPool)
		Safe_Release(pAtkColl);

	m_AtkCollPool.clear();
}
