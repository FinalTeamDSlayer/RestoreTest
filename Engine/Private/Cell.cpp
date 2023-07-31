#include "..\Public\Cell.h"

#ifdef _DEBUG
#include "PipeLine.h"
#include "Shader.h"
#include "VIBuffer_Cell.h"
#endif // _DEBUG


CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _uint iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	m_iIndex = iIndex; 

	//기존 벡터에 x, z를 바꾸고 x에 -를 붙여 Normal을 만듦
	XMStoreFloat3(&m_vNormals[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	m_vNormals[LINE_AB] = _float3(m_vNormals[LINE_AB].z * -1.f, 0.0f, m_vNormals[LINE_AB].x);

	XMStoreFloat3(&m_vNormals[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	m_vNormals[LINE_BC] = _float3(m_vNormals[LINE_BC].z * -1.f, 0.0f, m_vNormals[LINE_BC].x);

	XMStoreFloat3(&m_vNormals[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
	m_vNormals[LINE_CA] = _float3(m_vNormals[LINE_CA].z * -1.f, 0.0f, m_vNormals[LINE_CA].x);

#ifdef _DEBUG
	if (FAILED(Ready_Debug()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

_bool CCell::Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(pSourPoint), XMLoadFloat3(&m_vPoints[POINT_A])))
	{//SourPoint가 A점일 때
		if (true == XMVector3Equal(XMLoadFloat3(pDestPoint), XMLoadFloat3(&m_vPoints[POINT_B])))
			//pDestPoint가 B점과 같으면 true
			return true;

		else if (true == XMVector3Equal(XMLoadFloat3(pDestPoint), XMLoadFloat3(&m_vPoints[POINT_C])))
			//pDestPoint가 C점과 같으면 true
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(pSourPoint), XMLoadFloat3(&m_vPoints[POINT_B])))
	{//SourPoint가 B점일 때
		if (true == XMVector3Equal(XMLoadFloat3(pDestPoint), XMLoadFloat3(&m_vPoints[POINT_C])))
			//pDestPoint가 C점과 같으면 true
			return true;

		else if (true == XMVector3Equal(XMLoadFloat3(pDestPoint), XMLoadFloat3(&m_vPoints[POINT_A])))
			//pDestPoint가 A점과 같으면 true
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(pSourPoint), XMLoadFloat3(&m_vPoints[POINT_C])))
	{//SourPoint가 C점일 때
		if (true == XMVector3Equal(XMLoadFloat3(pDestPoint), XMLoadFloat3(&m_vPoints[POINT_A])))
			//pDestPoint가 A점과 같으면 true
			return true;

		else if (true == XMVector3Equal(XMLoadFloat3(pDestPoint), XMLoadFloat3(&m_vPoints[POINT_B])))
			//pDestPoint가 B점과 같으면 true
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; i++)
	{
		_vector		vSour = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector		vDest = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0.f < XMVectorGetX(XMVector3Dot(vSour, vDest)))
		{
			*pNeighborIndex = m_iNeighborIndex[i];
			return false;
		}
	}

	return true;
}

_float3 CCell::CheckPoint(_float3 vTargetPos, _float fDistance)
{
	_int iIndex = { -1 };
	_float fDis = D3D11_FLOAT32_MAX;
	_float fMin = D3D11_FLOAT32_MAX;
	for (_uint i = 0; i < POINT_END; i++)
	{
		fDis = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[i]) - XMLoadFloat3(&vTargetPos)));
		if (fDistance >= fDis)
		{
			if (D3D11_FLOAT32_MAX == fMin)
			{
				fMin = fDis;
				iIndex = i;
			}
			
			if (fMin > fDis)
			{
				fMin = fDis;
				iIndex = i;
			}
		}
	}

	if (D3D11_FLOAT32_MAX != fMin)
		return m_vPoints[iIndex];
	else
		return vTargetPos;
}

_vector CCell::Sliding(_fvector vPosition, _fvector vCurrentPos)
{
	for (_uint i = 0; i < LINE_END; i++)
	{
		_vector		vSour = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector		vDest = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0.f < XMVectorGetX(XMVector3Dot(vSour, vDest)))
		{
			_vector vDir = XMVector3Normalize(vPosition - vCurrentPos);

			_vector vSliding = (vDir + (XMVector3Dot(-vDir, -vDest)) * -vDest);

			return vSliding;
		}
	}

	return XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

#ifdef _DEBUG
HRESULT CCell::Render_Debug(const _float4* pColor, _float fY)
{
	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());


	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	WorldMatrix._42 += fY;

	if (FAILED(m_pShader->SetUp_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	_float4x4 ViewMatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->SetUp_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;

	_float4x4 ProjMatrix = pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShader->SetUp_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->SetUp_RawValue("g_vColor", pColor, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pPipeLine);

	m_pShader->Begin(0);

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CCell::Ready_Debug()
{
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECL::Elements, VTXPOS_DECL::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}
#endif // _DEBUG

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _uint iIndex)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
#endif // _DEBUG

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
