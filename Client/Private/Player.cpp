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

void CPlayer::Dir_Setting(_bool Reverse)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//카메라 방향 구해놓기
	CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(pGameInstance->Get_GameObject(pGameInstance->Get_CurLevelIdx(), TEXT("Layer_Camera"), 0));
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
	if (Reverse)
	{
		v45Rotate = -v45Rotate;
		v135Rotate = -v135Rotate;
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

	Safe_Release(pGameInstance);
}

void CPlayer::Trigger_Hit(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	//임시 히트
	if (pGameInstance->Get_DIKeyDown(DIK_Z))
	{
		m_Moveset.m_Down_Dmg_Small = true;
	}

	Safe_Release(pGameInstance);
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

	if (pGameInstance->Get_DIKeyDown(DIK_V))
	{
		m_isSpecialHit = true;
	}
	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -dTimeDelta);
#pragma endregion

	Trigger_Hit(dTimeDelta);

	if (m_Moveset.m_isHitMotion == false)
	{
		Key_Input_Battle_Move(dTimeDelta);

		Key_Input_Battle_Jump(dTimeDelta);

		Key_Input_Battle_Attack(dTimeDelta);

		Key_Input_Battle_ChargeAttack(dTimeDelta);

		Key_Input_Battle_Skill(dTimeDelta);

		Key_Input_Battle_Guard(dTimeDelta);

		Key_Input_Battle_Dash(dTimeDelta);

		Key_Input_Battle_Awaken(dTimeDelta);

		Key_Input_Battle_Special(dTimeDelta);

	}
	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Move(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	
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

	Dir_Setting(false);
	

	//키를 누를 시
	if (!m_isCool_MoveKey)
	{
		if (pGameInstance->Get_DIKeyDown(DIK_W) || pGameInstance->Get_DIKeyDown(DIK_S)
			|| pGameInstance->Get_DIKeyDown(DIK_A) || pGameInstance->Get_DIKeyDown(DIK_D))
		{
			if(m_Moveset.m_isRestrict_Move == false)
				m_Moveset.m_Down_Battle_Run = true;
		}
	}

	// 키를 뗄 시
	if (pGameInstance->Get_DIKeyUp(DIK_W) || pGameInstance->Get_DIKeyUp(DIK_S)
		|| pGameInstance->Get_DIKeyUp(DIK_A) || pGameInstance->Get_DIKeyUp(DIK_D))
	{
		if(m_Moveset.m_isRestrict_Charge == false )
			m_isCool_MoveKey = true;
	}

	// 키를 뗄 시 자연스러움 추가
	m_dTime_MoveKey += dTimeDelta;
	if (0.1f < m_dTime_MoveKey && m_isCool_MoveKey)
	{
		m_isCool_MoveKey = false;

		if(m_Moveset.m_isRestrict_Move == false)
			m_Moveset.m_Up_Battle_Run = true;
	}


	//무빙제한 상태에서 누르고 있을 시
	if (m_Moveset.m_isRestrict_Move)
	{
		if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
			m_Moveset.m_isPressing_While_Restrict = true;
		else
			m_Moveset.m_isPressing_While_Restrict = false;
	}
	

	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Jump(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	

	if (false == m_Moveset.m_isRestrict_KeyInput && false == m_Moveset.m_isRestrict_Jump )
	{
		if (pGameInstance->Get_DIKeyDown(DIK_K))
		{
			if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
			{
				m_Moveset.m_Down_Battle_JumpMove = true;
				m_Moveset.m_Down_Battle_Jump = false;

				Dir_Setting(false);
				
			}
			else
			{
				m_Moveset.m_Down_Battle_JumpMove = false;
				m_Moveset.m_Down_Battle_Jump = true;
			}

		}
	}

	// Jump attack
	if (m_isJumpOn)
	{
		if (pGameInstance->Get_DIKeyDown(DIK_J) && m_isFirst_JumpAtk)
		{
			//점프 트랙 공격 (이동키 + J키)
			if ( pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
			{
				if(false == m_Moveset.m_isRestrict_JumpCombo)	
					m_Moveset.m_Down_Battle_Jump_TrackAttack = true;
				else
					m_Moveset.m_Down_Battle_Jump_Attack = true;
			}
			// 점프 콤보 공격
			else
			{
				m_Moveset.m_Down_Battle_Jump_Attack = true;
			}
			
		}
	}


	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Attack(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (false == m_Moveset.m_isRestrict_KeyInput && false == m_Moveset.m_isRestrict_Jump && false == m_Moveset.m_isRestrict_Charge)
	{
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
	}
	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_ChargeAttack(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_dDelay_Charge_J += dTimeDelta;
	m_dDelay_Charge_W += dTimeDelta;

	if (false == m_Moveset.m_isRestrict_KeyInput && false == m_Moveset.m_isRestrict_Jump)
	{
		// 차지공격
		if (pGameInstance->Get_DIKeyDown(DIK_J))
		{
			m_dDelay_Charge_J = 0.0;
		}
		if (pGameInstance->Get_DIKeyDown(DIK_W))
		{
			m_dDelay_Charge_W = 0.0;
		}
		// 둘이 동시에 누른거 딜레이 확인
		if (m_dDelay_Charge_J < 0.015f && m_dDelay_Charge_W < 0.015f)
		{
			m_isCan_Charge = true;
		}
		//딜레이 확인 후, 둘이 동시에 눌린 상태인제 확인
		if (m_isCan_Charge && pGameInstance->Get_DIKeyState(DIK_J) && pGameInstance->Get_DIKeyState(DIK_W))
		{
			m_isCan_Charge = false;
			m_Moveset.m_Down_Battle_Charge = true;
			m_isCharging = true;
		}
		//차지상태
		if (m_isCharging && pGameInstance->Get_DIKeyState(DIK_J) && pGameInstance->Get_DIKeyState(DIK_W))
		{
			m_Moveset.m_State_Battle_Charge = true;
		}
		else
		{
			m_Moveset.m_State_Battle_Charge = false;
		}
		
		
	}

	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Skill(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (false == m_Moveset.m_isRestrict_KeyInput)
	{
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
	}
	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Guard(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//카메라 방향 구해놓기
	CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(pGameInstance->Get_GameObject(pGameInstance->Get_CurLevelIdx(), TEXT("Layer_Camera"), 0));
	_float4 CameraLook = pCamera->Get_CameraLook();
	CameraLook.y = 0.0f;
	CameraLook.w = 0.0f;
	_vector vLook = XMVector4Normalize(XMLoadFloat4(&CameraLook));

	if (m_Moveset.m_isRestrict_Throw == false && m_isComboing == false)
	{
		if (pGameInstance->Get_DIKeyDown(DIK_O))
		{
			m_Moveset.m_Down_Battle_Guard = true;
			XMStoreFloat4(&m_Moveset.m_Input_Dir, vLook);
		}


		if (pGameInstance->Get_DIKeyState(DIK_O))
		{
			m_Moveset.m_State_Battle_Guard = true;


		}
		else
		{
			m_Moveset.m_State_Battle_Guard = false;
		}


		if (pGameInstance->Get_DIKeyUp(DIK_O))
		{
			if (m_Moveset.m_isRestrict_Jump == false && m_Moveset.m_isRestrict_KeyInput == false)
				m_Moveset.m_Up_Battle_Guard = true;
		}
	}

	if (m_Moveset.m_isRestrict_Throw == false)
	{
		//잡기공격 ( O키 + J키)
		if (pGameInstance->Get_DIKeyState(DIK_O) )
		{
			if (pGameInstance->Get_DIKeyDown(DIK_J))
				m_Moveset.m_Down_Battle_Throw = true;
			else if (pGameInstance->Get_DIKeyDown(DIK_W) || pGameInstance->Get_DIKeyDown(DIK_S) || pGameInstance->Get_DIKeyDown(DIK_A) || pGameInstance->Get_DIKeyDown(DIK_D))
				m_Moveset.m_Down_Battle_Push = true;
		}
	}
	
	if (m_isMaintain_Guard)
	{
		m_isMaintain_Guard = false;
		m_isThrowing = false;

		if(pGameInstance->Get_DIKeyState(DIK_O))
			m_Moveset.m_Down_Battle_Guard = true;
	}


	

	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Dash(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//카메라 방향 구해놓기
	CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(pGameInstance->Get_GameObject(pGameInstance->Get_CurLevelIdx(), TEXT("Layer_Camera"), 0));
	_float4 CameraLook = pCamera->Get_CameraLook();
	CameraLook.y = 0.0f;
	CameraLook.w = 0.0f;
	_vector vLook = XMVector4Normalize(XMLoadFloat4(&CameraLook));


	if (pGameInstance->Get_DIKeyDown(DIK_L))
	{
		//스텝
		if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_A) || pGameInstance->Get_DIKeyState(DIK_D))
		{
			if (m_Moveset.m_isRestrict_DoubleStep == false)
			{
				if (pGameInstance->Get_DIKeyState(DIK_W) && m_Moveset.m_isRestrict_Step == false)
				{
					Dir_Setting(true);
					XMStoreFloat4(&m_vLook, vLook);
					m_Moveset.m_Down_Battle_Step = true;

					m_isForward = true;
					m_isBack = false;
					m_isLeft = false;
					m_isRight = false;
				}
				else if (pGameInstance->Get_DIKeyState(DIK_S) && m_Moveset.m_isRestrict_Step == false)
				{
					Dir_Setting(true);
					XMStoreFloat4(&m_vLook, vLook);
					m_Moveset.m_Down_Battle_Step = true;

					m_isForward = false;
					m_isBack = true;
					m_isLeft = false;
					m_isRight = false;
				}
				else if (pGameInstance->Get_DIKeyState(DIK_A))
				{
					Dir_Setting(true);
					XMStoreFloat4(&m_vLook, vLook);

					m_Moveset.m_Down_Battle_Step = true;

					m_isForward = false;
					m_isBack = false;
					m_isLeft = true;
					m_isRight = false;
				}
				else if (pGameInstance->Get_DIKeyState(DIK_D))
				{
					Dir_Setting(true);
					XMStoreFloat4(&m_vLook, vLook);
					m_Moveset.m_Down_Battle_Step = true;

					m_isForward = false;
					m_isBack = false;
					m_isLeft = false;
					m_isRight = true;
				}
			}
		}
		//대시
		else
		{
			if (m_Moveset.m_isRestrict_Dash == false)
			{
				XMStoreFloat4(&m_Moveset.m_Input_Dir, vLook);
				m_Moveset.m_Down_Battle_Dash = true;
			}
		}
	}

	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Awaken(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_Moveset.m_iAwaken != 0)
	{
		m_Moveset.m_dTime_Awaken_Duration -= dTimeDelta;

		if (m_Moveset.m_dTime_Awaken_Duration <= 0.0)
		{
			m_Moveset.m_dTime_Awaken_Duration = 10.0;
			m_Moveset.m_iAwaken = 0;
		}
	}

	if (pGameInstance->Get_DIKeyDown(DIK_Q))
	{
		m_Moveset.m_Down_Battle_Awaken = true;

		if (m_Moveset.m_iAwaken == 0)
		{
			m_Moveset.m_iAwaken = 1;
			m_Moveset.m_dTime_Awaken_Duration = 10.0;
		}
		else if (m_Moveset.m_iAwaken == 1)
		{
			m_Moveset.m_iAwaken = 2;
			m_Moveset.m_dTime_Awaken_Duration = 10.0;
		}
	}


	Safe_Release(pGameInstance);
}

void CPlayer::Key_Input_Battle_Special(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_Moveset.m_isRestrict_Special == false)
	{
		if (m_Moveset.m_isRestrict_Step == false)
		{
			if (pGameInstance->Get_DIKeyDown(DIK_E))
			{
				m_Moveset.m_Down_Battle_Special = true;
			}
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
