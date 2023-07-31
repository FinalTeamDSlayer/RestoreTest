#include "pch.h"
#include "..\Public\Camera_Tool.h"

#include "GameInstance.h"
#include "ImGui_Manager_Tool.h"

CCamera_Tool::CCamera_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Tool::CCamera_Tool(const CCamera_Tool& rhs)
	: CCamera(rhs)
	, m_MouseState(rhs.m_MouseState)
{
}

HRESULT CCamera_Tool::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_BOX("Failed to Initialize_Prototype : CCamera_Tool");
		return E_FAIL;
	}

	ZeroMemory(&m_MouseState, sizeof m_MouseState);

	return S_OK;
}

HRESULT CCamera_Tool::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
	{
		MSG_BOX("Failed to Initialize : CCamera_Tool");
		return E_FAIL;
	}

	if (FAILED(Add_Components()))
	{
		MSG_BOX("Failed to Add_Components : CCamera_Tool");
		return E_FAIL;
	}

	CImGui_Manager_Tool* pGUI = CImGui_Manager_Tool::GetInstance();
	Safe_AddRef(pGUI);

	pGUI->Set_CameraDesc((void*)&m_CameraDesc);

	Safe_Release(pGUI);

	return S_OK;
}

void CCamera_Tool::Tick(_double dTimeDelta)
{
	CImGui_Manager_Tool* pGUI = CImGui_Manager_Tool::GetInstance();
	Safe_AddRef(pGUI);

	CAMERADESC CameraDesc = pGUI->Get_CameraDesc();

	m_pTransformCom->Set_Speed(CameraDesc.TransformDesc.dSpeedPerSec);
	m_CameraDesc.fFarZ = (CameraDesc.fFarZ);
	m_CameraDesc.fFovY = (CameraDesc.fFovY);

	KeyInput(dTimeDelta);

	Safe_Release(pGUI);

	__super::Tick(dTimeDelta);
}

void CCamera_Tool::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);
}

HRESULT CCamera_Tool::Render()
{
	return S_OK;
}

HRESULT CCamera_Tool::Add_Components()
{
	
	return S_OK;
}

void CCamera_Tool::KeyInput(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		m_pTransformCom->Go_Straight(dTimeDelta);

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		m_pTransformCom->Go_Backward(dTimeDelta);

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		m_pTransformCom->Go_Left(dTimeDelta);

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		m_pTransformCom->Go_Right(dTimeDelta);

	_long MouseMove = { 0 };

	if (pGameInstance->Get_DIMouseState(CInput_Device::DIM_RB) & 0x80)
	{
		if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_X))
		{
			//m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_UP), (dTimeDelta * MouseMove * Get_Sensitivity()));
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (dTimeDelta * MouseMove * Get_Sensitivity()));
		}

		if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), (dTimeDelta * MouseMove * Get_Sensitivity()));
		}
	}
	
	Safe_Release(pGameInstance);
}

CCamera_Tool* CCamera_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Tool* pInstance = new CCamera_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Tool::Clone(void* pArg)
{
	CCamera_Tool* pInstance = new CCamera_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCamera_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Tool::Free()
{
	__super::Free();

}
