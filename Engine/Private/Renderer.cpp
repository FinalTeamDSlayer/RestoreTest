#include "..\Public\Renderer.h"

#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "PipeLine.h"
#include "GameInstance.h"
#include "UI.h"


CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pTarget_Manager->Initialize(m_pDevice, m_pContext);

	_uint	iNumViewports = { 1 };
	D3D11_VIEWPORT	Viewport;
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	/* For.Target_Diffuse */
	_float4 vColor_Diffuse = { 1.f, 0.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, vColor_Diffuse)))
		return E_FAIL;
	/* For.Target_Normal */
	_float4 vColor_Normal = { 1.f, 1.f, 0.f, 1.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, vColor_Normal)))
		return E_FAIL;
	/* For.Target_Depth */
	_float4 vColor_Depth = { 0.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_Depth)))
		return E_FAIL;
	/* For.Target_Shade */
	_float4 vColor_Shade = { 0.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, vColor_Shade)))
		return E_FAIL;
	/* For.Target_Specular */
	_float4 vColor_Specular = { 0.f, 0.f, 0.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_UNORM, vColor_Specular)))
		return E_FAIL;

	/* For.Target_ShadowDepth */
	_uint		iShadowMapCX = 12800;
	_uint		iShadowMapCY = 7200;
	_float4 vColor_ShadowDepth = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"), iShadowMapCX, iShadowMapCY, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_ShadowDepth)))
		return E_FAIL;

	///* For.Target_SSAO */
	_float4 vColor_SSAO = { 1.f, 1.f, 1.f, 1.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAO")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, vColor_SSAO)))
		return E_FAIL;

	///* For.Target_BlurX */
	_float4 vColor_BlurX = { 1.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurX")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_BlurX)))
		return E_FAIL;

	///* For.Target_BlurY */
	_float4 vColor_BlurY = { 1.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurY")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_BlurY)))
		return E_FAIL;

	///* For.Target_CombineBlur */
	_float4 vColor_Blur = { 1.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_CombineBlur")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_Blur)))
		return E_FAIL;

	///* For.Target_SSAOBlurX */
	//_float4 vColor_BlurX = { 1.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAOBlurX")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_BlurX)))
		return E_FAIL;

	///* For.Target_SSAOBlurY */
	//_float4 vColor_BlurY = { 1.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAOBlurY")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_BlurY)))
		return E_FAIL;

	///* For.Target_SSAOBlur */
	_float4 vColor_SSAOBlur = { 1.f, 1.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAOBlur")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_SSAOBlur)))
		return E_FAIL;

	///* For.Target_ShadowBlurX */
	//_float4 vColor_BlurX = { 1.f, 1.f, 1.f, 0.f };
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowBlurX")
	//	, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_BlurX)))
	//	return E_FAIL;

	/////* For.Target_ShadowBlurY */
	////_float4 vColor_BlurY = { 1.f, 1.f, 1.f, 0.f };
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowBlurY")
	//	, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_BlurY)))
	//	return E_FAIL;

	/////* For.Target_ShadowBlur */
	////_float4 vColor_SSAOBlur = { 1.f, 1.f, 1.f, 1.f };
	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowBlur")
	//	, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_SSAOBlur)))
	//	return E_FAIL;

	/* For.Target_ExportFinal */
	_float4 vColor_ExportTexture = { 0.f, 0.f, 0.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ExportFinal")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_ExportTexture)))
		return E_FAIL;

	/* For.Target_HDR */
	_float4 vColor_HDRTexture = { 0.f, 0.f, 0.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_HDR")
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, vColor_HDRTexture)))
		return E_FAIL;


	/* For.MRT_GameObject */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObject"), TEXT("Target_Depth"))))
		return E_FAIL;


	/* For.MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	// For.MRT_ShadowDepth(Shadow)
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;

	// For.MRT_SSAO
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAO"), TEXT("Target_SSAO"))))
		return E_FAIL;

	// For.MRT_Blur
	// 
	//최종블러
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurX"), TEXT("Target_BlurX"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurY"), TEXT("Target_BlurY"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_CombineBlur"), TEXT("Target_CombineBlur"))))
		return E_FAIL;

	//SSAO블러
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAOBlurX"), TEXT("Target_BlurX"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAOBlurY"), TEXT("Target_BlurY"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAOBlur"), TEXT("Target_SSAOBlur"))))
		return E_FAIL;

	//그림자블러
	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowBlurX"), TEXT("Target_ShadowBlurX"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowBlurY"), TEXT("Target_ShadowBlurY"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowBlur"), TEXT("Target_ShadowBlur"))))
		return E_FAIL;*/

	//HDR
 	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ExportHDR"), TEXT("Target_ExportFinal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_HDR"), TEXT("Target_HDR"))))
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(Viewport.Width, Viewport.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f));

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECL::Elements, VTXTEX_DECL::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 50.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 50.f, 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 50.0f, 300.0f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 150.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 150.0f, 200.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), 150.0f, 300.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSAO"), 50.0f, 400.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurX"), 150.0f, 400.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurY"), 50.0f, 500.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_CombineBlur"), 150.0f, 500.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSAOBlur"), 50.0f, 600.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_HDR"), 250.0f, 100.f, 100.f, 100.f)))
		return E_FAIL;
#endif // _DEBUG

	_uint      iNumViewport = 1;
	m_pContext->RSGetViewports(&iNumViewport, &m_VP);

	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (nullptr == pGameObject ||
		RENDER_END <= eRenderGroup)
	{
		MSG_BOX("Failed to Add_RenderGroup");
		return E_FAIL;
	}

	m_RenderObjects[eRenderGroup].emplace_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderTarget(const _tchar* pMRT_Tag, const _tchar* pRenderTargetTag, DXGI_FORMAT eFormat, _float4 vColor)
{
	_uint	iNumViewports = { 1 };
	D3D11_VIEWPORT	Viewport;
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	_float4 vColor_Diffuse = { 1.f, 0.f, 1.f, 0.f };
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, pRenderTargetTag
		, (_uint)Viewport.Width, (_uint)Viewport.Height, eFormat, vColor)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(pMRT_Tag, pRenderTargetTag)))
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_DebugGroup(CComponent* pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	m_DebugRender.emplace_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}
#endif // _DEBUG

HRESULT CRenderer::Draw_RenderObjects(HRESULT(*fp)())
{
	if (FAILED(Begin_DefaultRT()))
	{
		MSG_BOX("Failed to Begin_DefaultRT");
		return E_FAIL;
	}

	if (FAILED(Render_Priority()))
	{
		MSG_BOX("Failed to Render_Priority");
		return E_FAIL;
	}
	if (FAILED(Render_ShadowDepth()))
	{
		MSG_BOX("Failed to Render_ShadowDepth");
		return E_FAIL;
	}

	if (FAILED(Render_NonBlend()))
	{
		MSG_BOX("Failed to Render_NonBlend");
		return E_FAIL;
	}
	if (FAILED(Render_SSAO()))
	{
		MSG_BOX("Failed to Render_SSAO");
		return E_FAIL;
	}
	if (FAILED(Render_SSAOBlurX()))
	{
		MSG_BOX("Failed to Render_SSAOBlurX");
		return E_FAIL;
	}
	if (FAILED(Render_SSAOBlurY()))
	{
		MSG_BOX("Failed to Render_SSAOBlurY");
		return E_FAIL;
	}
	if (FAILED(Render_SSAOFinal()))
	{
		MSG_BOX("Failed to Render_SSAOFinalBlur");
		return E_FAIL;
	}
	/*if (FAILED(Render_ShadowBlurX()))
	{
		MSG_BOX("Failed to Render_ShadowBlurX");
		return E_FAIL;
	}
	if (FAILED(Render_ShadowBlurY()))
	{
		MSG_BOX("Failed to Render_ShadowBlurY");
		return E_FAIL;
	}
	if (FAILED(Render_ShadowBlur()))
	{
		MSG_BOX("Failed to Render_ShadowBlur");
		return E_FAIL;
	}*/

	if (FAILED(Render_Lights()))
	{
		MSG_BOX("Failed to Render_Lights");
		return E_FAIL;
	}

	if (FAILED(Render_Deferred()))
	{
		MSG_BOX("Failed to Render_Deferred");
		return E_FAIL;
	}

	if (FAILED(Render_ExportDeferred()))
	{
		MSG_BOX("Failed to Render_ExportBloom");
		return E_FAIL;
	}
	
	if (FAILED(Render_BlurX()))
	{
		MSG_BOX("Failed to Render_BlurX");
		return E_FAIL;
	}
	if (FAILED(Render_BlurY()))
	{
		MSG_BOX("Failed to Render_BlurY");
		return E_FAIL;
	}
	if (FAILED(Render_CombineBlur())) // 
	{
		MSG_BOX("Failed to Render_CombineBlur");
		return E_FAIL;
	}
	if (FAILED(Render_Bloom())) // 최종적으로 그려짐
	{
		MSG_BOX("Failed to Render_Bloom");
		return E_FAIL;
	}

	if (FAILED(Render_NonLight()))
	{
		MSG_BOX("Failed to Render_NonLight");
		return E_FAIL;
	}
	if (FAILED(Render_Blend()))
	{
		MSG_BOX("Failed to Render_Blend");
		return E_FAIL;
	}
	if (FAILED(Render_Effect()))
	{
		MSG_BOX("Failed to Render_Effect");
		return E_FAIL;
	}
	if (FAILED(Render_UI()))
	{
		MSG_BOX("Failed to Render_UI");
		return E_FAIL;
	}

#ifdef _DEBUG
	if(true == m_isRenderDebug)
	{
		if (FAILED(Render_Debug()))
		{
			MSG_BOX("Failed to Render_Debug");
			return E_FAIL;
		}
	}

	if (true == m_isRenderTarget)
	{
		if (FAILED(Render_RenderTaget()))
		{
			MSG_BOX("Failed to Render_Debug");
			return E_FAIL;
		}
	}
#endif // _DEBUG

	if (FAILED(Render_CallBack()))
	{
		MSG_BOX("Failed to Render_CallBack");
		return E_FAIL;
	}

	if (FAILED(End_DefaultRT()))
	{
		MSG_BOX("Failed to Begin_DefaultRT");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Begin_DefaultRT()
{
	if (FAILED(m_pTarget_Manager->Begin_DefaultRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Begin_MRT(const _tchar* pMRT_Tag)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(pMRT_Tag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::End_DefaultRT()
{
	if (FAILED(m_pTarget_Manager->End_DefaultRT()))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Default"), m_pShader, "g_Texture")))
		return E_FAIL;

	m_pShader->Begin(0);

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::End_MRT()
{
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);

	}

	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	/* ShadowDepth */
	m_pTarget_Manager->Begin_MRT_LightDepth(m_pContext, TEXT("MRT_LightDepth"));

	D3D11_VIEWPORT VP;
	ZeroMemory(&VP, sizeof(D3D11_VIEWPORT));
	VP.TopLeftX = 0.0f;
	VP.TopLeftY = 0.0f;
	VP.Width = 12800.f;
	VP.Height = 7200.f;
	VP.MinDepth = 0.0f;
	VP.MaxDepth = 1.0f;

	m_pContext->RSSetViewports(1, &VP);

	for (auto& pGameObject : m_RenderObjects[RENDER_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_ShadowDepth();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_SHADOWDEPTH].clear();

	m_pTarget_Manager->End_MRT();


	return S_OK;
}

HRESULT CRenderer::Render_ShadowBlurX()
{

	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurX"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	m_bSSAOBlur = true;
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAO", &m_bSSAOBlur, sizeof(_bool))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_ShadowBlurY()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurY"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	m_bSSAOBlur = true;
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAO", &m_bSSAOBlur, sizeof(_bool))))
		return E_FAIL;



	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_ShadowBlur()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlur"))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ShadowBlurX"), m_pShader, "g_BlurXTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ShadowBlurY"), m_pShader, "g_BlurYTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_Texture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_SSAO()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SSAO"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (pGameInstance->Get_DIKeyState(DIK_INSERT))
	{
		m_fSSAORadius += 0.0001f;
	}
	if (pGameInstance->Get_DIKeyState(DIK_DELETE))
	{
		m_fSSAORadius -= 0.0001f;
	}
	if (pGameInstance->Get_DIKeyState(DIK_HOME))
	{
		m_fSSAOBias += 0.0001f;
	}
	if (pGameInstance->Get_DIKeyState(DIK_END))
	{
		m_fSSAOBias -= 0.0001f;
	}

	if (FAILED(m_pShader->SetUp_RawValue("g_fRadius", &m_fSSAORadius, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_RawValue("g_fBias", &m_fSSAOBias, sizeof(_float))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 ViewMatrixInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;
	_float4x4 ProjMatrixInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;

	_float4 vCamPosition = pPipeLine->Get_CameraPosition();
	if (FAILED(m_pShader->SetUp_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pPipeLine);

	if (FAILED(m_pShader->Begin(5)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderer::Render_SSAOBlurX()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SSAOBlurX"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	m_bSSAOBlur = true;
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAO", &m_bSSAOBlur, sizeof(_bool))))
		return E_FAIL;



	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderer::Render_SSAOBlurY()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SSAOBlurY"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_bSSAOBlur = true;
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAO", &m_bSSAOBlur, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_BlurXTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(7)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderer::Render_SSAOFinal()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SSAOBlur"))))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAOBlurX"), m_pShader, "g_BlurXTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAOBlurY"), m_pShader, "g_BlurYTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_Texture")))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_SSAOTexture")))
		return E_FAIL;*/


	if (FAILED(m_pShader->Begin(9)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ExportDeferred()
{
	// 최종 장면을 추출
	// 최종 장면에서 밝은 부분만 추출 -> 블룸
	// 추출한걸 X,Y 블러 적용
	// 최종 + X + Y
	// 블룸 적용 -> 최종화면적용
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ExportHDR"))))
		return E_FAIL; 

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_CombineBlur"), m_pShader, "g_FinalTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(12)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
}


HRESULT CRenderer::Render_BlurX()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BlurX"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_bSSAOBlur = false;
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAO", &m_bSSAOBlur, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ExportFinal"), m_pShader, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(6)))
		return E_FAIL;
	/*if (FAILED(m_pShader->Begin(10)))
		return E_FAIL;*/

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderer::Render_BlurY()
{
	

	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BlurY"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_bSSAOBlur = false;
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAO", &m_bSSAOBlur, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ExportFinal"), m_pShader, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(7)))
		return E_FAIL;
	/*if (FAILED(m_pShader->Begin(11)))
		return E_FAIL;*/

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;



	return S_OK;
}

HRESULT CRenderer::Render_CombineBlur()
{
	
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HDR"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_BlurX"), m_pShader, "g_BlurXTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_BlurY"), m_pShader, "g_BlurYTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ExportFinal"), m_pShader, "g_Texture")))
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_SSAOTexture")))
		return E_FAIL;*/

	/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAOBlur"), m_pShader, "g_SSAOTexture")))
		return E_FAIL;*/

	if (FAILED(m_pShader->Begin(8)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderer::Render_Bloom()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HDR"))))
		return E_FAIL;*/

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_HDR"), m_pShader, "g_HDRTexture"))) // 블룸 + 블러 
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ExportFinal"), m_pShader, "g_BloomTextrue"))) // 블룸 -> 밝은 부분만 추출
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_CombineBlur"), m_pShader, "g_FinalTexture"))) // 디퍼드 최종 텍스처
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_SSAOTexture")))
		return E_FAIL;*/
		
	if (FAILED(m_pShader->Begin(13)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* 백버퍼를 빼고 */
	/* Diffuse + Normal */
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_GameObject"))))
		return E_FAIL;

	m_pContext->RSSetViewports(1, &m_VP);
	for (auto& pGameObject : m_RenderObjects[RENDER_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);

	}

	m_RenderObjects[RENDER_NONBLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);

	}

	m_RenderObjects[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);

	}

	m_RenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	m_RenderObjects[RENDER_UI].sort([](CGameObject* pDest, CGameObject* pSrc)->bool {
		return dynamic_cast<CUI*>(pDest)->Get_UI_Layer() < dynamic_cast<CUI*>(pSrc)->Get_UI_Layer();
	});

	for (auto& pGameObject : m_RenderObjects[RENDER_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);

	}

	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_CallBack()
{
	if (nullptr != m_Func)
	{
		if (FAILED(m_Func()))
			return S_OK;
	}

	return S_OK;
}

HRESULT CRenderer::Render_Effect()
{
	for (auto& pGameObject : m_RenderObjects[RENDER_EFFECT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RENDER_EFFECT].clear();

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	for (auto& pComponent : m_DebugRender)
	{
		if (nullptr != pComponent)
		{
			pComponent->Render();
			Safe_Release(pComponent);
		}
	}

	m_DebugRender.clear();

	return S_OK;
}

HRESULT CRenderer::Render_RenderTaget()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_GameObject"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightDepth"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_SSAO"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_BlurX"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_BlurY"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_CombineBlur"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_SSAOBlur"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_ExportHDR"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_HDR"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}
#endif // _DEBUG

HRESULT CRenderer::Render_Lights()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 ViewMatrixInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;
	_float4x4 ProjMatrixInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;

	_float4 vCamPosition = pPipeLine->Get_CameraPosition();
	if (FAILED(m_pShader->SetUp_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pPipeLine);

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAOBlur"), m_pShader, "g_SSAOTexture")))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyDown(DIK_TAB))
	{
		if (m_bSSAOSwitch == false)
			m_bSSAOSwitch = true;
		else if (m_bSSAOSwitch == true)
			m_bSSAOSwitch = false;
	}
	if (FAILED(m_pShader->SetUp_RawValue("g_bSSAOSwitch", &m_bSSAOSwitch, sizeof(_bool))))
		return E_FAIL;

	Safe_Release(pGameInstance);


	m_pLight_Manager->Render(m_pShader, m_pVIBuffer);

	m_pTarget_Manager->End_MRT();

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_CombineBlur"))))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ShadowDepth"), m_pShader, "g_ShadowDepthTexture")))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ShadowBlur"), m_pShader, "g_DepthTexture")))
		return E_FAIL;*/
		/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_ShadowBlur"), m_pShader, "g_ShadowDepthTexture")))
			return E_FAIL;*/
			/*if (FAILED(m_pTarget_Manager->Bind_ShaderResourceView(TEXT("Target_SSAO"), m_pShader, "g_SSAOTexture")))
				return E_FAIL;*/


	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_float4x4 matViewInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShader->SetUp_Matrix("g_matViewInv", &matViewInv)))
		return E_FAIL;
	_float4x4 matProjInv = pPipeLine->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->SetUp_Matrix("g_matProjInv", &matProjInv)))
		return E_FAIL;

	Safe_Release(pPipeLine);
	/*CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(pGameInstance->Get_Component(3, TEXT("Layer_Player"), TEXT("Com_Transform")));

	_vector	vPlayerPos = pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector	vLightEye = vPlayerPos + XMVectorSet(-5.f, 10.f, -5.f, 1.f);*/
	_vector	vLightEye = XMVectorSet(130.f, 10.f, 140.f, 1.f);
	_vector	vLightAt = XMVectorSet(60.f, 0.f, 60.f, 1.f);
	_vector	vLightUp = XMVectorSet(0.f, 1.f, 0.f, 1.f);

	_matrix		LightViewMatrix = XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp);
	_float4x4	FloatLightViewMatrix;
	XMStoreFloat4x4(&FloatLightViewMatrix, LightViewMatrix);

	if (FAILED(m_pShader->SetUp_Matrix("g_matLightView", &FloatLightViewMatrix)))
		return E_FAIL;

	_matrix		LightProjMatrix;
	_float4x4	FloatLightProjMatrix;

	LightProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(120.f), _float(1280) / _float(720), 0.2f, 300.f);
	XMStoreFloat4x4(&FloatLightProjMatrix, LightProjMatrix);

	if (FAILED(m_pShader->SetUp_Matrix("g_matProj", &FloatLightProjMatrix)))
		return E_FAIL;



	m_pShader->Begin(3);

	m_pVIBuffer->Render();
	m_pTarget_Manager->End_MRT();


	return S_OK;
}


CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRenderer::Clone(void* pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);


	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);

		RenderObjects.clear();
	}

#ifdef _DEBUG
	for (auto& pGameObject : m_DebugRender)
		Safe_Release(pGameObject);

	m_DebugRender.clear();
#endif // _DEBUG

}
