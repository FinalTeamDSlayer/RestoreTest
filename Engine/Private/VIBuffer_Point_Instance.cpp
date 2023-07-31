#include "..\Public\VIBuffer_Point_Instance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs)
	: CVIBuffer_Instance(rhs)
{
}

HRESULT CVIBuffer_Point_Instance::Initialize_Prototype(_uint iNumInstance)
{
	m_iNumInstance = iNumInstance;
	m_iNumIndicesPerInstance = 1;
	m_iNumVertexBuffers = 2;
	m_iStride = sizeof(VTXPOS);
	m_iNumVertices = 1;
	m_iNumIndices = m_iNumInstance;
	m_iIndexStride = sizeof(_ushort);
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	m_iInstanceStride = sizeof(VTXINSTANCE);


#pragma region VERTEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXPOS* pVertices = new VTXPOS;
	ZeroMemory(pVertices, sizeof(VTXPOS));

	pVertices->vPosition = _float3(0.0f, 0.0f, 0.f);
	

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region INDEXBUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	for (_uint i = 0; i < m_iNumIndices; ++i)
		pIndices[i] = 0;

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	random_device rd;
	mt19937 engine(rd());
	uniform_real_distribution<_float> Distribution(-1.f, 1.f);

	INSTANCEDESC	InstanceDesc = *(INSTANCEDESC*)pArg;
	m_InstanceDesc = InstanceDesc;

	m_pSpeed = new _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float fRand = Distribution(engine);

		fRand = ((fRand + 1.f) / 0.5f) + 0.01f;

		m_pSpeed[i] = (fRand * (m_InstanceDesc.fMaxSpeed - m_InstanceDesc.fMinSpeed)) + m_InstanceDesc.fMinSpeed;
	}

	m_pScale = new _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_float fRand = Distribution(engine);

		fRand = ((fRand + 1.f) / 0.5f) + 0.01f;

		m_pScale[i] = (fRand * (m_InstanceDesc.fMaxScale - m_InstanceDesc.fMinScale)) + m_InstanceDesc.fMinScale;
	}

	m_pAngle = new _float[m_iNumInstance];
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			_float fRand = Distribution(engine);

			fRand = ((fRand + 1.f) / 0.5f);

			m_pAngle[i] = (fRand * 360.f);
		}
	}

#pragma region INSTANCEBUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXINSTANCE* pInstanceVertices = new VTXINSTANCE[m_iNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXINSTANCE) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);

		pInstanceVertices[i].vTranslation = _float4(Distribution(engine) * InstanceDesc.vRange.x,
													Distribution(engine) * InstanceDesc.vRange.y,
													Distribution(engine) * InstanceDesc.vRange.z, 1.f);
		pInstanceVertices[i].vColor = _float4(1.0f, 1.f, (_float)(rand() % 2), 0.f);
		pInstanceVertices[i].vPSize = _float2(m_pScale[i], m_pScale[i]);
		pInstanceVertices[i].fAngle = m_pAngle[i];
	}

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);

#pragma endregion

	return S_OK;
}

//CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance)
//{
//	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
//	{
//		MSG_BOX("Failed to Created : CVIBuffer_Point_Instance");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}
//CComponent* CVIBuffer_Point_Instance::Clone(void* pArg)
//{
//	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(*this);
//
//	if (FAILED(pInstance->Initialize(pArg)))
//	{
//		MSG_BOX("Failed to Cloned : CVIBuffer_Point_Instance");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pSpeed);
	Safe_Delete_Array(m_pScale);
	Safe_Delete_Array(m_pAngle);
}
