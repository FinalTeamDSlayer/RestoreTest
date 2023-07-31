#include "pch.h"
#include "..\Public\Loader_Tool.h"

#include "GameInstance.h"
#include "ImGui_Manager_Tool.h"

#include "Camera_Tool.h"
#include "Player_Tool.h"
#include "Terrain_Tool.h"

//AnimTool¿ë
#include "AnimCharacter_Tool.h"
#include "ImGui_Animation_Tool.h"

//MapObject
#include "StaticMapObject.h"
#include "TerrainMapObject.h"
#include "RotationMapObject.h"
#include "InstanceMapObject.h"
#include "Sky.h"

CLoader_Tool::CLoader_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Loading_Main(void* pArg)
{
	CLoader_Tool* pLoader = (CLoader_Tool*)pArg;

	CoInitializeEx(nullptr, 0);
	 
	EnterCriticalSection(pLoader->Get_CS());

	HRESULT hr = 0;
	
	hr = pLoader->LoadingForAllStage();

	switch (pLoader->Get_LevelID())
	{
	case LEVEL_TOOL:
		hr = pLoader->LoadingForTool(CImGui_Manager_Tool::GetInstance()->Get_IsLoad(LEVEL_TOOL));
		break;
	case LEVEL_VILLAGE:
		hr = pLoader->LoadingForVillage(false == CImGui_Manager_Tool::GetInstance()->Get_IsLoad(LEVEL_VILLAGE));
		break;
	case LEVEL_HOUSE:
		hr = pLoader->LoadingForHouse(false == CImGui_Manager_Tool::GetInstance()->Get_IsLoad(LEVEL_HOUSE));
		break;
	case LEVEL_TRAIN:
		hr = pLoader->LoadingForTrain(false == CImGui_Manager_Tool::GetInstance()->Get_IsLoad(LEVEL_TRAIN));
		break;
	case LEVEL_FINALBOSS:
		hr = pLoader->LoadingForFinalBoss(false == CImGui_Manager_Tool::GetInstance()->Get_IsLoad(LEVEL_FINALBOSS));
		break;
	}

	if (FAILED(hr))
		return 1;

	LeaveCriticalSection(pLoader->Get_CS());

	return 0;
}

HRESULT CLoader_Tool::Initialize(LEVELID eLevelID)
{
	m_eLevelID = eLevelID;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Loading_Main, this, 0, nullptr);
	if(0 == m_hThread)
	{
		MSG_BOX("Failed to BeginThread");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader_Tool::LoadingForAllStage()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	if (false == pGameInstance->Get_IsLoadForAll())
	{
		/* Prototype_Component_Shader_VtxTerrainModel */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxTerrainModel"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/ShaderFiles/Shader_VtxTerrainModel.hlsl"), VTXMODEL_DECL::Elements, VTXMODEL_DECL::iNumElements))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxTerrainModel");
			return E_FAIL;
		}

		/* Prototype_Component_Shader_VtxModelInstance */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxModelInstance"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/ShaderFiles/Shader_VtxModelInstance.hlsl"), VTXMODELINSTANCE_DECL::Elements, VTXMODELINSTANCE_DECL::iNumElements))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxModelInstance");
			return E_FAIL;
		}

		/*==========================================================================*/

		Load_MapObjectModel_AllStage(pGameInstance);
	}
	else
	{

	}

	pGameInstance->Set_IsLoadForAll();

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader_Tool::LoadingForLogo(_bool isLoad)
{
	return S_OK;
}

HRESULT CLoader_Tool::LoadingForTool(_bool isLoad)
{
	SetWindowText(g_hWnd, TEXT("LoadingForTool"));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (false == isLoad)
	{

#pragma region COMPONENTS

		SetWindowText(g_hWnd, TEXT("Loading Texture..."));
#pragma region Texture

#pragma region EnvironmentTexture
	
#pragma endregion

#pragma region RampTexture

#pragma endregion

#pragma region EffectTexture

#pragma endregion

#pragma region UITexture

#pragma endregion


#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Model..."));
#pragma region Model

#pragma region Buffer

#pragma endregion
		_matrix		PivotMatrix = XMMatrixIdentity();

#pragma region Effect

#pragma endregion

#pragma region Character
		/* Prototype_Component_Model_Tanjiro */
		PivotMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Tanjiro"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Client/Bin/Resources/Models/Character/Tanjiro/Tanjiro.bin", PivotMatrix))))
		{
			MSG_BOX("Failed to Add_Prototype_Model_Tanjiro");
			return E_FAIL;
		}

		/* Prototype_Component_Model_Akaza */
		PivotMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Akaza"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../Client/Bin/Resources/Models/Character/Akaza/Akaza.bin", PivotMatrix))))
		{
			MSG_BOX("Failed to Add_Prototype_Model_Akaza");
			return E_FAIL;
		}

#pragma endregion

#pragma region NonCharacter


#pragma endregion

#pragma region Terrain

		Load_MapObjectModel_FinalBoss();

		/* Prototype_Component_Navigation */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Navigation"),
			CNavigation::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Data/Navigation.dat")))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Navigation");
			return E_FAIL;
		}

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Shader..."));
#pragma region Shader
		/* Prototype_Component_Shader_VtxNorTex */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECL::Elements, VTXNORTEX_DECL::iNumElements))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxNorTex");
			return E_FAIL;
		}

		/* Prototype_Component_Shader_VtxModel */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxModel"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL_DECL::Elements, VTXMODEL_DECL::iNumElements))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxModel");
			return E_FAIL;
		}

		/* Prototype_Component_Shader_VtxAnimModel */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxAnimModel"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECL::Elements, VTXANIMMODEL_DECL::iNumElements))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxAnimModel");
			return E_FAIL;
		}
#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading ETC..."));
#pragma region Etc
		/* Prototype_Component_Collider_AABB */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Collider_AABB"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Collider_AABB");
			return E_FAIL;
		}

		/* Prototype_Component_Collider_OBB */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Collider_OBB"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Collider_OBB");
			return E_FAIL;
		}

		/* Prototype_Component_Collider_Sphere */
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Collider_Sphere"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		{
			MSG_BOX("Failed to Add_Prototype_Component_Collider_Sphere");
			return E_FAIL;
		}


#pragma endregion


#pragma endregion

#pragma region GAMEOBJECTS

		SetWindowText(g_hWnd, TEXT("Loading GameObject..."));

		CImGui_Manager_Tool* pGUI = CImGui_Manager_Tool::GetInstance();
		Safe_AddRef(pGUI);

#pragma region Object
		/* Prototype_GameObject_Camera_Tool */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Tool"),
			CCamera_Tool::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_Camera_Tool");
			return E_FAIL;
		}

		/* Prototype_GameObject_Player_Tool */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Tool"),
			CPlayer_Tool::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_Player_Tool");
			return E_FAIL;
		}

		//AnimTool¿ë
		/* Prototype_GameObject_AnimCharacter_Tool */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AnimCharacter_Tool"),
			CAnimCharacter_Tool::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_AnimCharacter_Tool");
			return E_FAIL;
		}
#pragma endregion

#pragma region Environment
		/* For.Prototype_GameObject_StaticMapObject */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticMapObject"),
			CStaticMapObject::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_StaticMapObject");
			return E_FAIL;
		}

		/* For.Prototype_GameObject_TerrainMapObject */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TerrainMapObject"),
			CTerrainMapObject::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_TerrainMapObject");
			return E_FAIL;
		}

		/* For.Prototype_GameObject_RotationMapObject */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RotationMapObject"),
			CRotationMapObject::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_RotationMapObject");
			return E_FAIL;
		}

		/* For.Prototype_GameObject_InstanceMapObject */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InstanceMapObject"),
			CInstanceMapObject::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_InstanceMapObject");
			return E_FAIL;
		}

		/* For.Prototype_GameObject_Sky */
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
			CSky::Create(m_pDevice, m_pContext))))
		{
			MSG_BOX("Failed to Add_Prototype_GameObject_Sky");
			return E_FAIL;
		}
#pragma endregion

#pragma region UI

#pragma endregion

#pragma region Particale

#pragma endregion

#pragma region Effect

#pragma endregion


		Safe_Release(pGUI);
#pragma endregion

	}

	Safe_Release(pGameInstance);

	SetWindowText(g_hWnd, TEXT("Loading Finished!!"));
	m_isFinished = true;

	CImGui_Manager_Tool::GetInstance()->Set_IsLoad(LEVEL_TOOL);

	return S_OK;
}

HRESULT CLoader_Tool::LoadingForVillage(_bool isLoad)
{
	SetWindowText(g_hWnd, TEXT("LoadingForVillage"));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (false == isLoad)
	{

#pragma region COMPONENTS

		SetWindowText(g_hWnd, TEXT("Loading Texture..."));
#pragma region Texture

#pragma region EnvironmentTexture

#pragma endregion

#pragma region RampTexture

#pragma endregion

#pragma region EffectTexture

#pragma endregion

#pragma region UITexture

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Model..."));
#pragma region Model

#pragma region Buffer

#pragma endregion
		_matrix		PivotMatrix = XMMatrixIdentity();
#pragma region Effect

#pragma endregion

#pragma region Character

#pragma endregion

#pragma region NonCharacter

#pragma endregion

#pragma region Terrain

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Shader..."));
#pragma region Shader

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading ETC..."));
#pragma region Etc

#pragma endregion

#pragma endregion

#pragma region GAMEOBJECTS

		SetWindowText(g_hWnd, TEXT("Loading GameObject..."));
#pragma region Object

#pragma endregion

#pragma region Environment

#pragma endregion

#pragma region UI

#pragma endregion

#pragma region Particale

#pragma endregion

#pragma region Effect

#pragma endregion

#pragma endregion

	}

	Safe_Release(pGameInstance);

	SetWindowText(g_hWnd, TEXT("Loading Finished!!!"));
	m_isFinished = true;

	CImGui_Manager_Tool::GetInstance()->Set_IsLoad(LEVEL_VILLAGE);

	return S_OK;
}

HRESULT CLoader_Tool::LoadingForHouse(_bool isLoad)
{
	SetWindowText(g_hWnd, TEXT("LoadingForHouse"));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	if (false == isLoad)
	{

#pragma region COMPONENTS

		SetWindowText(g_hWnd, TEXT("Loading Texture..."));
#pragma region Texture

#pragma region EnvironmentTexture

#pragma endregion

#pragma region RampTexture

#pragma endregion

#pragma region EffectTexture

#pragma endregion

#pragma region UITexture

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Model..."));
#pragma region Model

#pragma region Buffer

#pragma endregion
		_matrix		PivotMatrix = XMMatrixIdentity();
#pragma region Effect

#pragma endregion

#pragma region Character

#pragma endregion

#pragma region NonCharacter

#pragma endregion

#pragma region Terrain
		Load_MapObjectModel_House();

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Shader..."));
#pragma region Shader

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading ETC..."));
#pragma region Etc

#pragma endregion

#pragma endregion

#pragma region GAMEOBJECTS

		SetWindowText(g_hWnd, TEXT("Loading GameObject..."));
#pragma region Object

#pragma endregion

#pragma region Environment

#pragma endregion

#pragma region UI

#pragma endregion

#pragma region Particale

#pragma endregion

#pragma region Effect

#pragma endregion

#pragma endregion

	}

	Safe_Release(pGameInstance);

	SetWindowText(g_hWnd, TEXT("Loading Finished!!!"));
	m_isFinished = true;

	CImGui_Manager_Tool::GetInstance()->Set_IsLoad(LEVEL_HOUSE);

	return S_OK;
}

HRESULT CLoader_Tool::LoadingForTrain(_bool isLoad)
{
	SetWindowText(g_hWnd, TEXT("LoadingForTrain"));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (false == isLoad)
	{

#pragma region COMPONENTS

		SetWindowText(g_hWnd, TEXT("Loading Texture..."));
#pragma region Texture

#pragma region EnvironmentTexture

#pragma endregion

#pragma region RampTexture

#pragma endregion

#pragma region EffectTexture

#pragma endregion

#pragma region UITexture

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Model..."));
#pragma region Model

#pragma region Buffer

#pragma endregion
		_matrix		PivotMatrix = XMMatrixIdentity();
#pragma region Effect

#pragma endregion

#pragma region Character

#pragma endregion

#pragma region NonCharacter

#pragma endregion

#pragma region Terrain

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Shader..."));
#pragma region Shader

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading ETC..."));
#pragma region Etc

#pragma endregion

#pragma endregion

#pragma region GAMEOBJECTS

		SetWindowText(g_hWnd, TEXT("Loading GameObject..."));
#pragma region Object

#pragma endregion

#pragma region Environment

#pragma endregion

#pragma region UI

#pragma endregion

#pragma region Particale

#pragma endregion

#pragma region Effect

#pragma endregion

#pragma endregion

	}

	Safe_Release(pGameInstance);

	SetWindowText(g_hWnd, TEXT("Loading Finished!!!"));
	m_isFinished = true;

	CImGui_Manager_Tool::GetInstance()->Set_IsLoad(LEVEL_TRAIN);

	return S_OK;
}

HRESULT CLoader_Tool::LoadingForFinalBoss(_bool isLoad)
{
	SetWindowText(g_hWnd, TEXT("LoadingForFinalBoss"));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (false == isLoad)
	{

#pragma region COMPONENTS

		SetWindowText(g_hWnd, TEXT("Loading Texture..."));
#pragma region Texture

#pragma region EnvironmentTexture

#pragma endregion

#pragma region RampTexture

#pragma endregion

#pragma region EffectTexture

#pragma endregion

#pragma region UITexture

#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Model..."));
#pragma region Model

#pragma region Buffer

#pragma endregion
		_matrix		PivotMatrix = XMMatrixIdentity();
#pragma region Effect

#pragma endregion

#pragma region Character

#pragma endregion

#pragma region NonCharacter

#pragma endregion

#pragma region Terrain
		
#pragma endregion

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading Shader..."));
#pragma region Shader

#pragma endregion

		SetWindowText(g_hWnd, TEXT("Loading ETC..."));
#pragma region Etc

#pragma endregion

#pragma endregion

#pragma region GAMEOBJECTS

		SetWindowText(g_hWnd, TEXT("Loading GameObject..."));
#pragma region Object

#pragma endregion

#pragma region Environment

#pragma endregion

#pragma region UI

#pragma endregion

#pragma region Particale

#pragma endregion

#pragma region Effect

#pragma endregion

#pragma endregion

	}

	Safe_Release(pGameInstance);

	SetWindowText(g_hWnd, TEXT("Loading Finished!!!"));
	m_isFinished = true;

	CImGui_Manager_Tool::GetInstance()->Set_IsLoad(LEVEL_FINALBOSS);

	return S_OK;
}

HRESULT CLoader_Tool::Load_MapObjectModel_AllStage(CGameInstance* pGameInstance)
{
	_matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	// ===========Texture=============
	/* For.Prototype_Component_Texture_TerrainMask*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Filter.jpg")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Splating*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Splating"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/T_d_114_RiceField_01a_BC.dds")))))
		return E_FAIL;

	// ===========Model=============
	/* For.Prototype_Component_ModelInstance_114_Grass_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_114_Grass_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Grass_01a.bin", PivotMatrix, 100))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_114_Grass_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_114_Grass_02a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Grass_02a.bin", PivotMatrix, 100))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_114_Grass_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_114_Grass_03a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Grass_03a.bin", PivotMatrix, 100))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_GrassStrip_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_GrassStrip_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/GrassStrip_01a.bin", PivotMatrix, 50))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_114_Bush_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_114_Bush_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Bush_01a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_114_Seedlings_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_114_Seedlings_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Seedlings_01a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_114_SmallTree_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_114_SmallTree_03a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_SmallTree_03a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_006_MiddleTree_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_006_MiddleTree_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Trees/006_MiddleTree_01a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_006_MiddleTree_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_006_MiddleTree_02a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Trees/006_MiddleTree_02a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Coniferous_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Coniferous_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Trees/cmn_mount_Coniferous_01a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Coniferous_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Coniferous_02a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Trees/cmn_mount_Coniferous_02a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Coniferous_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Coniferous_03a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Trees/cmn_mount_Coniferous_03a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Coniferous_04a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Coniferous_04a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Trees/cmn_mount_Coniferous_04a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_01a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_01a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_02a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_02a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_03a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_03a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_03b*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_03b"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_03b.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_03c*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_03c"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_03c.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_04a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_04a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_04a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_05a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_05a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_05a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_06a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_06a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_06a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_09a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_09a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_09a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_10a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_10a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_10a.bin", PivotMatrix, 20))))
		return E_FAIL;

	/* For.Prototype_Component_ModelInstance_cmn_mount_Bush_12a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ModelInstance_cmn_mount_Bush_12a"),
		CModel_Instance::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Environments/Map/Bushes/cmn_mount_Bush_12a.bin", PivotMatrix, 20))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader_Tool::Load_MapObjectModel_House()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_matrix			PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	// =============== StaticMapObject==============//

	/* For.Prototype_Component_Model_CorridorWood_02b*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_CorridorWood_02b"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/CorridorWood_02b.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_CorridorWood_06a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_CorridorWood_06a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/CorridorWood_06a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Room_S_01*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Room_S_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Room_S_01.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Room_S_02*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Room_S_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Room_S_02.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Room_S_06*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Room_S_06"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Room_S_06.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorShoji_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorShoji_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorShoji_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorShoji_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorShoji_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorShoji_02a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorShoji_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorShoji_03a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorShoji_03a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorEntrance_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorEntrance_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorEntrance_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorFusuma_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorFusuma_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorFusuma_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorFusuma_01d*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorFusuma_01d"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorFusuma_01d.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorFusuma_01g*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorFusuma_01g"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorFusuma_01g.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorFusuma_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorFusuma_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorFusuma_02a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorFusuma_02d*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorFusuma_02d"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorFusuma_02d.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DoorFusuma_02g*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_DoorFusuma_02g"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/DoorFusuma_02g.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Paper_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Paper_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Paper_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Chest_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Chest_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Chest_02a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Hibachi_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Hibachi_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Hibachi_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_RoomLight_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_RoomLight_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/RoomLight_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Lantern_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_Lantern_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/Lantern_01a.bin", PivotMatrix))))
		return E_FAIL;

	// ===============Environments/Map/RoomMap BossRoom======================
	/* For.Prototype_Component_Model_BossRoomBase*/
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_BossRoomBase"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/BossRoomBase.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_107_DoorWood_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_107_DoorWood_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/107_DoorWood_01a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_107_DoorWood_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_107_DoorWood_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/107_DoorWood_02a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WindowDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_WindowDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/WindowDoor.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WindowDoor_Small*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_WindowDoor_Small"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/WindowDoor_Small.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_WoodWall*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_WoodWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/WoodWall.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_AllWoodWall*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Model_AllWoodWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/RoomMap/AllWoodWall.bin", PivotMatrix))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader_Tool::Load_MapObjectModel_FinalBoss()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_matrix			PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));

	// ===============BattleMap Acaza======================
	/* For.Prototype_Component_Model_BattleMap_Acaza*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_BattleMap_Acaza"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/BattleMap_Acaza.bin", PivotMatrix))))
		return E_FAIL;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	/* For.Prototype_Component_Model_BrokenLocomotive_01*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_BrokenLocomotive_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/BrokenLocomotive_01.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_BrokenLocomotive_01c*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_BrokenLocomotive_01c"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/BrokenLocomotive_01c.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_BrokenLocomotive_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_BrokenLocomotive_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/BrokenLocomotive_02a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_BrokenLocomotive_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_BrokenLocomotive_03a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/BrokenLocomotive_03a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_BrokenLocomotive_04a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_BrokenLocomotive_04a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/BrokenLocomotive_04a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Sky*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_Sky"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Sky.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Far_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_Far_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Far_01a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Far_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_Far_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Far_02a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Far_04a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_Far_04a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Far_04a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_far_hill_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_far_hill_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/far_hill_01a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_far_hill_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_far_hill_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/far_hill_02a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_114_SmallCliff_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_SmallCliff_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_SmallCliff_01a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Rail_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_Rail_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Rail_01a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_Rail_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_Rail_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/Rail_02a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_114_Middle_01a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_01a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_01a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_02a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_02a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_02a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_02b*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_02b"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_02b.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_03a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_03a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_03a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_03b*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_03b"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_03b.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_04a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_04a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_04a.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_04b*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_04b"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_04b.bin", PivotMatrix))))
		return E_FAIL;
	/* For.Prototype_Component_Model_114_Middle_05a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_114_Middle_05a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Acaza_vs/114_Middle_05a.bin", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_006_Trees_04a*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_FINALBOSS, TEXT("Prototype_Component_Model_006_Trees_04a"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Environments/Map/Trees/006_Trees_04a.bin", PivotMatrix))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

CLoader_Tool* CLoader_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eLevelID)
{
	CLoader_Tool* pInstance = new CLoader_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eLevelID)))
	{
		MSG_BOX("Failed to Create : CLoader_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Tool::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
