#include "pch.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "AtkCollManager.h"
#include "ColliderManager.h"

#include "SoundMgr.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	CSoundMgr::Get_Instance()->Initialize();

	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	// D3D11_BLEND_DESC
	// D3D11_RASTERIZER_DESC
	// D3D11_DEPTH_STENCIL_DESC
	// D3D11_RASTERIZER_DESC

	//ID3D11BlendState*		pBlendState = { nullptr };

	//D3D11_BLEND_DESC		BlendDesc;

	//m_pDevice->CreateBlendState(&BlendDesc, &pBlendState);

	///* RenderState*/ 
	//m_pContext->OMSetBlendState(pBlendState);
	//m_pContext->OMSetDepthStencilState();
	//m_pContext->RSSetState();

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.eWinMode = GRAPHICDESC::MODE_WIN;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInstance, LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
	{
		MSG_BOX("Failed to Initialize_Engine");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Default"), TEXT("../Bin/Resources/Fonts/Bombardier.spritefont"))))
	{
		MSG_BOX("Failed to Add_Font");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_KR"), TEXT("../Bin/Resources/Fonts/Mabi.spritefont"))))
	{
		MSG_BOX("Failed to Add_Font");
		return E_FAIL;
	}

	if (FAILED(Ready_Prototype_Component_For_Static()))
	{
		MSG_BOX("Failed to Ready_Prototype_Component_For_Static");
		return E_FAIL;
	}

	if (FAILED(SetUp_StartLevel(LEVEL_LOGO)))
	{
		MSG_BOX("Failed to SetUp_StartLevel");
		return E_FAIL;
	}

	/*_tchar szBgm[MAX_PATH] = TEXT("BGM_Gurenge.mp3");
	CSoundMgr::Get_Instance()->PlayBGM(szBgm, 0.3f);*/

	return S_OK;
}

void CMainApp::Tick(_double dTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	m_pGameInstance->Tick_Engine(dTimeDelta);

#ifdef _DEBUG
	Key_Input(dTimeDelta);

	m_TimeAcc += dTimeDelta;
#endif
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
	{
		MSG_BOX("GameInstance or Renderer is null : MainApp");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(m_vTestBG)))
	{
		MSG_BOX("Failed to Clear_BackBuffef_View");
		return E_FAIL;
	}

	if(FAILED(m_pGameInstance->Clear_DepthStencil_View()))
	{
		MSG_BOX("Failed to Clear_DepthStencil_View");
		return E_FAIL;
	}

	if (FAILED(m_pRenderer->Draw_RenderObjects()))
		return E_FAIL;

#ifdef _DEBUG
	++m_iRenderCnt;

	if (m_TimeAcc >= 1.0)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iRenderCnt);
		m_iRenderCnt = 0;
		m_TimeAcc = 0.0;
	}

	if (m_isRenderDebugInfo) {

		if (true == m_isRenderFPS)
		{
			if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_Default"), m_szFPS, _float2(0.f, 0.f), _float2(0.5f, 0.5f))))
				return E_FAIL;
		}

		if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("Num0 To OnOff StaticCam"), _float2(0.f, 620.f), _float2(0.5f, 0.5f))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("Num9 To OnOff LockMouse"), _float2(0.f, 640.f), _float2(0.5f, 0.5f))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("F6 To OnOff FPS"), _float2(0.f, 660.f), _float2(0.5f, 0.5f))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("F7 To OnOff RenderDebut"), _float2(0.f, 680.f), _float2(0.5f, 0.5f))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Draw_Font(TEXT("Font_KR"), TEXT("F8 To OnOff RenderTarget"), _float2(0.f, 700.f), _float2(0.5f, 0.5f))))
			return E_FAIL;

	}
	
#endif // _DEBUG

	if(FAILED(m_pGameInstance->Present()))
	{
		//MSG_BOX("Failed to Present");
		return E_FAIL;
	}

	return S_OK;
}


#ifdef _DEBUG
void CMainApp::Key_Input(_double dTimeDelta)
{
	HRESULT hr = 0;
	if (true == m_pGameInstance->Get_IsStage())
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_LCONTROL) & 0x80)
		{
			if (m_pGameInstance->Get_DIKeyDown(DIK_1))
			{
				if (nullptr == m_pGameInstance->Get_LoadedStage(LEVEL_LOGO))
					hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOGO), false, false);
				else
					hr = m_pGameInstance->Swap_Level(LEVEL_LOGO);
			}

			if (m_pGameInstance->Get_DIKeyDown(DIK_2))
			{
				if (nullptr == m_pGameInstance->Get_LoadedStage(LEVEL_GAMEPLAY))
					hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY), false, false);
				else
					hr = m_pGameInstance->Swap_Level(LEVEL_GAMEPLAY);
			}

			if (m_pGameInstance->Get_DIKeyDown(DIK_3))
			{
				if (nullptr == m_pGameInstance->Get_LoadedStage(LEVEL_VILLAGE))
					hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_VILLAGE), false, false);
				else
					hr = m_pGameInstance->Swap_Level(LEVEL_VILLAGE);
			}

			if (m_pGameInstance->Get_DIKeyDown(DIK_4))
			{
				if (nullptr == m_pGameInstance->Get_LoadedStage(LEVEL_HOUSE))
					hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_HOUSE), false, false);
				else
					hr = m_pGameInstance->Swap_Level(LEVEL_HOUSE);
			}

			if (m_pGameInstance->Get_DIKeyDown(DIK_5))
			{
				if (nullptr == m_pGameInstance->Get_LoadedStage(LEVEL_TRAIN))
					hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_TRAIN), false, false);
				else
					hr = m_pGameInstance->Swap_Level(LEVEL_TRAIN);
			}

			if (m_pGameInstance->Get_DIKeyDown(DIK_6))
			{
				if (nullptr == m_pGameInstance->Get_LoadedStage(LEVEL_FINALBOSS))
					hr = m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_FINALBOSS), false, false);
				else
					hr = m_pGameInstance->Swap_Level(LEVEL_FINALBOSS);
			}
		}
	}


	if (m_pGameInstance->Get_DIKeyDown(DIK_F6))
		m_isRenderFPS = !m_isRenderFPS;

	if (m_pGameInstance->Get_DIKeyDown(DIK_F7))
		m_pRenderer->OnOff_RenderDebug();

	if (m_pGameInstance->Get_DIKeyDown(DIK_F8))
		m_pRenderer->OnOff_RenderTarget();
	
	if (m_pGameInstance->Get_DIKeyDown(DIK_TAB))
		m_isRenderDebugInfo = !m_isRenderDebugInfo;

}
#endif // _DEBUG


HRESULT CMainApp::Ready_Prototype_Component_For_Static()
{
	if (nullptr == m_pGameInstance)
	{
		MSG_BOX("GameInstance is null : MainApp");
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
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
				CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECL::Elements, VTXTEX_DECL::iNumElements))))
	{
		MSG_BOX("Failed to Add_Prototype_Component_Shader_VtxTex");
		return E_FAIL;
	}

	/* Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.png"), 3))))
	{
		MSG_BOX("Failed to Add_Prototype_Component_Texture_BackGround");
		return E_FAIL;
	}

	/* Prototype_Component_Texture_UI */

#pragma region Player_Battle_Bar_UI	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Bar/P/P_Frame.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Face"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Face/P/C_%d.png"), 5))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Name"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Name/P/N_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Hp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Bar/P/Hp_%d.png"), 4))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Mp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Bar/P/Mp_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Ult_Frame_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Stack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Ult_Stack_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Bar_Color"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Ult_Bar_Color_0.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Combo_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Combo/Combo_Frame_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Num_Combo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Combo/Num_Combo%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Stack_Num"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Ult_Num_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Stock_Eff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Eff/Ult_Stock_Eff_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Awk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Eff/Eff_Awk_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Swk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Eff/Eff_Swk_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Flower"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Ult_Gauge_Flower.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Lang_Ult"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Lang_Ult_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Eff_Gauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Ult_Bar/Eff/Eff_Gauge_%d.png"), 2))))
		return E_FAIL;

#pragma endregion

#pragma region Boss_Battle_Bar_UI	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Bar/B/B_Frame.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Face"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Face/B/C_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Name"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Name/B/N_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Hp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Bar/B/Hp_%d.png"), 4))))
		return E_FAIL;


#pragma endregion


	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::SetUp_StartLevel(LEVELID eLevelID)
{
	if (LEVEL_STATIC == eLevelID || 
		LEVEL_LOADING == eLevelID)
	{
		MSG_BOX("Out of Range : LEVELID");
		return E_FAIL;
	}

	CLevel* pLevel = CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID);
	if (nullptr == pLevel)
	{
		MSG_BOX("Level is null : MainApp");
		return E_FAIL;
	}

	return m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel);
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);

	CSoundMgr::Get_Instance()->StopAll();

	CAtkCollManager::GetInstance()->DestroyInstance();
	CColliderManager::GetInstance()->DestroyInstance();
	CSoundMgr::Get_Instance()->Destroy_Instance();
	CGameInstance::Release_Engine();
}
