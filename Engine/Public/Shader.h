#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar * pShaderFilePath, D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElemets);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT SetUp_RawValue(const char* pConstantName, const void* pValue, _uint iByteSize);
	HRESULT SetUp_Vector(const char* pConstantName, const _float4 * pVector);
	HRESULT SetUp_Matrix(const char* pConstantName, const _float4x4 * pMatrix);
	HRESULT SetUp_Matrix_Array(const char* pConstantName, const _float4x4 * pMatrix, _uint iNumMatrices);
	HRESULT SetUp_SRV(const char* pConstantName, ID3D11ShaderResourceView * pSRV);
	HRESULT SetUp_SRV_Array(const char* pConstantName, ID3D11ShaderResourceView ** ppSRVs, _uint iNumTextures);
public:
	HRESULT Begin(_uint iPassIndex);

private:
	ID3DX11Effect*				m_pEffect = { nullptr };
	ID3DX11EffectTechnique*		m_pTechnique = { nullptr };
	_uint						m_iNumPasses = { 0 };

	vector<ID3D11InputLayout*>	m_InputLayouts;
public:
	static CShader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElemets);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END