#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_pVerticesPos(rhs.m_pVerticesPos)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong	dwByte = { 0 };
	HANDLE	hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
	{
		MSG_BOX("Failed to CreateFile : VIBuffer_Terrain");
		return E_FAIL;
	}

	BITMAPFILEHEADER fh;
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);

	BITMAPINFOHEADER ih;
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	_ulong* pPixel = new _ulong[m_iNumVertices];
	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVertices, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVertexBuffers = 1;
	m_iStride = sizeof(VTXNORTEX);
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_iIndexStride = sizeof(_ulong);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pVerticesPos = new _float4[m_iNumVertices];

#pragma region VERTEXBUFFER
	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	
	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));

			_float4 vPos = _float4(pVertices[iIndex].vPosition.x, pVertices[iIndex].vPosition.y, pVertices[iIndex].vPosition.z, 1.f);

			m_pVerticesPos[iIndex] = vPos;
		}
	}

	Safe_Delete_Array(pPixel);
#pragma endregion


#pragma region INDEXBUFFER

	_uint iNumIndices = { 0 };

	_vector vSour, vDest, vNormal;

	_ulong* pIndices = new _ulong[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ulong) * m_iNumIndices);

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;
			
			_uint iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumIndices]		= iIndices[0];
			pIndices[iNumIndices + 1]	= iIndices[1];
			pIndices[iNumIndices + 2]	= iIndices[2];

			vSour = XMLoadFloat3(&pVertices[pIndices[iNumIndices + 1]].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumIndices]].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[iNumIndices + 2]].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumIndices + 1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[iNumIndices]].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumIndices]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumIndices + 1]].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumIndices + 1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumIndices + 2]].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumIndices + 2]].vNormal) + vNormal);

			iNumIndices += 3;

			pIndices[iNumIndices]		= iIndices[0];
			pIndices[iNumIndices + 1]	= iIndices[2];
			pIndices[iNumIndices + 2]	= iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[iNumIndices + 1]].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumIndices]].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[iNumIndices + 2]].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumIndices + 1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pIndices[iNumIndices]].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumIndices]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumIndices + 1]].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumIndices + 1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumIndices + 2]].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumIndices + 2]].vNormal) + vNormal);

			iNumIndices += 3;

		}
	}

	for (_uint i = 0; i < m_iNumVertices; i++)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pVB)))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pIndices;

  	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	
	return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);
}
