#include "pch.h"
#include "..\Public\MainApp_Tool.h"
#include "GameInstance.h"

#include "Level_Loading_Tool.h"


CMainApp_Tool::CMainApp_Tool()
	: m_pGameInstance(CGameInstance::GetInstance())
	, m_pGUI(CImGui_Manager_Tool::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGUI);
}

HRESULT CMainApp_Tool::Initialize()
{
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GRAPHICDESC::MODE_WIN;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInstance, LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
	{
		MSG_BOX("Failed to Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Default"), TEXT("../../Client/Bin/Resources/Fonts/Bombardier.spritefont"))))
	{
		MSG_BOX("Failed to Add_Font");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_KR"), TEXT("../../Client/Bin/Resources/Fonts/Mabi.spritefont"))))
	{
		MSG_BOX("Failed to Add_Font");
		return E_FAIL;
	}

	if (FAILED(m_pGUI->Initialize_ImGui(m_pDevice, m_pContext)))
	{
		MSG_BOX("Failed to SetUp_ImGUI");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_Component_For_Static()))
	{
		MSG_BOX("Failed to Ready_Prototype_Component_For_Static");
		return E_FAIL;
	}

	if (FAILED(SetUp_StartLevel(LEVEL_TOOL)))
	{
		MSG_BOX("Failed to SetUp_StartLevel");
		return E_FAIL;
	}

	m_pRenderer->Add_RenderTarget(TEXT("MRT_GUI"), TEXT("Target_GUI"), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f));

	return S_OK;
}

void CMainApp_Tool::Tick(_double dTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

#ifdef _DEBUG
	if (m_pGameInstance->Get_DIKeyDown(DIK_F7))
		m_isRenderFPS = !m_isRenderFPS;

	if (m_pGameInstance->Get_DIKeyDown(DIK_F8))
		m_pRenderer->OnOff_RenderTarget();
	m_TimeAcc += dTimeDelta;
#endif

	m_pGUI->Tick_ImGui();

	m_pGameInstance->Tick_Engine(dTimeDelta);

	m_pGUI->ImGui_Set();

	m_pGUI->ImGUI_ShowDemo();
}

HRESULT CMainApp_Tool::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
	{
		MSG_BOX("GameInstance or Renderer is null : MainApp_Tool");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(m_vTestBG)))
	{
		MSG_BOX("Failed to Clear_BackBuffef_View");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
	{
		MSG_BOX("Failed to Clear_DepthStencil_View");
		return E_FAIL;
	}

	m_pRenderer->RegistCallBack<CImGui_Manager_Tool>(m_pGUI, &CImGui_Manager_Tool::Render_ImGui);

	if (FAILED(m_pRenderer->Draw_RenderObjects()))
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	++m_iRenderCnt;

	if (m_TimeAcc >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iRenderCnt);
		m_iRenderCnt = 0;
		m_TimeAcc = 0.0;
	}

	if (true == m_isRenderFPS)
	{
		if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_Default"), m_szFPS, _float2(0.f, 0.f), _float2(0.5f, 0.5f))))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("F7 To OnOff FPS"), _float2(0.f, 680.f), _float2(0.5f, 0.5f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("F8 To OnOff RenderDebug"), _float2(0.f, 700.f), _float2(0.5f, 0.5f))))
		return E_FAIL;
#endif

	if (FAILED(m_pGameInstance->Present()))
	{
		//MSG_BOX("Failed to Present");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp_Tool::Ready_Prototype_Component_For_Static()
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX("GameInstance is null : MainApp_Tool");
		return E_FAIL;
	}

	/* Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
	{
		MSG_BOX("Failed to Add_Prototype_Component_Transform");
		return E_FAIL;
	}

	/* Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
	{
		MSG_BOX("Failed to Add_Prototype_Component_Renderer");
		return E_FAIL;
	}

	/* Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
	{
		MSG_BOX("Failed to Add_Prototype_Component_VIBuffer_Rect");
		return E_FAIL;
	}

	/* Protoype_Component_Shader_VtxTex */
	/*if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECL::Elements, VTXTEX_DECL::iNumElements))))
	{
		MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxTex");
		return E_FAIL;
	}*/

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp_Tool::SetUp_StartLevel(LEVELID eLevelID)
{
	if (LEVEL_STATIC == eLevelID ||
		LEVEL_LOADING == eLevelID)
	{
		MSG_BOX("Out of Range : LEVELID");
		return E_FAIL;
	}

	CLevel* pLevel = CLevel_Loading_Tool::Create(m_pDevice, m_pContext, eLevelID);
	if (nullptr == pLevel)
	{
		MSG_BOX("Level is null : MainApp_Tool");
		return E_FAIL;
	}

	return m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel);
}



CMainApp_Tool* CMainApp_Tool::Create()
{
	CMainApp_Tool* pInstance = new CMainApp_Tool();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp_Tool::Free()
{
	m_pGUI->Release_ImGui();

	Safe_Release(m_pGUI);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	CImGui_Manager_Tool::DestroyInstance();
	CGameInstance::Release_Engine();
}
