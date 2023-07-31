#include "pch.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"

#include "Player.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_BOX("Failed to Initialize_Prototype : Camera_Free");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
	{
		MSG_BOX("Failed to Initialize : Camera_Free");
		return E_FAIL;
	}

	return S_OK;
}

void CCamera_Free::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	
	if (pGameInstance->Get_DIKeyDown(DIK_0))
	{
		if (false == m_bCamChange)
			m_bCamChange = true;
		else if (true == m_bCamChange)
			m_bCamChange = false;
	}
	if (pGameInstance->Get_DIKeyDown(DIK_9))
	{
		if (false == m_bLockMouse)
			m_bLockMouse = true;
		else if (true == m_bLockMouse)
			m_bLockMouse = false;
	}
	if (pGameInstance->Get_DIKeyDown(DIK_Q))
	{
		if (false == m_Is_Battle)
			m_Is_Battle = true;
		else if (true == m_Is_Battle)
			m_Is_Battle = false;
	}

	if (true == m_bLockMouse)
		LockMouse();

	

	Safe_Release(pGameInstance);



	__super::Tick(dTimeDelta);
}

void CCamera_Free::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform* m_pTargetTransformCom = dynamic_cast<CTransform*>(pGameInstance->Get_Component(pGameInstance->Get_CurLevelIdx(), TEXT("Layer_Player"), TEXT("Com_Transform")));

	m_vTargetPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);

	m_vBattleTargetPos = { 140.f,0.f,120.f,1.f }; // ÀÓ½ÃÁÂÇ¥
	m_vBattleCenter = (m_vTargetPos + m_vBattleTargetPos) * 0.5f;

	
	if (m_bCamChange == true)
	{
		// adventure
		if (m_Is_Battle != true) {
			m_fDistance = { 4.f };
			m_vOffSet = { 0.f, 1.5f, 0.f, 0.f };
			m_vLookOffSet = { 0.f, 1.7f, 0.f, 0.f };

			AdventureCamera(dTimeDelta);
		}
		// Battle
		else if (m_Is_Battle == true) {

			m_fDistance = { 6.f };
			m_vOffSet = { 0.f, 2.3f, 0.f, 0.f };
			m_vLookOffSet = { 0.f, 1.5f, 0.f, 0.f };

			BattleCamera(dTimeDelta);
		}
	}
	else
	{
		FreeCamera(dTimeDelta);
	}

	XMStoreFloat4(&m_fCameraLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	Safe_Release(pGameInstance);
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::FreeCamera(_double dTimeDelta)
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

	// Camera_Shake
	if (pGameInstance->Get_DIKeyDown(DIK_LCONTROL))
		Shake(1.f, 500);

	_long MouseMove = { 0 };

	if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), (dTimeDelta * MouseMove * Get_Sensitivity()));
	}

	if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), (dTimeDelta * MouseMove * Get_Sensitivity()));
	}

	Safe_Release(pGameInstance);
}

void CCamera_Free::AdventureCamera(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_vector vUp = XMVector3Normalize({ 0.f,1.f,0.f });

	_vector vCamPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	Turn_Camera(dTimeDelta);

	_vector vDist = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_vTargetPos;

	_float dist = XMVectorGetX(XMVector3Length(vDist));

	_float Y = 0.08f + dist * 0.04f;

	m_vDist = { XMVectorGetX(m_vDist), 0.12f ,XMVectorGetZ(m_vDist), 0.f };
	m_vDist = XMVector3Normalize(m_vDist);

	_vector vDest = m_vTargetPos + m_vOffSet + (m_vDist * m_fDistance);

	_float t = (_float)dTimeDelta * m_fDamping;

	if (t > 1.0f)
		t = 1.0f;
	else if (t < 0.f)
		t = 0.0f;

	_vector CamPos;

	CamPos = XMVectorLerp(vCamPosition, vDest, t);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CamPos);

	_vector NewLook = m_vTargetPos + m_vLookOffSet - vCamPosition;

	NewLook = XMVector3Normalize(NewLook);

	_float New_t = (_float)dTimeDelta * m_fLookDamping;

	m_pTransformCom->LerpVector(NewLook, New_t);
	
	Safe_Release(pGameInstance);
}

void CCamera_Free::BattleCamera(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_vector vUp = XMVector3Normalize({ 0.f,1.f,0.f });

	_vector vCamPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_vDist = m_vTargetPos - m_vBattleTargetPos;

	_float dist = XMVectorGetX(XMVector3Length(m_vDist));

	// Combo_On
	if (pGameInstance->Get_DIKeyDown(DIK_R) && dist <= 7.f) {
		if (m_bIs_Combo_On == true)
			m_bIs_Combo_On = false;
		else {
			m_bIs_Combo_On = true;
		}
	}

	if (dist > 7.f) {
		m_bIs_Combo_On = false;
	}

	if (m_bIs_Combo_On)
		m_vCameraAngle = 80.f + dist * 2.f;
	else {
		m_vCameraAngle = 15.f;
	}

	_matrix		RotationMatrix = XMMatrixRotationAxis(vUp, XMConvertToRadians(-m_vCameraAngle));

	m_vDist = XMVector3TransformNormal(m_vDist, RotationMatrix);

	_float Y;

	if (dist < 2.f) {
		Y = 0.1f;
	}
	else {
		Y = 0.3f - (1 / dist) * 0.2f;
	}

	m_vDist = { XMVectorGetX(m_vDist), Y ,XMVectorGetZ(m_vDist), XMVectorGetW(m_vDist) };
	m_vDist = XMVector3Normalize(m_vDist);

	_vector vDest = m_vTargetPos + m_vOffSet + (m_vDist * m_fDistance);

	_float t = (_float)dTimeDelta * m_fDamping;

	_vector CamPos = XMVectorLerp(vCamPosition, vDest, t);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CamPos);

	_vector NewLook = m_vBattleCenter + m_vLookOffSet - vCamPosition;

	NewLook = XMVector3Normalize(NewLook);

	_float New_t = (_float)dTimeDelta * m_fLookDamping;

	
	m_pTransformCom->LerpVector(NewLook, New_t);
	

	Safe_Release(pGameInstance);

}

void CCamera_Free::CutInCamera(_double dTimeDelta)
{
}

void CCamera_Free::Turn_Camera(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_vector vUp = XMVector3Normalize({ 0.f,1.f,0.f });

	m_vAdventureCameraAngle = 0.f;

	if (pGameInstance->Get_DIKeyState(DIK_LEFTARROW) & 0x80) {

		m_vAdventureCameraAngle -= (_float)TimeDelta * 100.f;

		_matrix		RotationMatrix = XMMatrixRotationAxis(vUp, XMConvertToRadians(m_vAdventureCameraAngle));

		m_vDist = XMVector3TransformNormal(m_vDist, RotationMatrix);

	}
	else if (pGameInstance->Get_DIKeyState(DIK_RIGHTARROW) & 0x80) {

		m_vAdventureCameraAngle += (_float)TimeDelta * 100.f;

		_matrix		RotationMatrix = XMMatrixRotationAxis(vUp, XMConvertToRadians(m_vAdventureCameraAngle));

		m_vDist = XMVector3TransformNormal(m_vDist, RotationMatrix);

	}
	

	Safe_Release(pGameInstance);

}

void CCamera_Free::LockMouse()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	ShowCursor(false);
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}