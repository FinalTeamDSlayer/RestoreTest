#include "..\Public\Layer.h"
#include "GameObject.h"
CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _tchar* pComponentTag)
{
	if (0 == m_GameObjects.size())
		return nullptr;

	for (auto& pGameObject : m_GameObjects)
		if (nullptr != pGameObject)
			return pGameObject->Get_Component(pComponentTag);

	return nullptr;
}

CGameObject* CLayer::Get_GameObject(_uint iIndex)
{
	if (0 == m_GameObjects.size())
		return nullptr;

	auto iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; i++)
		iter++;

	return (*iter);
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_double dTimeDelta)
{
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		if (nullptr != (*iter))
		{
			(*iter)->Tick(dTimeDelta);

			if (true == (*iter)->Get_Dead())
			{
				Safe_Release((*iter));
				iter = m_GameObjects.erase(iter);
				continue;
			}
			++iter;
		}
	}
}

void CLayer::LateTick(_double dTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->LateTick(dTimeDelta);
	}
}

void CLayer::Clear_Layer()
{
	Free();
}

CLayer* CLayer::Create()
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failet to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	
	m_GameObjects.clear();
}
