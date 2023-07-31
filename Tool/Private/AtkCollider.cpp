#include "pch.h"
#include "..\Public\AtkCollider.h"

#include "GameInstance.h"

CAtkCollider::CAtkCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CAtkCollider::CAtkCollider(const CAtkCollider& rhs)
	: CGameObject(rhs)
{
}

HRESULT CAtkCollider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAtkCollider::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CAtkCollider::Tick(_double dTimeDelta)
{
	if (true == m_isDead)
		return;

	__super::Tick(dTimeDelta);
}

void CAtkCollider::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);

#ifdef _DEBUG
	if (FAILED(m_pRendererCom->Add_DebugGroup(m_pColliderCom)))
		return;
#endif // _DEBUG
}

HRESULT CAtkCollider::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAtkCollider::Add_Components()
{
	/* for.Com_Sphere */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Sphere"), (CComponent**)&m_pColliderCom)))
	{
		MSG_BOX("Failed to Add_Com_Sphere : CAtkCollider");
		return E_FAIL;
	}

#ifdef _DEBUG
	/* for.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSG_BOX("Failed to Add_Com_Renderer : CAtkCollider");
		return E_FAIL;
	}
#endif // _DEBUG

	return S_OK;
}

CAtkCollider* CAtkCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAtkCollider* pInstance = new CAtkCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAtkCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAtkCollider::Clone(void* pArg)
{
	CAtkCollider* pInstance = new CAtkCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAtkCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAtkCollider::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);

#ifdef _DEBUG
	Safe_Release(m_pRendererCom);
#endif // _DEBUG
}
