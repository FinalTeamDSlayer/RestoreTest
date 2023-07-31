#include "pch.h"
#include "..\Public\Character_Tool.h"

#include "GameInstance.h"

CCharacter_Tool::CCharacter_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject(pDevice, pContext)
{
}

CCharacter_Tool::CCharacter_Tool(const CCharacter_Tool& rhs)
	: CLandObject(rhs)
{
}

HRESULT CCharacter_Tool::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_CharacterDesc, pArg, sizeof m_CharacterDesc);
	}

	if (FAILED(__super::Initialize(&m_CharacterDesc.WorldInfo)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{//초기 위치 설정
		m_pTransformCom->Scaling(m_WorldInfo.vScale);
		m_pTransformCom->Rotation(XMLoadFloat3(&m_WorldInfo.vAxis), m_WorldInfo.fDegree);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_WorldInfo.vPosition));
	}

	return S_OK;
}

void CCharacter_Tool::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);



	for (_uint i = 0; i < COLL_END; i++)
		m_pColliderCom[i]->Tick(m_pTransformCom->Get_WorldMatrix(), dTimeDelta);
}

void CCharacter_Tool::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < COLL_END; i++)
	{
		if (FAILED(m_pRendererCom->Add_DebugGroup(m_pColliderCom[i])))
			return;
	}
#endif // _DEBUG
}

HRESULT CCharacter_Tool::Render()
{
	return S_OK;
}

HRESULT CCharacter_Tool::Add_Components()
{
	/* for.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSG_BOX("Failed to Add_Com_Renderer : CCharacter_Tool");
		return E_FAIL;
	}
		
	/* for.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &m_CharacterDesc.TransformDesc)))
	{
		MSG_BOX("Failed to Add_Com_Transform : CCharacter_Tool");
		return E_FAIL;
	}

	/* for.Com_AABB */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_AABB"), (CComponent**)&m_pColliderCom[COLL_AABB], &m_CharacterDesc.ColliderDesc[COLL_AABB])))
	{
		MSG_BOX("Failed to Add_Com_AABB : CCharacter_Tool");
		return E_FAIL;
	}

	/* for.Com_OBB */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_OBB"), (CComponent**)&m_pColliderCom[COLL_OBB], &m_CharacterDesc.ColliderDesc[COLL_OBB])))
	{
		MSG_BOX("Failed to Add_Com_OBB : CCharacter_Tool");
		return E_FAIL;
	}

	/* for.Com_Sphere */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Sphere"), (CComponent**)&m_pColliderCom[COLL_SPHERE], &m_CharacterDesc.ColliderDesc[COLL_SPHERE])))
	{
		MSG_BOX("Failed to Add_Com_Sphere : CCharacter_Tool");
		return E_FAIL;
	}

	/* for.Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &m_CharacterDesc.NaviDesc)))
	{
		MSG_BOX("Failed to Add_Com_Navigation : CCharacter_Tool");
		return E_FAIL;
	}

	return S_OK;
}

void CCharacter_Tool::SetUp_Height()
{
	_fvector vPos = Compute_Height(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CCharacter_Tool::Tick_Collider(_double dTimeDelta)
{
	for (_uint i = 0; i < COLL_END; i++)
		m_pColliderCom[i]->Tick(m_pTransformCom->Get_WorldMatrix(), dTimeDelta);
}

void CCharacter_Tool::Free()
{
	__super::Free();

	for (_uint i = 0; i < COLL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
