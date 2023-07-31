#include "pch.h"
#include "..\Public\Player_Tanjiro.h"

#include "GameInstance.h"
#include "SoundMgr.h"


CPlayer_Tanjiro::CPlayer_Tanjiro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer(pDevice, pContext)
{
}

CPlayer_Tanjiro::CPlayer_Tanjiro(const CPlayer_Tanjiro& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Tanjiro::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Tanjiro::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(ANIM_BATTLE_IDLE);

	if (FAILED(Read_Animation_Control_File("Tanjiro.bin")))
	{
		MSG_BOX("Failed to AnimData Read : Tanjiro");
		return E_FAIL;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 150.f,0.f,150.f,1.f });

	return S_OK;
}

void CPlayer_Tanjiro::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (true == m_isDead)
		return;

	Animation_Control(dTimeDelta);

	//애니메이션 처리
	m_pModelCom->Play_Animation(dTimeDelta);
	RootAnimation(dTimeDelta);

	//이벤트 콜
	EventCall_Control(dTimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this)))
		return;
}

void CPlayer_Tanjiro::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);

	Gravity(dTimeDelta);

	

#ifdef _DEBUG
	/*if (FAILED(m_pRendererCom->Add_DebugGroup(m_pNavigationCom)))
		return;*/
#endif
}

HRESULT CPlayer_Tanjiro::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	//Outline Render
	for (m_iMeshNum = 0; m_iMeshNum < iNumMeshes; m_iMeshNum++)
	{
		if (FAILED(m_pModelCom->Bind_ShaderResource(m_iMeshNum, m_pShaderCom, "g_DiffuseTexture", MESHMATERIALS::TextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_ShaderBoneMatrices(m_iMeshNum, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		if (m_iMeshNum == 2)
			m_pShaderCom->Begin(2);
		else
			m_pShaderCom->Begin(1);

		m_pModelCom->Render(m_iMeshNum);
	}

	// Default Render
	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_ShaderResource(i, m_pShaderCom, "g_DiffuseTexture", MESHMATERIALS::TextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_ShaderBoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CPlayer_Tanjiro::Render_ShadowDepth()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();


	_vector vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector	vLightEye = XMVectorSet(-5.f, 10.f, -5.f, 1.f);
	_vector	vLightAt = XMVectorSet(60.f, 0.f, 60.f, 1.f);
	_vector	vLightUp = XMVectorSet(0.f, 1.f, 0.f, 1.f);


	_matrix      LightViewMatrix = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	_float4x4   FloatLightViewMatrix;
	XMStoreFloat4x4(&FloatLightViewMatrix, LightViewMatrix);

	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ViewMatrix",
		&FloatLightViewMatrix)))
		return E_FAIL;

	_matrix      LightProjMatrix;
	_float4x4   FloatLightProjMatrix;

	LightProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.f), _float(1280) / _float(720), 0.2f, 300.f);
	XMStoreFloat4x4(&FloatLightProjMatrix, LightProjMatrix);

	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ProjMatrix",
		&FloatLightProjMatrix)))
		return E_FAIL;


	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_ShaderResource(i, m_pShaderCom, "g_DiffuseTexture", MESHMATERIALS::TextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_ShaderBoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;



		m_pShaderCom->Begin(3);

		m_pModelCom->Render(i);
	}

	return S_OK;
}


void CPlayer_Tanjiro::EventCall_Control(_double dTimeDelta)
{
	CAnimation* pAnim = m_pModelCom->Get_Animation();
	if (pAnim->Get_AnimationDesc().m_dTimeAcc == 0)
	{
		m_iEvent_Index = 0;
	}

	if (EventCallProcess())
	{
#pragma region Combo_Attack
		if (ANIM_ATK_COMBO == m_pModelCom->Get_iCurrentAnimIndex())
		{
			if (0 == m_iEvent_Index)
			{
				_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
				CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
			}

		}
		if (22 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			if (0 == m_iEvent_Index)
			{
				_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
				CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
			}
			else if (1 == m_iEvent_Index)
			{
				_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
				CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
			}
			
		}
		if (23 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			if (0 == m_iEvent_Index)
			{
				_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
				CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
			}
		}
		if (25 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			if (0 == m_iEvent_Index)
			{
				_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
				CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
			}
		}
		if (ANIM_ATK_SPECIAL_CUTSCENE == m_pModelCom->Get_iCurrentAnimIndex())
		{
			
		}
#pragma endregion
		m_iEvent_Index++;
	}
}

void CPlayer_Tanjiro::Animation_Control(_double dTimeDelta)
{
	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), dTimeDelta);

	Moving_Restrict();

	Animation_Control_Battle_Dmg(dTimeDelta);

	if (m_Moveset.m_isHitMotion == false)
	{
		Animation_Control_Battle_Jump(dTimeDelta);

		Animation_Control_Battle_Move(dTimeDelta);

		Animation_Control_Battle_Attack(dTimeDelta);

		Animation_Control_Battle_Charge(dTimeDelta);

		Animation_Control_Battle_Skill(dTimeDelta);

		Animation_Control_Battle_Guard(dTimeDelta);

		Animation_Control_Battle_Dash(dTimeDelta);

		Animation_Control_Battle_Awaken(dTimeDelta);

		Animation_Control_Battle_Special(dTimeDelta);
	}
}

void CPlayer_Tanjiro::Animation_Control_Battle_Move(_double dTimeDelta)
{
	//무빙제한시 리턴
	if (m_Moveset.m_isRestrict_Move)
	{
		return;
	}
	//무빙제한이 풀릴시 이동 설정, 버그수정
	if (m_Moveset.m_isPressing_While_Restrict)
	{
		m_Moveset.m_isPressing_While_Restrict = false;
		m_Moveset.m_Down_Battle_Run = true;
	}


	//무빙키입력들
	if (m_Moveset.m_Down_Battle_Run)
	{
		m_Moveset.m_Down_Battle_Run = false;
		m_pModelCom->Set_Animation(ANIM_BATTLE_RUN);
	}

	if (m_Moveset.m_State_Battle_Run)
	{
		m_pTransformCom->Set_Look(m_Moveset.m_Input_Dir);
		m_fMove_Speed = 2.0f;
		m_pTransformCom->Go_Straight(dTimeDelta * m_fMove_Speed);
	}

	if (m_Moveset.m_Up_Battle_Run)
	{
		m_Moveset.m_Up_Battle_Run = false;
		m_pModelCom->Set_Animation(ANIM_BATTLE_RUN_END);
	}
	Go_Straight_Deceleration(dTimeDelta, ANIM_BATTLE_RUN_END, m_fMove_Speed , 0.15f); 
}

void CPlayer_Tanjiro::Animation_Control_Battle_Jump(_double dTimeDelta)
{
	if (m_Moveset.m_Down_Battle_JumpMove)
	{
		m_pTransformCom->Set_Look(m_Moveset.m_Input_Dir);
		m_Moveset.m_Down_Battle_JumpMove = false;
		m_isJump_Move = true;

		m_pModelCom->Set_Animation(ANIM_BATTLE_JUMP);
		Jumping(4.5f, 0.2f);

		m_pModelCom->Set_EarlyEnd(85, true);
	}
	if (m_isJump_Move)
	{
		Go_Straight_Constant(dTimeDelta, ANIM_BATTLE_JUMP, m_fMove_Speed * 1.2f);
		Go_Straight_Constant(dTimeDelta, 84, m_fMove_Speed * 1.2f);
		Go_Straight_Constant(dTimeDelta, 85, m_fMove_Speed * 1.2f);
		Go_Straight_Deceleration(dTimeDelta, 86, m_fMove_Speed * 1.2f, 0.16f); // Down
	}
	Ground_Animation_Play(85, 86);



	if (m_Moveset.m_Down_Battle_Jump)
	{
		m_Moveset.m_Down_Battle_Jump = false;
		m_isJump_Move = false;

		m_pModelCom->Set_Animation(ANIM_BATTLE_JUMP);
		Jumping(4.5f, 0.2f);

		m_pModelCom->Set_EarlyEnd(85, true);
	}
	


	//공중 공격 콤보
	if (m_Moveset.m_Down_Battle_Jump_Attack)
	{
		m_Moveset.m_Down_Battle_Jump_Attack = false;
		m_isJump_Move = false;

		//콤보 첫 애니메이션 설정
		if (m_pModelCom->Get_Combo_Doing() == false)
		{
			m_pModelCom->Set_Combo_Doing(true);
			m_pModelCom->Set_Animation(ANIM_ATK_AIRCOMBO);

			JumpStop(0.3);
		}
		//아닐경우, 다음 콤보로 진행
		else
		{
			m_pModelCom->Set_Combo_Trigger(true);

			m_pModelCom->Set_EarlyEnd(30, true);
			JumpStop(0.65);
			m_isFirst_JumpAtk = false;
		}
	}


	//공중 트랙공격 (이동키 + 공격키)
	if (m_Moveset.m_Down_Battle_Jump_TrackAttack)
	{
		m_Moveset.m_Down_Battle_Jump_TrackAttack = false;

		m_isFirst_JumpAtk = false;

		m_pModelCom->Set_Animation(ANIM_ATK_AIRTRACK);
		JumpStop(0.3);
		Set_FallingStatus(3.0f, 0.0f);
	}
	Ground_Animation_Play(50, 51);
	Go_Straight_Constant(dTimeDelta, 50, 3.f);
	Go_Straight_Deceleration(dTimeDelta, 51, 3.f, 0.2f); // Down
}

void CPlayer_Tanjiro::Animation_Control_Battle_Attack(_double dTimeDelta)
{
	_int iCurAnimIndex = m_pModelCom->Get_iCurrentAnimIndex();

	// 콤보공격
	if (m_Moveset.m_Down_Battle_Combo)
	{
		m_Moveset.m_Down_Battle_Combo = false;
		m_isComboing = true;

		//첫 애니메이션 설정
		if (m_pModelCom->Get_Combo_Doing() == false)
		{
			m_pModelCom->Set_Combo_Doing(true);
			m_pModelCom->Set_Animation(ANIM_ATK_COMBO);
		}
		//아닐경우, 다음 콤보로 진행
		else
		{
			m_pModelCom->Set_Combo_Trigger(true);
			//콤보 분기 설정
			if (23 == iCurAnimIndex)
			{
				// 아래콤보 s콤보
				if (m_Moveset.m_Down_Battle_Combo_Down)
				{
					m_pModelCom->Set_Combo_Another(24);
				}
				// 위콤보 w콤보
				else if (m_Moveset.m_Down_Battle_Combo_Up)
				{
					m_pModelCom->Set_Combo_Another(26);
				}
			}
		}
	}
	// 공격 모션별 전진이동 제어 (Timedelta, 애니메이션인덱스,  초기화속도,  감속도)
	Go_Straight_Deceleration(dTimeDelta, ANIM_ATK_COMBO, 3.0f, 0.3f);
	Go_Straight_Deceleration(dTimeDelta, 22, 3.0f, 0.16f);
	Go_Straight_Deceleration(dTimeDelta, 23, 4.0f, 0.11f);
	//분기
	Go_Straight_Deceleration(dTimeDelta, 24, 3.2f, 0.05f); // Down
	Go_Straight_Deceleration(dTimeDelta, 25, 5.0f, 0.35f); // Normal
	Go_Straight_Deceleration(dTimeDelta, 26, 3.0f, 0.29f); // Up

	if (m_pModelCom->Get_iCurrentAnimIndex() == ANIM_BATTLE_IDLE)
	{
		m_isComboing = false;
	}
}

void CPlayer_Tanjiro::Animation_Control_Battle_Charge(_double dTimeDelta)
{
	if (m_Moveset.m_Down_Battle_Charge)
	{
		m_Moveset.m_Down_Battle_Charge = false;

		m_pModelCom->Set_Animation(ANIM_ATK_CHARGE);
	}



	if (m_isCharging && m_Moveset.m_State_Battle_Charge == false)
	{
		m_isCharging = false;

		m_pModelCom->Set_Animation(33);
	}
	Go_Straight_Deceleration(dTimeDelta, 33, 4.5f, 0.15f);
}

void CPlayer_Tanjiro::Animation_Control_Battle_Skill(_double dTimeDelta)
{
	//스킬_0
	if (m_Moveset.m_Down_Skill_Normal)
	{
		m_Moveset.m_Down_Skill_Normal = false;

		m_pModelCom->Set_Animation(ANIM_ATK_SKILL_NORMAL);
		Jumping(4.0f, 0.18f);
	}
	Go_Straight_Deceleration(dTimeDelta, ANIM_ATK_SKILL_NORMAL, 3.0f, 0.07f);
	

	//스킬_1 : 이동키 + I키
	if (m_Moveset.m_Down_Skill_Move)
	{
		m_Moveset.m_Down_Skill_Move = false;

		m_pModelCom->Set_Animation(ANIM_ATK_SKILL_MOVE);
		Jumping(0.3f, 0.07f);

		m_pModelCom->Get_Animation()->Set_EarlyEnd(true);
	}
	Go_Straight_Constant(dTimeDelta, ANIM_ATK_SKILL_MOVE, 2.7f);
	Go_Straight_Constant(dTimeDelta, 40, 2.7f);
	Go_Straight_Deceleration(dTimeDelta, 41, 2.7f, 0.11f);


	//스킬_2 : 가드키 + I키
	if (m_Moveset.m_Down_Skill_Guard)
	{
		m_Moveset.m_Down_Skill_Guard = false;

		m_pModelCom->Set_Animation(ANIM_ATK_SKILL_GUARD);
		Jumping(3.0f, 0.05f);
	}
	Go_Straight_Deceleration(dTimeDelta, ANIM_ATK_SKILL_GUARD, 5.f, 0.25f);
}

void CPlayer_Tanjiro::Animation_Control_Battle_Guard(_double dTimeDelta)
{
	//가드 시작
	if (m_Moveset.m_Down_Battle_Guard)
	{
		m_Moveset.m_Down_Battle_Guard = false;

		m_pTransformCom->Set_Look(m_Moveset.m_Input_Dir);
		m_pModelCom->Set_Animation(ANIM_BATTLE_GUARD);
	}

	//가드 도중


	//가드 내리기
	if (m_Moveset.m_Up_Battle_Guard)
	{
		m_Moveset.m_Up_Battle_Guard = false;

		m_pModelCom->Set_Animation(65);
	}


	//잡기 ( O키 가드키 + J키 공격키)
	if (m_Moveset.m_Down_Battle_Throw)
	{
		m_Moveset.m_Down_Battle_Throw = false;

		m_isThrowing = true;
		m_pModelCom->Set_Animation(ANIM_ATK_THROW);
	}
	if (m_isThrowing && m_pModelCom->Get_iCurrentAnimIndex() == ANIM_BATTLE_IDLE && m_Moveset.m_State_Battle_Guard)
	{
		m_isMaintain_Guard = true;
	}
	
	//잡기 ( O키 가드키 + 이동키)
	if (m_Moveset.m_Down_Battle_Push)
	{
		m_Moveset.m_Down_Battle_Push = false;

		m_isThrowing = true;
		m_pModelCom->Set_Animation(ANIM_BATTLE_GUARD_PUSH);
	}
	if (m_isThrowing && m_pModelCom->Get_iCurrentAnimIndex() == ANIM_BATTLE_IDLE && m_Moveset.m_State_Battle_Guard)
	{
		m_isMaintain_Guard = true;
	}

}

void CPlayer_Tanjiro::Animation_Control_Battle_Dash(_double dTimeDelta)
{
	if (m_Moveset.m_Down_Battle_Dash)
	{
		m_Moveset.m_Down_Battle_Dash = false;

		m_pModelCom->Set_Animation(ANIM_BATTLE_DASH);
	}
	Go_Straight_Constant(dTimeDelta, 80, 3.0f);


	if (m_Moveset.m_Down_Battle_Step)
	{
		m_Moveset.m_Down_Battle_Step = false;

		m_pTransformCom->Set_Look(m_vLook);
		if(m_isForward)
			m_pModelCom->Set_Animation(ANIM_BATTLE_STEP_F);
		else if(m_isBack)
			m_pModelCom->Set_Animation(ANIM_BATTLE_STEP_B);
		else
		{
			if (m_isLeft)
			{
				//콤보 첫 애니메이션 설정
				if (m_pModelCom->Get_Combo_Doing() == false)
				{
					m_pModelCom->Set_Combo_Doing(true);
					m_pModelCom->Set_Animation(ANIM_BATTLE_STEP_L);
				}
				//아닐경우, 다음 콤보로 진행
				else
				{
					m_pModelCom->Set_Combo_Trigger(true);
					Jumping(3.0f, 0.25f);
				}
			}
			else if (m_isRight)
			{
				//콤보 첫 애니메이션 설정
				if (m_pModelCom->Get_Combo_Doing() == false)
				{
					m_pModelCom->Set_Combo_Doing(true);
					m_pModelCom->Set_Animation(ANIM_BATTLE_STEP_R);
				}
				//아닐경우, 다음 콤보로 진행
				else
				{
					m_pModelCom->Set_Combo_Trigger(true);
					Jumping(3.0f, 0.25f);
				}
			}
		}
	}
	_vector vDir = XMLoadFloat4(&m_Moveset.m_Input_Dir);
	_float4 fDir;
	XMStoreFloat4(&fDir, -vDir);
	Go_Dir_Deceleration(dTimeDelta, ANIM_BATTLE_STEP_F, 4.5f, 0.15f, m_Moveset.m_Input_Dir);
	Go_Dir_Deceleration(dTimeDelta, ANIM_BATTLE_STEP_B, 4.5f, 0.15f, m_Moveset.m_Input_Dir);
	Go_Dir_Deceleration(dTimeDelta, ANIM_BATTLE_STEP_L, 4.5f, 0.15f, m_Moveset.m_Input_Dir);
	Go_Dir_Deceleration(dTimeDelta, ANIM_BATTLE_STEP_R, 4.5f, 0.15f, m_Moveset.m_Input_Dir);

	//더블스텝
	Go_Dir_Deceleration(dTimeDelta, 98, 5.0f, 0.15f, m_Moveset.m_Input_Dir);
	Go_Dir_Deceleration(dTimeDelta, 100, 5.0f, 0.15f, m_Moveset.m_Input_Dir);
}

void CPlayer_Tanjiro::Animation_Control_Battle_Awaken(_double dTimeDelta)
{
	if (m_Moveset.m_Down_Battle_Awaken)
	{
		m_Moveset.m_Down_Battle_Awaken = false;

		if (m_Moveset.m_iAwaken == 1)
		{
			m_pModelCom->Set_Animation(ANIM_BATTLE_AWAKEN);
		}
		else if (m_Moveset.m_iAwaken == 2)
		{
			m_pModelCom->Set_Animation(ANIM_BATTLE_AWAKEN_COMPLETE_CUTSCENE);
		}
	}
}

void CPlayer_Tanjiro::Animation_Control_Battle_Special(_double dTimeDelta)
{
	if (m_Moveset.m_Down_Battle_Special)
	{
		m_Moveset.m_Down_Battle_Special = false;

		m_pModelCom->Set_Animation(ANIM_ATK_SPECIAL_READY);
		m_dTime_Special_Ready = 0.0;
	}
	m_dTime_Special_Ready += dTimeDelta;
	if(m_dTime_Special_Ready >1.5f)
		Go_Straight_Deceleration(dTimeDelta, 109, 4.0f, 0.23f);
	
	//Go_Straight_Constant(dTimeDelta, 108, 2.7f);
	if (m_pModelCom->Get_iCurrentAnimIndex() == 108 || m_pModelCom->Get_iCurrentAnimIndex() == 109)
	{
		if (m_isSpecialHit)
		{
			m_isSpecialHit = false;
			m_isFirst_Special_Jump = true;

			m_pModelCom->Set_Animation(ANIM_ATK_SPECIAL_CUTSCENE);
			m_dTime_Special_CutScene = 0.0;
		}
	}
	m_dTime_Special_CutScene += dTimeDelta;

	Go_Straight_Constant(dTimeDelta, ANIM_ATK_SPECIAL_CUTSCENE, 1.0f);
	if (0.90f <= m_dTime_Special_CutScene)
	{
		Go_Left_Deceleration(dTimeDelta, ANIM_ATK_SPECIAL_CUTSCENE, 10.0f, 0.1f);
	}
	else if (0.65f <= m_dTime_Special_CutScene)
	{
		Go_Right_Deceleration(dTimeDelta, ANIM_ATK_SPECIAL_CUTSCENE, 10.0f, 0.1f);
	}
	else if (0.35f <= m_dTime_Special_CutScene )
	{
		Go_Left_Deceleration(dTimeDelta, ANIM_ATK_SPECIAL_CUTSCENE, 10.0f, 0.1f);
	}

	if (m_pModelCom->Get_iCurrentAnimIndex() == 103 && m_isFirst_Special_Jump)
	{
		m_isFirst_Special_Jump = false;
		Jumping(2.65f, 0.025f);
	}
	
	if (m_pModelCom->Get_iCurrentAnimIndex() == 106 && m_isSecond_Special_Jump)
	{
		m_isSecond_Special_Jump = false;
		Jumping(1.0f, 0.08f);
	}
	Go_Straight_Deceleration(-dTimeDelta, 106, 1.0f, 0.01f);
}

void CPlayer_Tanjiro::Animation_Control_Battle_Dmg(_double dTimeDelta)
{
	if (m_Moveset.m_Down_Dmg_Small)
	{
		m_Moveset.m_Down_Dmg_Small = false;

		m_pModelCom->Set_Animation(ANIM_DMG_SMALL);
		m_Moveset.m_isHitMotion = true;
	}
	if (m_pModelCom->Get_AnimFinish(ANIM_DMG_SMALL))
	{
		m_Moveset.m_isHitMotion = false;
	}


	
}

void CPlayer_Tanjiro::Moving_Restrict()
{
	_int iCurAnimIndex = m_pModelCom->Get_iCurrentAnimIndex();

	//콤보공격시 무빙제한
	if (ANIM_ATK_COMBO == iCurAnimIndex
		|| 22 == iCurAnimIndex || 23 == iCurAnimIndex
		|| 24 == iCurAnimIndex || 25 == iCurAnimIndex || 26 == iCurAnimIndex
		|| 27 == iCurAnimIndex || 28 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
	}
	//점프 콤보공격시 제한
	else if (ANIM_ATK_AIRCOMBO == iCurAnimIndex || 30 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_Jump = true;
		m_Moveset.m_isRestrict_JumpCombo = true;
	}
	//차지공격 시 무빙제한
	else if (ANIM_ATK_CHARGE == iCurAnimIndex || 32 == iCurAnimIndex || 33 == iCurAnimIndex )
	{
		
		m_Moveset.m_isRestrict_Charge = true;
	}
	//스킬공격 시 무빙제한
	else if (ANIM_ATK_SKILL_GUARD == iCurAnimIndex || 35 == iCurAnimIndex || 36 == iCurAnimIndex || 37 == iCurAnimIndex
		|| ANIM_ATK_SKILL_MOVE == iCurAnimIndex || 39 == iCurAnimIndex || 40 == iCurAnimIndex || 41 == iCurAnimIndex
		|| ANIM_ATK_SKILL_NORMAL == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
	} 
	//잡기 공격 시 제한
	else if (ANIM_ATK_THROW == iCurAnimIndex )
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
		m_Moveset.m_isRestrict_Throw = true;
	}
	//점프 트랙 (이동키 + J키)공격 시 제한
	else if (ANIM_ATK_AIRTRACK == iCurAnimIndex || 50 == iCurAnimIndex || 51 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_Jump = true;
	}
	//Awaken
	else if (ANIM_BATTLE_AWAKEN == iCurAnimIndex || ANIM_BATTLE_AWAKEN_COMPLETE_CUTSCENE == iCurAnimIndex )
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
		m_Moveset.m_isRestrict_Jump = true;
		m_Moveset.m_isRestrict_Charge = true;
		m_Moveset.m_isRestrict_Step = true;
		m_Moveset.m_isRestrict_Dash = true;
	}
	//Special
	else if (ANIM_ATK_SPECIAL_CUTSCENE == iCurAnimIndex || 102 == iCurAnimIndex || 103 == iCurAnimIndex || 104 == iCurAnimIndex || 105 == iCurAnimIndex || 106 == iCurAnimIndex
		|| ANIM_ATK_SPECIAL_READY == iCurAnimIndex || 108 == iCurAnimIndex || 109 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
		m_Moveset.m_isRestrict_Jump = true;
		m_Moveset.m_isRestrict_Charge = true;
		m_Moveset.m_isRestrict_Step = true;
		m_Moveset.m_isRestrict_Dash = true;
		m_Moveset.m_isRestrict_Special = true;
	}
	//가드 시 제한
	else if (ANIM_BATTLE_GUARD == iCurAnimIndex || 64 == iCurAnimIndex || 65 == iCurAnimIndex
		|| ANIM_BATTLE_GUARD_HIT_BIG == iCurAnimIndex || ANIM_BATTLE_GUARD_HIT_SMALL == iCurAnimIndex || ANIM_BATTLE_GUARD_PUSH == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
	}
	//대시 시 제한
	else if (ANIM_BATTLE_DASH == iCurAnimIndex || 80 == iCurAnimIndex || 81 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
		m_Moveset.m_isRestrict_Dash = true;
	}
	//점프 시 무빙제한
	else if (ANIM_BATTLE_JUMP == iCurAnimIndex
		|| 84 == iCurAnimIndex || 85 == iCurAnimIndex || 86 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_Jump = true;
	}
	//스텝 시 제한
	else if (ANIM_BATTLE_STEP_AB == iCurAnimIndex || ANIM_BATTLE_STEP_AF == iCurAnimIndex || ANIM_BATTLE_STEP_AL == iCurAnimIndex || ANIM_BATTLE_STEP_AR == iCurAnimIndex
		|| ANIM_BATTLE_STEP_B == iCurAnimIndex || ANIM_BATTLE_STEP_F == iCurAnimIndex || ANIM_BATTLE_STEP_L == iCurAnimIndex || ANIM_BATTLE_STEP_R == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
		m_Moveset.m_isRestrict_Step = true;
	}
	//더블스텝 시 제한
	else if ( 98 == iCurAnimIndex || 100 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
		m_Moveset.m_isRestrict_KeyInput = true;
		
		m_Moveset.m_isRestrict_DoubleStep = true;
	}
	//제한 해제d
	else
	{
		m_Moveset.m_isRestrict_Move = false;
		m_Moveset.m_isRestrict_KeyInput = false;
		m_Moveset.m_isRestrict_Jump = false;
		m_Moveset.m_isRestrict_JumpCombo = false;
		m_Moveset.m_isRestrict_Throw = false;
		m_Moveset.m_isRestrict_Charge = false;
		m_Moveset.m_isRestrict_Dash = false;
		m_Moveset.m_isRestrict_Step = false;
		m_Moveset.m_isRestrict_DoubleStep = false;
		m_Moveset.m_isRestrict_Special = false;
	}
}

HRESULT CPlayer_Tanjiro::Add_Components()
{
	/* for.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tanjiro"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSG_BOX("Failed to Add_Com_Model : CPlayer_Tanjiro");
		return E_FAIL;
	}

	/* for.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSG_BOX("Failed to Add_Com_Shader : CPlayer_Tanjiro");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer_Tanjiro::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_float4x4 ViewMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;

	_float4x4 ProjMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;


	// OutlineThickness
	if (FAILED(m_pShaderCom->SetUp_RawValue("g_OutlineThickness", &m_fOutlineThickness, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->SetUp_RawValue("g_OutlineFaceThickness", &m_fOutlineFaceThickness, sizeof(_float))))
		return E_FAIL;





	Safe_Release(pGameInstance);

	return S_OK;
}

CPlayer_Tanjiro* CPlayer_Tanjiro::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Tanjiro* pInstance = new CPlayer_Tanjiro(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Tanjiro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Tanjiro::Clone(void* pArg)
{
	CPlayer_Tanjiro* pInstance = new CPlayer_Tanjiro(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Tanjiro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Tanjiro::Free()
{
	__super::Free();
}
