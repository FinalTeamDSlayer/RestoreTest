#include "pch.h"
#include "..\Public\ColliderManager.h"

#include "GameInstance.h"
#include "Player.h"

CColliderManager::CColliderManager()
{
}

HRESULT CColliderManager::Inititalize()
{
	return S_OK;
}

HRESULT CColliderManager::Check_Collider(_uint iLevelIndex, _double dTimeDelta)
{
	if (FAILED(Check_PlayerToMonster(iLevelIndex, dTimeDelta)))
	{
		MSG_BOX("Failed to Check_PlayerToMonster");
		return E_FAIL;
	}

	if (FAILED(Check_MonsterToMonster(iLevelIndex, dTimeDelta)))
	{
		MSG_BOX("Failed to Check_PlayerToMonster");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CColliderManager::Check_PlayerToMonster(_uint iLevelIndex, _double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(pGameInstance->Get_Component(iLevelIndex, TEXT("Layer_Player"), TEXT("Com_Sphere")));

	list<CGameObject*>* pMonsters = pGameInstance->Get_GameObjects(iLevelIndex, TEXT("Layer_Monster"));

	_int iCollCount = { 0 };

	if (nullptr != pPlayerCollider && nullptr != pMonsters)
	{
		for (auto& pMonster : (*pMonsters))
		{
			if (nullptr != pMonster)
			{
				CCollider* pMonsterCollider = dynamic_cast<CCollider*>(pMonster->Find_Component(TEXT("Com_Sphere")));
				pPlayerCollider->Intersect(pMonsterCollider);

				if (true == pPlayerCollider->Get_Coll())
					iCollCount++;
			}
		}
	}

	if (0 < iCollCount)
		pPlayerCollider->Set_Coll(true);
	else
		pPlayerCollider->Set_Coll(false);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CColliderManager::Check_MonsterToMonster(_uint iLevelIndex, _double dTimeDelta)
{
	return S_OK;
}

void CColliderManager::Free()
{
}
