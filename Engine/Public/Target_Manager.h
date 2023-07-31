#pragma once

#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT Begin_DefaultRT();
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT Begin_MRT_NoneClear(const _tchar* pMRTTag);
	HRESULT Begin_MRT_LightDepth(ID3D11DeviceContext* pContext, const _tchar* pMRTTag); // Shadow
	HRESULT End_DefaultRT();
	HRESULT End_MRT();

	HRESULT Bind_ShaderResourceView(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);

	HRESULT Ready_DefaultRenderTarget();
	HRESULT Ready_LightDepthStencilRenderTargetView(); // Shadow

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG


private:
	map<const _tchar*, class CRenderTarget*>			m_RenderTargets;
	typedef map<const _tchar*, class CRenderTarget*>	RENDERTARGETS;

private:
	map<const _tchar*, list<class CRenderTarget*>>		m_MRTs;

private:
	ID3D11RenderTargetView*		m_pOldRTV = { nullptr };
	ID3D11RenderTargetView*		m_pDefaultRTV = { nullptr };
	ID3D11DepthStencilView*		m_pDSV = { nullptr };

	_float4					m_vColor_Default = { 0.8f, 0.5f, 0.5f, 0.f };

	// Shadow
	ID3D11DepthStencilView* m_pShadowDSV = { nullptr };
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END