#include "pch.h"
#include "..\Public\LevelObject.h"

#include "GameInstance.h"

CLevelObject::CLevelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CLevelObject::CLevelObject(const CLevelObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CLevelObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CLevelObject::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CLevelObject::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);
}

HRESULT CLevelObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelObject::Add_Components()
{
	/* for.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSG_BOX("Failed to Add_Com_Renderer : CPlayer_Tool");
		return E_FAIL;
	}

	/* for.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
	{
		MSG_BOX("Failed to Add_Com_Transform : CPlayer_Tool");
		return E_FAIL;
	}
	return S_OK;
}

void CLevelObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
