#include "..\Public\Target_Manager.h"

#include "RenderTarget.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}
// shadow
HRESULT CTarget_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Ready_DefaultRenderTarget()))
	{
		MSG_BOX("Failed to Ready_DefaultRenderTarget : CTarget_Manager");
		return E_FAIL;
	}
	if (FAILED(Ready_LightDepthStencilRenderTargetView()))
	{
		MSG_BOX("Failed to Ready_LightDepthStencilRenderTargetView : CTarget_Manager");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, const _float4& vClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(pDevice, pContext, iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;

		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_DefaultRT()
{
	m_pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pDSV);

	m_pContext->ClearRenderTargetView(m_pDefaultRTV, (_float*)&m_vColor_Default);

	m_pContext->OMSetRenderTargets(1, &m_pDefaultRTV, m_pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	m_pContext->OMGetRenderTargets(1, &m_pDefaultRTV, &m_pDSV);

	_uint		iNumRenderTargets = { 0 };

	ID3D11RenderTargetView* pRTVs[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRTVs[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRTVs, m_pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT_NoneClear(const _tchar* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	m_pContext->OMGetRenderTargets(1, &m_pDefaultRTV, &m_pDSV);

	_uint		iNumRenderTargets = { 0 };

	ID3D11RenderTargetView* pRTVs[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		//pRenderTarget->Clear();
		pRTVs[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}

	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRTVs, m_pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT_LightDepth(ID3D11DeviceContext* pContext, const _tchar* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;
	
	pContext->ClearDepthStencilView(m_pShadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pContext->OMGetRenderTargets(1, &m_pDefaultRTV, &m_pDSV);		// 장치에 바인딩 되어있던 0번째 RenderTarget만 꺼내와라

	_uint	iNumRenderTargets = { 0 };

	ID3D11RenderTargetView* pRTVs[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRTVs[iNumRenderTargets++] = pRenderTarget->Get_RTV();		// 기존에 있던 백버퍼를 꺼내와서 따로 저장한다.
	}

	pContext->OMSetRenderTargets(iNumRenderTargets, pRTVs, m_pShadowDSV);

	return S_OK;
}

HRESULT CTarget_Manager::End_DefaultRT()
{
	m_pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pDSV);

	Safe_Release(m_pOldRTV);
	Safe_Release(m_pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	m_pContext->OMSetRenderTargets(1, &m_pDefaultRTV, m_pDSV);

	Safe_Release(m_pDefaultRTV);
	Safe_Release(m_pDSV);

	return S_OK;
}

HRESULT CTarget_Manager::Bind_ShaderResourceView(const _tchar* pTargetTag, CShader* pShader, const char* pConstantName)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResourceView(pShader, pConstantName);
}

HRESULT CTarget_Manager::Ready_DefaultRenderTarget()
{
	_uint	iNumViewports = { 1 };
	D3D11_VIEWPORT	Viewport;
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	m_vColor_Default = { 0.8f, 0.5f, 0.5f, 0.f };
	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext
		, (_uint)Viewport.Width, (_uint)Viewport.Height, DXGI_FORMAT_B8G8R8A8_UNORM, m_vColor_Default);

	m_RenderTargets.emplace(TEXT("Target_Default"), pRenderTarget);

	m_pDefaultRTV = pRenderTarget->Get_RTV();

	return S_OK;
}

HRESULT CTarget_Manager::Ready_LightDepthStencilRenderTargetView()
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 12800;
	TextureDesc.Height = 7200;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	//pTargetTag 렌더타겟을 찾음
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	//찾은 렌더타겟을 SizeX, SizeY 크기로 X,Y 위치에 그린다
	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug(const _tchar* pMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		if (nullptr != pRenderTarget)
			pRenderTarget->Render_Debug(pShader, pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTag_Finder(pTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CTarget_Manager::Find_MRT(const _tchar* pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTag_Finder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pShadowDSV);
	
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();
}
