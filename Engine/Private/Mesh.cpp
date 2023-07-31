#include "..\Public\Mesh.h"

#include "Model.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_Bones(rhs.m_Bones)
{
	strcpy_s(m_szName, rhs.m_szName);
}

void CMesh::Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix, CModel* pModel)
{
	for (_uint i = 0; i < m_iNumBones; i++)
	{
		CBone* pBone = pModel->Get_Bone(m_Bones[i]);

		XMStoreFloat4x4(&pBoneMatrices[i],
			pBone->Get_OffsetMatrix() *
			pBone->Get_CombinedTransformationMatrix() * PivotMatrix);
	}
}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, MESHDATA* pMeshData, _fmatrix PivotMatrix, CModel* pModel)
{
	_uint iSize = { 0 };
	strcpy_s(m_szName, pMeshData->szName);
	m_iMaterialIndex = pMeshData->iMaterialIndex;
	m_iNumVertices = pMeshData->iNumVertices;
	m_iNumIndices = pMeshData->iNumFaces;

	m_iNumVertexBuffers = 1;
	m_iIndexStride = sizeof(_ulong);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = CModel::TYPE_NONANIM == eModelType ? sizeof(VTXMODEL) : sizeof(VTXANIMMODEL);

	m_VertexPos.reserve(m_iNumVertices);
	m_Faces.reserve(m_iNumIndices);
	m_iNumIndices *= 3;

#pragma region VERTEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;
	
	HRESULT hr = { 0 };

	if (CModel::TYPE_NONANIM == eModelType)
	{
		hr = Ready_VertexBuffer_NonAnim(pMeshData, PivotMatrix);
	}
	else
	{
		hr = Ready_VertexBuffer_Anim(pMeshData, pModel);
	}

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

	_ulong* pIndices = new _ulong[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ulong) * m_iNumIndices);

	_uint iNumIndices = { 0 };

	_uint iNumFaces = m_iNumIndices / 3;

	for (_uint i = 0; i < iNumFaces; i++)
	{
		pIndices[iNumIndices]		= pMeshData->pMeshIdxData[i].iIndex0;
		pIndices[iNumIndices + 1]	= pMeshData->pMeshIdxData[i].iIndex1;
		pIndices[iNumIndices + 2]	= pMeshData->pMeshIdxData[i].iIndex2;

		m_Faces.emplace_back(_uint3(pIndices[iNumIndices], pIndices[iNumIndices + 1], pIndices[iNumIndices + 2]));

		iNumIndices += 3;
	}

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(MESHDATA* pMeshData, _fmatrix PivotMatrix)
{
	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i++)
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

HRESULT CMesh::Ready_VertexBuffer_Anim(MESHDATA* pMeshData, CModel* pModel)
{
	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vPosition = pMeshData->pMeshVtxData[i].vPosition;

		m_VertexPos.emplace_back(_float4(pVertices[i].vPosition.x, pVertices[i].vPosition.y, pVertices[i].vPosition.z, 1.f));

		pVertices[i].vNormal = pMeshData->pMeshVtxData[i].vNormal;

		pVertices[i].vTexUV = pMeshData->pMeshVtxData[i].vTexUV;
		pVertices[i].vTangent = pMeshData->pMeshVtxData[i].vTangent;
	}


	//메쉬에 영향을 주는 뼈의 갯수
	m_iNumBones = pMeshData->iNumBones;

	for (_uint i = 0; i < m_iNumBones; i++)
	{
		//뼈와 정점의 로컬 스페이스가 다를 수 있는데, 이를 보정해주는 행렬
		_float4x4 OffsetMatrix;
		OffsetMatrix = pMeshData->pAnimMeshData[i].OffsetMatrix;

		char szName[MAX_PATH] = { "" };
		strcpy_s(szName, pMeshData->pAnimMeshData[i].szName);

		//모델에서 로드한 뼈 중 메쉬에 영향을 주는 i번째 뼈와 이름이 같은 뼈를 찾아 주소를 저장한다.
		CBone* pBone = pModel->Get_Bone(szName);
		if (nullptr == pBone)
			return E_FAIL;

		//R,U,L이 Row 정렬 상태 - Transpose로 Column 정렬로 바꿔주고 Set해준다.
		pBone->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

		m_Bones.emplace_back(pModel->Get_BoneIndex(szName));

		_uint iNumWeights = pMeshData->pAnimMeshData[i].iNumWeights;

		//i번째 뼈가 영향을 주는 정점의 갯수
		for (_uint j = 0; j < iNumWeights; j++)
		{
			_uint iVertexID = pMeshData->pAnimMeshData[i].pWeightData[j].iVertexID;
			_float fWeights = pMeshData->pAnimMeshData[i].pWeightData[j].fWeights;

			//pAIBone->mWeights[j].mVertexId = i번째 뼈가 영향을 주는 j정점의 인덱스"Idx"
			if (0.f == pVertices[iVertexID].vBlendWeights.x)
			{
				//"Idx" 정점에 영향을 주는 뼈 x = i
				pVertices[iVertexID].vBlendIndices.x = i;

				//"Idx" 정점에 뼈 x가 몇 %의 영향을 주는가
				pVertices[iVertexID].vBlendWeights.x = fWeights;
			}
			else if (0.f == pVertices[iVertexID].vBlendWeights.y)
			{
				pVertices[iVertexID].vBlendIndices.y = i;
				pVertices[iVertexID].vBlendWeights.y = fWeights;
			}
			else if (0.f == pVertices[iVertexID].vBlendWeights.z)
			{
				pVertices[iVertexID].vBlendIndices.z = i;
				pVertices[iVertexID].vBlendWeights.z = fWeights;
			}
			else
			{
				pVertices[iVertexID].vBlendIndices.w = i;
				pVertices[iVertexID].vBlendWeights.w = fWeights;
			}
		}
	}

	//뼈(애니메이션)가 없는 메쉬에 뼈를 강제로 1로 넣어줘서 모델의 애니메이션이 재생될 때
	//메쉬가 제 자리에서 재생될 수 있도록 예외처리를 해준다.
	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_Bones.emplace_back(pModel->Get_BoneIndex(m_szName));
	}

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, MESHDATA* pMeshData, _fmatrix PivotMatrix, CModel* pModel)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pMeshData, PivotMatrix, pModel)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();

	m_Bones.clear();
}
