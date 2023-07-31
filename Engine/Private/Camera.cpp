#include "..\Public\Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);

	if (nullptr == m_pTransformCom)
		return E_FAIL;
		
	memcpy(&m_CameraDesc, pArg, sizeof m_CameraDesc);

	if(FAILED(m_pTransformCom->Initialize(&m_CameraDesc.TransformDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_CameraDesc.vEye));

	m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));

	return S_OK;
}

void CCamera::Tick(_double dTimeDelta)
{
	if (nullptr == m_pPipeLine)
		return;
	// Camera_Shake
	//Tick_Shake(dTimeDelta);

	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldFloat4x4_Inverse());

	_float4x4 ProjMatrix;

	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fNearZ, m_CameraDesc.fFarZ));

	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, ProjMatrix);
}

void CCamera::LateTick(_double dTimeDelta)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Tick_Shake(_double TimeDelta)
{
	if (true == m_bShake)
	{
		m_Time += TimeDelta;

		if (m_ShakeTime > m_Time)
		{
			float fX = (float)Random::Generate_Int(-m_iShakePower, m_iShakePower) / 1000.0f;
			float fY = (float)Random::Generate_Int(-m_iShakePower, m_iShakePower) / 1000.0f;
			float fZ = (float)Random::Generate_Int(-m_iShakePower, m_iShakePower) / 1000.0f;
			_vector vShake = { fX, fY, fZ, 0.f };

			_vector vCamPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vCamLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

			_vector vEye = vCamPos + vShake; // ½¦ÀÌÅ·
			
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEye);

		}
		else
		{
			m_Time = 0.0f;
			m_bShake = false;
		}
	}
}

void CCamera::Shake(const _double& ShakeTime, const _int& iShakePower)
{
	if (m_bShake == true)
	{
		if (m_ShakeTime - m_Time < ShakeTime)
		{
			m_ShakeTime = ShakeTime;
			m_iShakePower = iShakePower;

			m_bShake = true;
		}
	}
	else
	{
		m_ShakeTime = ShakeTime;
		m_iShakePower = iShakePower;

		m_bShake = true;
	}
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPipeLine);
}
