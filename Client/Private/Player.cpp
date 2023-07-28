#include "pch.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "SoundMgr.h"
#include "Camera_Free.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CCharacter(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (true == m_isDead)
		return;

	Key_Input(dTimeDelta);

}

void CPlayer::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);


}

HRESULT CPlayer::Render()
{

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth()
{

	return S_OK;
}

void CPlayer::Key_Input(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

#pragma region Test
	if (pGameInstance->Get_DIKeyState(DIK_HOME) & 0x80)
	{
		++m_iNumAnim;
		if (m_pModelCom->Get_NumAnims() <= m_iNumAnim)
			m_iNumAnim = m_pModelCom->Get_NumAnims() - 1;
		m_pModelCom->Set_Animation(m_iNumAnim);
	}

	if (pGameInstance->Get_DIKeyState(DIK_END) & 0x80)
	{
		if (0 < m_iNumAnim)
			--m_iNumAnim;
		if (0 > m_iNumAnim)
			m_iNumAnim = 0;
		m_pModelCom->Set_Animation(m_iNumAnim);
	}
	if (pGameInstance->Get_DIKeyState(DIK_UP))
	{
		m_pTransformCom->Go_Straight(dTimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_DOWN))
	{
		m_pTransformCom->Go_Backward(dTimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_LEFT))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -dTimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), dTimeDelta);
	}

#pragma endregion

	Key_Input_Battle_Move(dTimeDelta);

	Key_Input_Battle_Attack(dTimeDelta);

	Key_Input_Battle_Skill(dTimeDelta);


	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Move(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//카메라 방향 구해놓기
	CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), 0));
	_float4 CameraLook = pCamera->Get_CameraLook();
	CameraLook.y = 0.0f;
	CameraLook.w = 0.0f;
	_vector vLook = XMVector4Normalize(XMLoadFloat4(&CameraLook));
	_vector	vUp = { 0.0f, 1.0f, 0.0f , 0.0f };
	_vector crossLeft = XMVector3Cross(vLook, vUp);

	//45degree look
	_vector quaternionRotation = XMQuaternionRotationAxis(vUp, XMConvertToRadians(45.0f));
	_vector v45Rotate = XMVector3Rotate(vLook, quaternionRotation);

	//135degree look
	_vector quaternionRotation2 = XMQuaternionRotationAxis(vUp, XMConvertToRadians(135.0f));
	_vector v135Rotate = XMVector3Rotate(vLook, quaternionRotation2);


	//무브키를 누르고 있는 상태
	if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S)
		|| pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_Moveset.m_State_Battle_Run = true;
		m_dTime_MoveKey = 0.0;
	}
	else
	{
		m_Moveset.m_State_Battle_Run = false;
	}

	//Dir설정
	if (pGameInstance->Get_DIKeyState(DIK_W) && pGameInstance->Get_DIKeyState(DIK_A))
	{
		XMStoreFloat4(&m_Moveset.m_Input_Dir, -v135Rotate);
		XMStoreFloat4(&m_Moveset.m_Input_Dir, XMVector4Normalize(XMLoadFloat4(&m_Moveset.m_Input_Dir)));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_W) && pGameInstance->Get_DIKeyState(DIK_D))
	{
		XMStoreFloat4(&m_Moveset.m_Input_Dir, v45Rotate);
		XMStoreFloat4(&m_Moveset.m_Input_Dir, XMVector4Normalize(XMLoadFloat4(&m_Moveset.m_Input_Dir)));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_S) && pGameInstance->Get_DIKeyState(DIK_A))
	{
		XMStoreFloat4(&m_Moveset.m_Input_Dir, -v45Rotate);
		XMStoreFloat4(&m_Moveset.m_Input_Dir, XMVector4Normalize(XMLoadFloat4(&m_Moveset.m_Input_Dir)));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_S) && pGameInstance->Get_DIKeyState(DIK_D))
	{
		XMStoreFloat4(&m_Moveset.m_Input_Dir, v135Rotate);
		XMStoreFloat4(&m_Moveset.m_Input_Dir, XMVector4Normalize(XMLoadFloat4(&m_Moveset.m_Input_Dir)));
	}
	else
	{
		if (pGameInstance->Get_DIKeyState(DIK_W))
		{
			XMStoreFloat4(&m_Moveset.m_Input_Dir, vLook);
		}
		else if (pGameInstance->Get_DIKeyState(DIK_S))
		{
			XMStoreFloat4(&m_Moveset.m_Input_Dir, -vLook);
		}
		else if (pGameInstance->Get_DIKeyState(DIK_A))
		{
			XMStoreFloat4(&m_Moveset.m_Input_Dir, crossLeft);
		}
		else if (pGameInstance->Get_DIKeyState(DIK_D))
		{
			XMStoreFloat4(&m_Moveset.m_Input_Dir, -crossLeft);
		}
	}

	//키를 누를 시
	if (!m_isCool_MoveKey)
	{
		if (pGameInstance->Get_DIKeyDown(DIK_W) || pGameInstance->Get_DIKeyDown(DIK_S)
			|| pGameInstance->Get_DIKeyDown(DIK_A) || pGameInstance->Get_DIKeyDown(DIK_D))
		{
			m_Moveset.m_Down_Battle_Run = true;
		}
	}

	// 키를 뗄 시
	if (pGameInstance->Get_DIKeyUp(DIK_W) || pGameInstance->Get_DIKeyUp(DIK_S)
		|| pGameInstance->Get_DIKeyUp(DIK_A) || pGameInstance->Get_DIKeyUp(DIK_D))
	{
		m_isCool_MoveKey = true;
	}

	// 키를 뗄 시 자연스러움 추가
	m_dTime_MoveKey += dTimeDelta;
	if (0.1f < m_dTime_MoveKey && m_isCool_MoveKey)
	{
		m_isCool_MoveKey = false;
		m_Moveset.m_Up_Battle_Run = true;
	}


	//무빙제한 상태에서 누르고 있을 시
	if (m_Moveset.m_isRestrict_Move)
	{
		if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
			m_Moveset.m_isPressing_While_Combo = true;
		else
			m_Moveset.m_isPressing_While_Combo = false;
	}

	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Attack(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 콤보공격
	if (pGameInstance->Get_DIKeyDown(DIK_J))
	{
		m_Moveset.m_Down_Battle_Combo = true;

		//콤보 분기용
		if (pGameInstance->Get_DIKeyState(DIK_W))
		{
			m_Moveset.m_Down_Battle_Combo_Up = true;
			m_Moveset.m_Down_Battle_Combo_Down = false;
		}
		else if (pGameInstance->Get_DIKeyState(DIK_S))
		{
			m_Moveset.m_Down_Battle_Combo_Up = false;
			m_Moveset.m_Down_Battle_Combo_Down = true;
		}
		else
		{
			m_Moveset.m_Down_Battle_Combo_Up = false;
			m_Moveset.m_Down_Battle_Combo_Down = false;
		}
	}

	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Skill(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (pGameInstance->Get_DIKeyDown(DIK_I))
	{
		if (pGameInstance->Get_DIKeyState(DIK_O))
		{
			m_Moveset.m_Down_Skill_Guard = true;
		}
		else if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S)
			|| pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
		{
			m_Moveset.m_Down_Skill_Move = true;
		}
		else
		{
			m_Moveset.m_Down_Skill_Normal = true;
		}
	}

	Safe_Release(pGameInstance);
}

HRESULT CPlayer::Add_Components()
{

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
{

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
}
