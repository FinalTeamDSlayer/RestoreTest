#include "pch.h"
#include "..\Public\MapObject.h"

#include "GameInstance.h"

CMapObject::CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CMapObject::CMapObject(const CMapObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMapObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_MapObject_Info, pArg, sizeof m_MapObject_Info);

	wcscpy_s(m_PrototypeObjectTag, m_MapObject_Info.szMeshName);

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_MapObject_Info.vPos));
	m_pTransformCom->Rotation(m_MapObject_Info.vRotAngle);
	m_pTransformCom->Scaling(m_MapObject_Info.vScale);

	return S_OK;
}

void CMapObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CMapObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CMapObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_ShaderResource(i, m_pShaderCom, "g_DiffuseTexture", MESHMATERIALS::TextureType_DIFFUSE)))
			return E_FAIL;

		/*if (FAILED(m_pModelCom->Bind_ShaderResource(i, m_pShaderCom, "g_NormalTexture", MESHMATERIALS::TextureType_NORMALS)))
			return E_FAIL;*/

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CMapObject::Add_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	LEVELID eLevelID = LEVEL_END;

	switch (m_MapObject_Info.iSceneType)
	{
	case SCENE_STATIC:
		eLevelID = LEVEL_STATIC;
		break;
	case SCENE_GAMEPLAY:
		eLevelID = LEVEL_GAMEPLAY;
		break;
	case SCENE_VILLAGE:
		eLevelID = LEVEL_VILLAGE;
		break;
	case SCENE_HOUSE:
		eLevelID = LEVEL_HOUSE;
		break;
	case SCENE_TRAIN:
		eLevelID = LEVEL_TRAIN;
		break;
	case SCENE_FINALBOSS:
		eLevelID = LEVEL_FINALBOSS;
		break;
	}

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(eLevelID, m_PrototypeObjectTag,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapObject::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_float4x4 ViewMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	_float4x4 ProjMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	_float4 vCamPos = pGameInstance->Get_CameraPosition();

	if (FAILED(m_pShaderCom->SetUp_Vector("g_vCamPosition", &vCamPos)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

void CMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
