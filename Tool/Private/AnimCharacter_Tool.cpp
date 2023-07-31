#include "pch.h"
#include "..\Public\AnimCharacter_Tool.h"

#include "GameInstance.h"
#include "Animation.h"

#include "SoundMgr.h"



CAnimCharacter_Tool::CAnimCharacter_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter_Tool(pDevice, pContext)
	, m_pImGui_Anim(CImGui_Animation_Tool::GetInstance())
{
	Safe_AddRef(m_pImGui_Anim);
}

CAnimCharacter_Tool::CAnimCharacter_Tool(const CAnimCharacter_Tool& rhs)
	: CCharacter_Tool(rhs)
	, m_pImGui_Anim(rhs.m_pImGui_Anim)
{
}

HRESULT CAnimCharacter_Tool::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CAnimCharacter_Tool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	
	
	
		//여기서 
		char szFullPath[MAX_PATH] = { "" };
		//★ 수정 필요
		strcpy_s(szFullPath, "../../Client/Bin/Resources/AnimToolBin/Akaza.bin");

		ifstream fin;
		fin.open(szFullPath, ios::binary);
		if (false == fin.is_open())
		{
			MSG_BOX("Faield to open : Anim Data");
			//return E_FAIL;
		}

		_int AnimSize;
		fin.read(reinterpret_cast<char*>(&AnimSize), sizeof(_int));

		for (_int i = 0; i < AnimSize; i++)
		{
			CAnimation::CONTROLDESC ControlDesc;

			fin.read(reinterpret_cast<char*>(&ControlDesc.m_fAnimationSpeed), sizeof(_float));
			fin.read(reinterpret_cast<char*>(&ControlDesc.m_iConnect_Anim), sizeof(_int));
			fin.read(reinterpret_cast<char*>(&ControlDesc.m_isCombo), sizeof(_bool));
			fin.read(reinterpret_cast<char*>(&ControlDesc.m_iConnect_ComboAnim), sizeof(_int));
			fin.read(reinterpret_cast<char*>(&ControlDesc.m_isRootAnimation), sizeof(_bool));

			_int isizeEvent;
			fin.read(reinterpret_cast<char*>(&isizeEvent), sizeof(_int));

			for (_int i = 0; i < isizeEvent; i++)
			{
				CAnimation::EVENTDESC EventDesc;
				fin.read(reinterpret_cast<char*>(&EventDesc.m_dTime), sizeof(_double));
				fin.read(reinterpret_cast<char*>(&EventDesc.m_isFirst), sizeof(_bool));

				ControlDesc.m_vecTime_Event.emplace_back(EventDesc);
			}

			m_pModelCom->Set_Animation_Control(i, ControlDesc);
		}

		fin.close();
	
	Safe_Release(pGameInstance);

	return S_OK;
}

void CAnimCharacter_Tool::Tick(_double dTimeDelta)
{
	__super::Tick(dTimeDelta);

	if (true == m_isDead)
		return;

	ImGUI_Control(dTimeDelta);

	KeyInput(dTimeDelta);

	m_pModelCom->Play_Animation(dTimeDelta);
	RootAnimation(dTimeDelta);

	Event_Call(dTimeDelta);
	
}

void CAnimCharacter_Tool::LateTick(_double dTimeDelta)
{
	__super::LateTick(dTimeDelta);


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;


	

#ifdef _DEBUG
	/*if (FAILED(m_pRendererCom->Add_DebugGroup(m_pNavigationCom)))
		return;*/
#endif 
}

HRESULT CAnimCharacter_Tool::Render()
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

		if (FAILED(m_pModelCom->Bind_ShaderBoneMatrices(i, m_pShaderCom, "g_BoneMatrices")))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		//if(m_iTest != i)
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CAnimCharacter_Tool::ImGUI_Control(_double dTimeDelta)
{
	//툴작업을 위한 첫 애니메이션 초기화
	if (m_isFirst_Name)
	{
		m_isFirst_Name = false;
		_int	index = 0;

		vector<CAnimation*> vecAnim = m_pModelCom->Get_vecAnimation();
		for (auto& pAnim : vecAnim)
		{
			// 애니메이션 리스트 목록 index
			char szFullName[MAX_PATH];
			sprintf_s(szFullName, MAX_PATH, "%d_", index);

			char szNameCombine[MAX_PATH] = "";
			strcpy_s(szNameCombine, (pAnim->Get_AnimationDesc()).m_szName);
			strcat_s(szFullName, szNameCombine);

			const char* szName = szFullName;
			size_t len = strlen(szName) + 1; // +1 for the null-terminator
			char* pNewName = new char[len];
			strcpy_s(pNewName, len, szName);

			m_vecName.emplace_back(pNewName);


			// Imgui 초기값 넣어주기.
			// ★Connect 초기값 넣어주기.
			//CAnimation::CONTROLDESC control = pAnim->Get_ControlDesc();
			//control.m_iConnect_Anim = index;
			//pAnim->Set_ControlDesc(control);


			index++;
		}
		m_pImGui_Anim->Set_vecName(m_vecName);
	}

	if (m_pImGui_Anim->Get_Signal_Change_Anim())
	{
		m_pImGui_Anim->Set_Signal_Change_Anim(false);

		m_iNumAnim = m_pImGui_Anim->Get_AnimIndex();
		m_pModelCom->Set_Animation(m_iNumAnim);
	}
	
	
	// ImGui에 현재 해당 애니메이션 데이터 넣기. 
	m_pImGui_Anim->Set_AnimIndex(m_pModelCom->Get_iCurrentAnimIndex());
	m_pImGui_Anim->Set_Animation(m_pModelCom->Get_Animation());


	// 메인 GUI 띄우기
	m_pImGui_Anim->Animation_ImGui_Main();

	//재생기능
	m_pModelCom->Set_isPlay(m_pImGui_Anim->Get_Play());


	//세이브
	if (m_pImGui_Anim->Get_Save())
	{
		m_pImGui_Anim->Set_Save(false);
		Save_Animations();
	}
}

void CAnimCharacter_Tool::RootAnimation(_double dTimeDelta)
{
	CAnimation* pAnim = m_pModelCom->Get_Animation();

	if (pAnim->Get_ControlDesc().m_isRootAnimation)
	{
		//애니메이션 시작시 첫 위치
		if (pAnim->Get_AnimationDesc().m_dTimeAcc == 0.0)
		{
			XMStoreFloat4(&m_Save_RootPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_Save_RootPos));
		}
		else
		{
			_float4 fPos;
			XMStoreFloat4(&fPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

			_float3 RootPosition = pAnim->Get_RootPosition();
			_float4x4 RootWorldConvert = m_pTransformCom->Get_WorldFloat4x4();
			_float3 FinalRootPos = { 0.0f, 0.0f, 0.0f };
			XMStoreFloat3(&FinalRootPos, XMVector3TransformCoord(XMLoadFloat3(&RootPosition), XMLoadFloat4x4(&RootWorldConvert)));

			_float4 Final = { -FinalRootPos.x * 0.01f, FinalRootPos.y * 0.01f , -FinalRootPos.z * 0.01f, 1.f };

			// 플레이어의 월드 위치를 기준으로 Root bone의 위치를 변화시킴
			_float4  SubPos = { m_Save_RootPos.x + Final.x, m_Save_RootPos.y + Final.y , m_Save_RootPos.z + Final.z , 1.f };

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&SubPos));
		}
	}
}

void CAnimCharacter_Tool::Save_Animations()
{
	//FileFind
	char FindFile[MAX_PATH] = { "" };
	WIN32_FIND_DATAA fdFind;
	//★
	HANDLE hFindOut = ::FindFirstFileA("../../Client/Bin/Resources/Character/Models/Akaza/*.bin", &fdFind);
	if (hFindOut != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fdFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char* filename = fdFind.cFileName;
				strcpy_s(FindFile, filename);
			}
		} while (::FindNextFileA(hFindOut, &fdFind));
		::FindClose(hFindOut);
	}

	char szFullPath[MAX_PATH] = { "" };

	strcpy_s(szFullPath, "../../Client/Bin/Resources/AnimToolBin/");
	strcat_s(szFullPath, FindFile);

	ofstream fout;
	fout.open(szFullPath, ios::binary);
	if (!fout.is_open())
	{
		MSG_BOX("Failed File Open");
	}


	vector<CAnimation*> pAnims = m_pModelCom->Get_vecAnimation();

	_int ModelAnimsSize = (_int)pAnims.size();
	fout.write(reinterpret_cast<const char*>(&ModelAnimsSize), sizeof(_int));

	for (auto& pAnim : pAnims)
	{
		CAnimation::CONTROLDESC ControlDesc = pAnim->Get_ControlDesc();

		fout.write(reinterpret_cast<const char*>(&ControlDesc.m_fAnimationSpeed), sizeof(_float));
		fout.write(reinterpret_cast<const char*>(&ControlDesc.m_iConnect_Anim), sizeof(_int));
		fout.write(reinterpret_cast<const char*>(&ControlDesc.m_isCombo), sizeof(_bool));
		fout.write(reinterpret_cast<const char*>(&ControlDesc.m_iConnect_ComboAnim), sizeof(_int));
		fout.write(reinterpret_cast<const char*>(&ControlDesc.m_isRootAnimation), sizeof(_bool));

		_int isizeEvent = (_int)ControlDesc.m_vecTime_Event.size();
		fout.write(reinterpret_cast<const char*>(&isizeEvent), sizeof(_int));

		for (auto& event : ControlDesc.m_vecTime_Event)
		{
			fout.write(reinterpret_cast<const char*>(&event.m_dTime), sizeof(_double));
			fout.write(reinterpret_cast<const char*>(&event.m_isFirst), sizeof(_bool));
		}
	}

	fout.close();
}

void CAnimCharacter_Tool::KeyInput(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyDown(DIK_UPARROW))
	{
		++m_iNumAnim;
		if (m_pModelCom->Get_NumAnims() <= m_iNumAnim)
			m_iNumAnim = m_pModelCom->Get_NumAnims() - 1;
		m_pModelCom->Set_Animation(m_iNumAnim);
	}
	else if (pGameInstance->Get_DIKeyDown(DIK_DOWNARROW))
	{
		if (0 < m_iNumAnim)
			--m_iNumAnim;
		if (0 > m_iNumAnim)
			m_iNumAnim = 0;
		m_pModelCom->Set_Animation(m_iNumAnim);
	}

	if (pGameInstance->Get_DIKeyDown(DIK_LEFTARROW))
	{
		m_iTest--;
	}
	else if (pGameInstance->Get_DIKeyDown(DIK_RIGHTARROW))
	{
		m_iTest++;
	}



	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD8))
	{
		m_pTransformCom->Go_Straight(dTimeDelta);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_NUMPAD5))
	{
		m_pTransformCom->Go_Backward(dTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_NUMPAD4))
	{
		m_pTransformCom->Go_Left(dTimeDelta);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_NUMPAD6))
	{
		m_pTransformCom->Go_Right(dTimeDelta);
	}


	//콤보공격 테스트
	if (pGameInstance->Get_DIKeyDown(DIK_NUMPAD7))
	{
		/*//첫 애니메이션 설정
		if (m_pModelCom->Get_Combo_Doing() == false)
		{
			m_pModelCom->Set_Combo_Doing(true);
			m_pModelCom->Set_Animation(1);
		}
		//아닐경우, 다음 콤보로 진행
		else
			m_pModelCom->Set_Combo_Trigger(true);*/

			m_pModelCom->Set_Combo_Trigger(true);
	}
	



	Safe_Release(pGameInstance);
}

_bool CAnimCharacter_Tool::EventCallProcess()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation();

	CAnimation::CONTROLDESC ControlDesc = pAnim->Get_ControlDesc();

	if (ControlDesc.m_isEventCall)
	{
		ControlDesc.m_isEventCall = false;

		pAnim->Set_ControlDesc(ControlDesc);

		return true;
	}

	return false;
}

void CAnimCharacter_Tool::Event_Call(_double dTimeDelta)
{
	if (EventCallProcess())
	{
#pragma region Combo_Attack
		if (21 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			m_iTest++;
		}
		if (22 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			m_iTest++;
		}
		if (23 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			m_iTest++;
		}
		if (25 == m_pModelCom->Get_iCurrentAnimIndex())
		{
			m_iTest++;
		}
#pragma endregion
	}
	m_pImGui_Anim->Set_iTest(m_iTest);
}


HRESULT CAnimCharacter_Tool::Add_Components()
{
	// for.Com_Model 
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Model_Akaza"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	{
		MSG_BOX("Failed to Add_Com_Model : CAnimCharacter_Tool");
		return E_FAIL;
	}

	// for.Com_Shader 
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	{
		MSG_BOX("Failed to Add_Com_Shader : CAnimCharacter_Tool");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAnimCharacter_Tool::SetUp_ShaderResources()
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

	Safe_Release(pGameInstance);

	return S_OK;
}

CAnimCharacter_Tool* CAnimCharacter_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAnimCharacter_Tool* pInstance = new CAnimCharacter_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAnimCharacter_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAnimCharacter_Tool::Clone(void* pArg)
{
	CAnimCharacter_Tool* pInstance = new CAnimCharacter_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAnimCharacter_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimCharacter_Tool::Free()
{
	__super::Free();


	Safe_Release(m_pImGui_Anim);

	for (auto& pName : m_vecName)
	{
		delete[] pName;
	}
	m_vecName.clear();
	
}
