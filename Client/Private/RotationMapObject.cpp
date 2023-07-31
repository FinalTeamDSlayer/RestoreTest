#include "pch.h"
#include "..\Public\RotationMapObject.h"

#include "GameInstance.h"

CRotationMapObject::CRotationMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMapObject(pDevice, pContext)
{
}

CRotationMapObject::CRotationMapObject(const CRotationMapObject& rhs)
	: CMapObject(rhs)
{
}

HRESULT CRotationMapObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRotationMapObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_MapObject_Info.iMapObjectType = MAPOBJECT_ROTATION;

	return S_OK;
}

void CRotationMapObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CRotationMapObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CRotationMapObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CRotationMapObject* CRotationMapObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRotationMapObject* pInstance = new CRotationMapObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRotationMapObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRotationMapObject::Clone(void* pArg)
{
	CRotationMapObject* pInstance = new CRotationMapObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRotationMapObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRotationMapObject::Free()
{
	__super::Free();
}
