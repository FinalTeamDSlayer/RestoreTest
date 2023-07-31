#include "pch.h"
#include "..\Public\ColliderManager.h"

#include "GameInstance.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CColliderManager)

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

	if (FAILED(Check_PlayerAtkToMonster(iLevelIndex, dTimeDelta)))
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

				if (true == pMonsterCollider->Get_Coll())
				{
					CTransform* pMonsterTransform = dynamic_cast<CTransform*>(pMonster->Find_Component(TEXT("Com_Transform")));
					CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pGameInstance->Get_Component(iLevelIndex, TEXT("Layer_Player"), (TEXT("Com_Transform"))));

					_vector vDir = pMonsterTransform->Get_State(CTransform::STATE_POSITION) - pPlayerTransform->Get_State(CTransform::STATE_POSITION);

					pMonsterTransform->Go_Dir(dTimeDelta * 0.5, vDir);
					pPlayerTransform->Go_Dir(dTimeDelta * 0.5, vDir * -1);
				}
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

HRESULT CColliderManager::Check_PlayerAtkToMonster(_uint iLevelIndex, _double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*>* pPlayerAtkColls = pGameInstance->Get_GameObjects(LEVEL_STATIC, TEXT("Layer_PlayerAtk"));

	list<CGameObject*>* pMonsters = pGameInstance->Get_GameObjects(iLevelIndex, TEXT("Layer_Monster"));

	if (nullptr != pPlayerAtkColls && nullptr != pMonsters)
	{
		for (auto& pPlayerAtkColl : (*pPlayerAtkColls))
		{
			if (nullptr != pPlayerAtkColl)
			{
				for (auto& pMonster : (*pMonsters))
				{
					if (nullptr != pMonster)
					{
						CCollider* pAtkCollider = dynamic_cast<CCollider*>(pPlayerAtkColl->Find_Component(TEXT("Com_Sphere")));
						CCollider* pMonsterCollider = dynamic_cast<CCollider*>(pMonster->Find_Component(TEXT("Com_Sphere")));

						pAtkCollider->Intersect(pMonsterCollider);

						if (true == pMonsterCollider->Get_Coll())
						{
							//몬스터를 넉백
							CTransform* pMonsterTransform = dynamic_cast<CTransform*>(pMonster->Find_Component(TEXT("Com_Transform")));
							CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pGameInstance->Get_Component(iLevelIndex, TEXT("Layer_Player"), (TEXT("Com_Transform"))));

							_vector vDir = pMonsterTransform->Get_State(CTransform::STATE_POSITION) - pPlayerTransform->Get_State(CTransform::STATE_POSITION);

							pMonsterTransform->Go_Dir(dTimeDelta * 0.5, vDir);
							//pPlayerTransform->Go_Dir(dTimeDelta * 0.5, vDir * -1);
						}
					}
				}
			}
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CColliderManager::Check_MonsterToMonster(_uint iLevelIndex, _double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	list<CGameObject*>* pMonsters = pGameInstance->Get_GameObjects(iLevelIndex, TEXT("Layer_Monster"));

	if (nullptr != pMonsters)
	{
		_uint iSourIndex = { 0 };
		for (auto& pSourMonster : (*pMonsters))
		{
			if (nullptr != pSourMonster)
			{
				_uint iDestIndex = { 0 };
				for (auto& pDestMonster : (*pMonsters))
				{
					if (nullptr != pDestMonster)
					{
						if (iSourIndex == iDestIndex)
						{
							iDestIndex++;
							continue;
						}

						CCollider* pSourCollider = dynamic_cast<CCollider*>(pSourMonster->Find_Component(TEXT("Com_Sphere")));
						CCollider* pDestCollider = dynamic_cast<CCollider*>(pDestMonster->Find_Component(TEXT("Com_Sphere")));
						pSourCollider->Intersect(pDestCollider);

						if (true == pDestCollider->Get_Coll())
						{
							CTransform* pSourTransform = dynamic_cast<CTransform*>(pSourMonster->Find_Component(TEXT("Com_Transform")));
							CTransform* pDestTransform = dynamic_cast<CTransform*>(pDestMonster->Find_Component(TEXT("Com_Transform")));
							
							_vector vDir = pDestTransform->Get_State(CTransform::STATE_POSITION) - pSourTransform->Get_State(CTransform::STATE_POSITION);

							if (0.f == Convert::GetLength(vDir))
							{
								vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
							}

							pDestTransform->Go_Dir(dTimeDelta * 0.5, vDir);
							pSourTransform->Go_Dir(dTimeDelta * 0.5, vDir * -1);
						}
						iDestIndex++;
					}
				}
				iSourIndex++;
			}
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

void CColliderManager::Free()
{
}
