#include "..\Public\Mesh_Instance.h"
#include "Bone.h"



CMesh_Instance::CMesh_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CMesh_Instance::CMesh_Instance(const CMesh_Instance & rhs)
	: CVIBuffer_Instance(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_pFin(rhs.m_pFin)
{
	strcpy_s(m_szName, rhs.m_szName);
}


HRESULT CMesh_Instance::Initialize_Prototype(MESHDATA* pMeshData, _fmatrix PivotMatrix, class CModel_Instance* pModel , _uint iNumInstance)
{
	m_iMaxNumInstance = iNumInstance;
	
	m_pModel = pModel;

	XMStoreFloat4x4(&m_PivotMatrix , PivotMatrix);

	_uint iSize = { 0 };
	strcpy_s(m_szName, pMeshData->szName);
	m_iMaterialIndex = pMeshData->iMaterialIndex;
	m_iNumVertices = pMeshData->iNumVertices;
	m_iNumFaces = pMeshData->iNumFaces;


	m_iNumVertexBuffers = 2;

	m_iNumIndices = m_iNumFaces * 3 * m_iMaxNumInstance;
	m_iIndexStride = sizeof(_ulong);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXMODEL);

	m_VertexPos.reserve(m_iNumVertices);
	m_Faces.reserve(m_iNumIndices / 3);

	m_iNumIndicesPerInstance = m_iNumFaces * 3;
	m_iInstanceStride = sizeof(VTXMODELINSTANCE);

	/* 버텍스 버퍼를 할당한다. */
#pragma region VERTEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	HRESULT			hr = { 0 };

	hr = Ready_VertexBuffer_NonAnim(pMeshData, PivotMatrix);

	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEXBUFFER	

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ulong* pSaveIndices = new _ulong[m_iNumFaces * 3];
	ZeroMemory(pSaveIndices, sizeof(_ulong) * m_iNumFaces * 3);

	_uint		iSaveNumIndices = { 0 };

	for (_uint i = 0; i < m_iNumFaces; ++i)
	{
		pSaveIndices[iSaveNumIndices]		= pMeshData->pMeshIdxData[i].iIndex0;
		pSaveIndices[iSaveNumIndices + 1]	= pMeshData->pMeshIdxData[i].iIndex1;
		pSaveIndices[iSaveNumIndices + 2]	= pMeshData->pMeshIdxData[i].iIndex2;

		iSaveNumIndices += 3;
	}
	

	_ulong* pIndices = new _ulong[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ulong) * m_iNumIndices);

	_uint		iNumIndices = { 0 };
	iSaveNumIndices = 0;

	for (_uint j = 0; j < m_iMaxNumInstance; ++j)
	{
		for (_uint i = 0; i < m_iNumFaces; ++i)
		{
			pIndices[iNumIndices] = pSaveIndices[iSaveNumIndices];
			pIndices[iNumIndices+1] = pSaveIndices[iSaveNumIndices+1];
			pIndices[iNumIndices+2] = pSaveIndices[iSaveNumIndices+2];

			m_Faces.emplace_back(_uint3(pIndices[iNumIndices], pIndices[iNumIndices + 1], pIndices[iNumIndices + 2]));

			iNumIndices += 3;
			iSaveNumIndices += 3;
		}
		iSaveNumIndices = 0;
	}


	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pIndices;

	/* 인덱스 버퍼를 할당한다. */
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pSaveIndices);

	return S_OK;
}

HRESULT CMesh_Instance::Initialize(void * pArg)
{
	m_iInstanceStride = sizeof(VTXMODELINSTANCE);


	return S_OK;
}

void CMesh_Instance::Tick(_double TimeDelta)
{
	
}

HRESULT CMesh_Instance::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	m_pContext->DrawIndexedInstanced(m_iNumIndicesPerInstance, m_pModel->Get_NumInstance(), 0, 0, 0);

	return S_OK;
}

HRESULT CMesh_Instance::Bind_Resources()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pModel->Get_VBInstance(),
	};

	_uint					iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint					iOffsets[] = {
		0,
		0
	};

	/* 버텍스 버퍼들을 장치에 바인딩하낟. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

HRESULT CMesh_Instance::Ready_VertexBuffer_NonAnim(MESHDATA* pMeshData, _fmatrix PivotMatrix)
{
	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition = pMeshData->pMeshVtxData[i].vPosition;
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		m_VertexPos.emplace_back(_float4(pVertices[i].vPosition.x, pVertices[i].vPosition.y, pVertices[i].vPosition.z, 1.f));

		pVertices[i].vNormal = pMeshData->pMeshVtxData[i].vNormal;
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		pVertices[i].vTexUV = pMeshData->pMeshVtxData[i].vTexUV;
		pVertices[i].vTangent = pMeshData->pMeshVtxData[i].vTangent;
	}

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh_Instance * CMesh_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, MESHDATA* pMeshData, _fmatrix PivotMatrix, CModel_Instance* pModel, _uint iNumInstance)
{
	CMesh_Instance*		pInstance = new CMesh_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pMeshData, PivotMatrix, pModel , iNumInstance)))
	{
		MSG_BOX("Failed to Created : CMesh_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CComponent * CMesh_Instance::Clone(void * pArg)
{
	CMesh_Instance*		pInstance = new CMesh_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CMesh_Instance::Free()
{
	__super::Free();
}
