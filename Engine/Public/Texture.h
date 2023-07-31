#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	HRESULT Initialize_Prototype(const _tchar * pTexFilePath, _uint iNumTextures);
	HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResourceView(class CShader* pShader, const char* pConstantName, _uint iTextureIndex = 0);
	HRESULT Bind_ShaderResourceViews(class CShader* pShader, const char* pConstantName);

private:
	_uint								m_iNumTextures = { 0 };
	ID3D11ShaderResourceView**			m_ppSRVs = { nullptr };

public:
	static CTexture* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTexFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END