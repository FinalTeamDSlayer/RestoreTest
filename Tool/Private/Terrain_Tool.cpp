#include "pch.h"
#include "..\Public\Terrain_Tool.h"

#include "GameInstance.h"

CTerrain_Tool::CTerrain_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTerrain_Tool::CTerrain_Tool(const CTerrain_Tool& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_Tool::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Tool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr != pArg)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4((_float4*)pArg));

	return S_OK;
}

void CTerrain_Tool::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);
}

void CTerrain_Tool::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CTerrain_Tool::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_ShaderResource(i, m_pShaderCom, "g_DiffuseTexture", MESHMATERIALS::TextureType_DIFFUSE)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	/*if (FAILED(m_pModelCom->Bind_ShaderResource(0, m_pShaderCom, "g_DiffuseTexture", aiTextureType_BASE_COLOR)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pModelCom->Render(0);*/

	return S_OK;
}

HRESULT CTerrain_Tool::Add_Components()
{
	/* for.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Model_Terrain"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSG_BOX("Failed to Add_Com_Model : CTerrain_Tool");
		return E_FAIL;
	}

	/* for.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSG_BOX("Failed to Add_Com_Shader : CTerrain_Tool");
		return E_FAIL;
	}

	/* for.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSG_BOX("Failed to Add_Com_Renderer : CTerrain_Tool");
		return E_FAIL;
	}

	/* for.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
	{
		MSG_BOX("Failed to Add_Com_Transform : CTerrain_Tool");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain_Tool::SetUp_ShaderResources()
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

CTerrain_Tool* CTerrain_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_Tool* pInstance = new CTerrain_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain_Tool::Clone(void* pArg)
{
	CTerrain_Tool* pInstance = new CTerrain_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Tool::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
