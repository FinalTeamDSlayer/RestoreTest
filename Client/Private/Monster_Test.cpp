#include "pch.h"
#include "..\Public\Monster_Test.h"

#include "GameInstance.h"
#include "SoundMgr.h"


CMonster_Test::CMonster_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer(pDevice, pContext)
{
}

CMonster_Test::CMonster_Test(const CMonster_Test& rhs)
	: CPlayer(rhs)
{
}

HRESULT CMonster_Test::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Test::Initialize(void* pArg)
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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 140.f,0.f,120.f,1.f });

	return S_OK;
}

void CMonster_Test::Tick(_double dTimeDelta)
{
	//__super::Tick(dTimeDelta);
	
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

void CMonster_Test::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);

	

#ifdef _DEBUG
	/*if (FAILED(m_pRendererCom->Add_DebugGroup(m_pNavigationCom)))
		return;*/
#endif
}

HRESULT CMonster_Test::Render()
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

HRESULT CMonster_Test::Render_ShadowDepth()
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


void CMonster_Test::EventCall_Control(_double dTimeDelta)
{
	if (EventCallProcess())
	{
#pragma region Combo_Attack
		if (ANIM_ATK_COMBO == m_pModelCom->Get_iCurrentAnimIndex())
		{
			_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
			CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
		}
		if (22 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
			CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
		}
		if (23 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
			CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
		}
		if (25 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			_tchar szTest[MAX_PATH] = TEXT("TestSound.wav");
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_SLASH);
			CSoundMgr::Get_Instance()->PlaySound(szTest, CSoundMgr::PLAYER_SLASH, 0.9f);
		}
#pragma endregion
	}
}

void CMonster_Test::Animation_Control(_double dTimeDelta)
{
	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), dTimeDelta);

	Animation_Control_Battle_Move(dTimeDelta);
	Animation_Control_Battle_Attack(dTimeDelta);
}

void CMonster_Test::Animation_Control_Battle_Move(_double dTimeDelta)
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
	else
	{
		m_fMove_Speed -= 0.15f;
		if (m_fMove_Speed <= 0)
			m_fMove_Speed = 0.0f;
		m_pTransformCom->Go_Straight(dTimeDelta * m_fMove_Speed);
	}

	if (m_Moveset.m_Up_Battle_Run)
	{
		m_Moveset.m_Up_Battle_Run = false;
		m_pModelCom->Set_Animation(ANIM_BATTLE_RUN_END);
	}
}

void CMonster_Test::Animation_Control_Battle_Attack(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//공격시 무빙제한
	_int iCurAnimIndex = m_pModelCom->Get_iCurrentAnimIndex();
	if (ANIM_ATK_COMBO == iCurAnimIndex
		|| 22 == iCurAnimIndex || 23 == iCurAnimIndex 
		|| 24 == iCurAnimIndex || 25 == iCurAnimIndex || 26 == iCurAnimIndex 
		|| 27 == iCurAnimIndex || 28 == iCurAnimIndex)
	{
		m_Moveset.m_isRestrict_Move = true;
	}
	else
	{
		m_Moveset.m_isRestrict_Move = false;
	}


	// 콤보공격
	if (m_Moveset.m_Down_Battle_Combo)
	{
		m_Moveset.m_Down_Battle_Combo = false;

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


	//콤보 도중의 Transform 이동
	if (ANIM_ATK_COMBO == iCurAnimIndex)
	{
		m_pTransformCom->Go_Straight(dTimeDelta );
	}


	Safe_Release(pGameInstance);
}

HRESULT CMonster_Test::Add_Components()
{
	/* for.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tanjiro"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSG_BOX("Failed to Add_Com_Model : CMonster_Test");
		return E_FAIL;
	}

	/* for.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSG_BOX("Failed to Add_Com_Shader : CMonster_Test");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonster_Test::SetUp_ShaderResources()
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

CMonster_Test* CMonster_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Test* pInstance = new CMonster_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonster_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Test::Clone(void* pArg)
{
	CMonster_Test* pInstance = new CMonster_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster_Test");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Test::Free()
{
	__super::Free();
}
