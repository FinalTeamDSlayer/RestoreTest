#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_iNumPasses(rhs.m_iNumPasses)
	, m_pEffect(rhs.m_pEffect)
	, m_pTechnique(rhs.m_pTechnique)
	, m_InputLayouts(rhs.m_InputLayouts)
{
	for (_uint i = 0; i < m_iNumPasses; i++)
		Safe_AddRef(m_InputLayouts[i]);

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElemets)
{
	_uint iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG

	//경로에 있는 셰이더 파일을 코드에서 사용할 수 있도록 pEffect 객체로 만듦
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	//만든 pEffect에서 0번째 technique11객체를 받아옴
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == m_pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC	TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof TechniqueDesc);

	//Technique 객체가 가진 정보를 받아옴 (passes를 사용하기 위함)
	m_pTechnique->GetDesc(&TechniqueDesc);
	
	m_iNumPasses = TechniqueDesc.Passes;

	//Technique 객체의 pass 수 만큼 반복
	//즉 셰이더 파일안의 pass 수 만큼 반복한다
	for (_uint i = 0; i < m_iNumPasses; i++)
	{
		//만든 InputLayout을 받을 변수
		ID3D11InputLayout* pInputLayout = { nullptr };
		//i번 pass객체를 받아옴
		ID3DX11EffectPass* pPass = m_pTechnique->GetPassByIndex(i);
		if (nullptr == pPass)
			return E_FAIL;

		D3DX11_PASS_DESC PassDesc;
		ZeroMemory(&PassDesc, sizeof PassDesc);

		//pass객체가 가진 정보를 받아옴 (InputLayout을 만들 때 필요)
		pPass->GetDesc(&PassDesc);

		//InputLayout을 만들 때 3, 4번 인자를 넣어줄 PassDesc를 얻어오고, 그 정보를 만들기 위해 위의 과정을 수행했음
		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElemets, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		//만든 객체를 벡터에 추가
		m_InputLayouts.emplace_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::SetUp_RawValue(const char* pConstantName, const void* pValue, _uint iByteSize)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pValue, 0, iByteSize);
}

HRESULT CShader::SetUp_Vector(const char* pConstantName, const _float4* pVector)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectVectorVariable* pVectorVariable = pVariable->AsVector();
	if (nullptr == pVectorVariable)
		return E_FAIL;

	//매개변수로 받은 행렬의 값을 셰이더의 전역변수에 전달
	return pVectorVariable->SetFloatVector((const _float*)pVector);
}

//셰이더에 값을 전달해줄 함수
HRESULT CShader::SetUp_Matrix(const char* pConstantName, const _float4x4* pMatrix)
{
	//상수 문자열과 같은 이름의 전역변수를 객체로 만듦 - 값을 받을 객체
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	//객체를 행렬로 캐스팅 - 셰이더의 전역변수가 행렬이 아닐 경우 오류 발생
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	//매개변수로 받은 행렬의 값을 셰이더의 전역변수에 전달
	return pMatrixVariable->SetMatrix((const _float*)pMatrix);
}

HRESULT CShader::SetUp_Matrix_Array(const char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((const _float*)pMatrix, 0, iNumMatrices);
}

HRESULT CShader::SetUp_SRV(const char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	//상수 문자열과 같은 이름의 전역변수를 객체로 만듦 - 값을 받을 객체
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	//객체를 셰이더 리소스로 캐스팅
	ID3DX11EffectShaderResourceVariable* pSRVVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVVariable)
		return E_FAIL;

	//매개변수로 받은 텍스처(리소스로 변환된)의 값을 셰이더의 텍스처 전역 변수에 전달
	return pSRVVariable->SetResource(pSRV);
}

HRESULT CShader::SetUp_SRV_Array(const char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumTextures)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVVariable)
		return E_FAIL;

	return pSRVVariable->SetResourceArray(ppSRVs, 0, iNumTextures);
}

//Begin이 호출되기 전에(이 안의 Apply가 호출되기 전에)
//반드시 셰이더에 선언된 전역변수(셰이더가 전달받아야 할 값)의
//모든 값 전달이 끝나야 한다.
HRESULT CShader::Begin(_uint iPassIndex)
{
	if (m_iNumPasses <= iPassIndex)
		return E_FAIL;

	//DX9에서 SetFVF()의 기능을 구현해준 것
	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	ID3DX11EffectPass* pEffectPass = m_pTechnique->GetPassByIndex(iPassIndex);
	if (nullptr == pEffectPass)
		return E_FAIL;

	pEffectPass->Apply(0, m_pContext);

	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElemets)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElemets)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);

	for (_uint i = 0; i < m_iNumPasses; i++)
		Safe_Release(m_InputLayouts[i]);
}
