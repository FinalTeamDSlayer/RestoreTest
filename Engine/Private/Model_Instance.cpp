#include "..\Public\Model_Instance.h"

#include "Texture.h"
#include "Mesh_Instance.h"
#include "Bone.h"

#include <random>

static int	Get_RandomNumber(int iMin, int iMax)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(iMin, iMax);

	return dis(gen);
}

CModel_Instance::CModel_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CModel_Instance::CModel_Instance(const CModel_Instance& rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_ModelFilePath(rhs.m_ModelFilePath)
	, m_iMaxNumInstance(rhs.m_iMaxNumInstance)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}

}

HRESULT CModel_Instance::Initialize_Prototype(const char* pModelFilePath, _fmatrix PivotMatrix, _uint iMaxNumInstance)
{
	m_ModelFilePath = pModelFilePath;
	m_iMaxNumInstance = iMaxNumInstance;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	return S_OK;
}

HRESULT CModel_Instance::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	ZeroMemory(&m_ModelData, sizeof m_ModelData);

	ifstream fin;
	fin.open(m_ModelFilePath, ios::binary);

	if (false == fin.is_open())
	{
		MSG_BOX("Failed to ReadModelBinaryFile : CModel_Instance");
		return E_FAIL;
	}

	if (FAILED(Ready_ModelData()))
	{
		MSG_BOX("Failed to Ready_ModelData : CModel_Instance");
		return E_FAIL;
	}

	if (FAILED(Ready_HierarchyBones()))
	{
		MSG_BOX("Failed to Ready_HierarchyBones : CModel_Instance");
		return E_FAIL;
	}

	if (FAILED(Ready_Meshes(m_iMaxNumInstance)))
		return E_FAIL;

	if (FAILED(Ready_Materials()))
		return E_FAIL;

	if (FAILED(Clear_LoadData()))
	{
		MSG_BOX("Failed to Clear_LoadData : CModel");
		return E_FAIL;
	}


	memcpy(&m_tModelInstanceDesc, pArg, sizeof m_tModelInstanceDesc);

	m_iNumInstance = m_tModelInstanceDesc.iNumInstance;
	
#pragma region INSTANCEBUFFER	
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	_uint iInstanceStride = sizeof(VTXMODELINSTANCE);

	m_BufferDesc.ByteWidth = iInstanceStride * m_iMaxNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;	// CPU 가 접근해서 실제 이 값을 교체 할 수 있어야한다.
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = iInstanceStride;

	VTXMODELINSTANCE* pInstanceVertices = new VTXMODELINSTANCE[m_iMaxNumInstance];
	ZeroMemory(pInstanceVertices, sizeof(VTXMODELINSTANCE) * m_iMaxNumInstance);

	for (_uint i = 0; i < m_iMaxNumInstance; ++i)
	{
		_float fSize = Get_RandomNumber((_int)(m_tModelInstanceDesc.fMinSize * (_int)1000.f), ((_int)m_tModelInstanceDesc.fMaxSize * (_int)1000.f)) / 1000.f;

		float angle = 0.f;
		if (m_tModelInstanceDesc.bRandomRatationY)
			angle = Get_RandomNumber(0, 360) / 1.f;

		_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * fSize;
		_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * fSize;
		_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * fSize;

		_matrix		RotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(angle), XMConvertToRadians(0.f));

		XMStoreFloat4(&pInstanceVertices[i].vRight, XMVector3TransformNormal(vRight, RotationMatrix));
		XMStoreFloat4(&pInstanceVertices[i].vUp, XMVector3TransformNormal(vUp, RotationMatrix));
		XMStoreFloat4(&pInstanceVertices[i].vLook, XMVector3TransformNormal(vLook, RotationMatrix));

		if (m_tModelInstanceDesc.iArrangementType == 0)	// RECT
		{
			_float fRanNumX = Get_RandomNumber((_int)(-m_tModelInstanceDesc.fRange * 1000.f), (_int)(m_tModelInstanceDesc.fRange * 1000.f)) / 1000.f;
			_float fRanNumZ = Get_RandomNumber((_int)(-m_tModelInstanceDesc.fRange * 1000.f), (_int)(m_tModelInstanceDesc.fRange * 1000.f)) / 1000.f;

			pInstanceVertices[i].vTranslation = _float4(fRanNumX, 0.f, fRanNumZ, 1.f);
		}
		else if (m_tModelInstanceDesc.iArrangementType == 1)	// LINEX
			pInstanceVertices[i].vTranslation = _float4((i * m_tModelInstanceDesc.fRange) / (_float)m_tModelInstanceDesc.iNumInstance, 0.f, 0.f, 1.f);

		else if (m_tModelInstanceDesc.iArrangementType == 2)	// LINEY
			pInstanceVertices[i].vTranslation = _float4(0.f, 0.f, (i * m_tModelInstanceDesc.fRange) / (_float)m_tModelInstanceDesc.iNumInstance, 1.f);

		pInstanceVertices[i].iNumInstance = m_tModelInstanceDesc.iNumInstance;
	}

	ZeroMemory(&m_SubresourceData, sizeof m_SubresourceData);
	m_SubresourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, &m_pVBInstance)))	// VBInstance에 저장하고 새로운 정점이 생겼다!
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);

#pragma endregion

	for (_uint i = 0; i < m_iNumMeshes; ++i)
		m_Meshes[i]->Initialize(pArg);

	return S_OK;
}

HRESULT CModel_Instance::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Resources();
	m_Meshes[iMeshIndex]->Render();
	
	return S_OK;
}

HRESULT CModel_Instance::Bind_ShaderResource(CShader * pShader, _uint iMeshIndex, const char * pConstantName, MESHMATERIALS::TEXTURETYPE eType)
{
	return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pTexture[eType]->Bind_ShaderResourceView(pShader, pConstantName);
}

HRESULT CModel_Instance::Ready_ModelData()
{
	ifstream fin;
	fin.open(m_ModelFilePath, ios::binary);

	if (false == fin.is_open())
	{
		MSG_BOX("Failed to ReadModelBinaryFile : CModel");
		return E_FAIL;
	}

#pragma region Bones
	fin.read(reinterpret_cast<char*>(&m_ModelData.iNumBones), sizeof(_uint));

	if (0 != m_ModelData.iNumBones)
	{
		m_ModelData.pBoneData = new BONEDATA[m_ModelData.iNumBones];

		for (_uint i = 0; i < m_ModelData.iNumBones; i++)
		{
			fin.read(reinterpret_cast<char*>(&m_ModelData.pBoneData[i].iNameSize), sizeof(_uint));
			fin.read(m_ModelData.pBoneData[i].szName, m_ModelData.pBoneData[i].iNameSize);
			strcat_s(m_ModelData.pBoneData[i].szName, "\0");
			fin.read(reinterpret_cast<char*>(&m_ModelData.pBoneData[i].TransformationMatrix), sizeof(_float4x4));
			fin.read(reinterpret_cast<char*>(&m_ModelData.pBoneData[i].iParentIndex), sizeof(_int));
		}
	}
#pragma endregion

#pragma region Meshes
	fin.read(reinterpret_cast<char*>(&m_ModelData.iNumMeshes), sizeof(_uint));

	if (0 != m_ModelData.iNumMeshes)
	{
		m_ModelData.pMeshData = new MESHDATA[m_ModelData.iNumMeshes];

		for (_uint i = 0; i < m_ModelData.iNumMeshes; i++)
		{
			fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].iNameSize), sizeof(_uint));
			fin.read(m_ModelData.pMeshData[i].szName, m_ModelData.pMeshData[i].iNameSize);
			strcat_s(m_ModelData.pMeshData[i].szName, "\0");
			fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].iMaterialIndex), sizeof(_uint));
			fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].iNumVertices), sizeof(_uint));
			fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].iNumFaces), sizeof(_uint));

			if (0 != m_ModelData.pMeshData[i].iNumVertices)
			{
				m_ModelData.pMeshData[i].pMeshVtxData = new MESHVTXDATA[m_ModelData.pMeshData[i].iNumVertices];

				for (_uint j = 0; j < m_ModelData.pMeshData[i].iNumVertices; j++)
				{
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshVtxData[j].vPosition), sizeof(_float3));
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshVtxData[j].vNormal), sizeof(_float3));
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshVtxData[j].vTexUV), sizeof(_float2));
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshVtxData[j].vTangent), sizeof(_float3));
				}
			}

			if (0 != m_ModelData.pMeshData[i].iNumFaces)
			{
				m_ModelData.pMeshData[i].pMeshIdxData = new MESHIDXDATA[m_ModelData.pMeshData[i].iNumFaces];

				for (_uint j = 0; j < m_ModelData.pMeshData[i].iNumFaces; j++)
				{
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshIdxData[j].iIndex0), sizeof(_uint));
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshIdxData[j].iIndex1), sizeof(_uint));
					fin.read(reinterpret_cast<char*>(&m_ModelData.pMeshData[i].pMeshIdxData[j].iIndex2), sizeof(_uint));
				}
			}
		}
	}
#pragma endregion

#pragma region Meterials
	fin.read(reinterpret_cast<char*>(&m_ModelData.iNumMaterials), sizeof(_uint));
	if (0 != m_ModelData.iNumMaterials)
	{
		m_ModelData.pMaterialData = new MATERIALDATA[m_ModelData.iNumMaterials * TEXTURE_TYPE_MAX];

		for (_uint i = 0; i < m_ModelData.iNumMaterials; i++)
		{
			for (_uint j = 0; j < TEXTURE_TYPE_MAX; j++)
			{
				_uint iIndex = (i * TEXTURE_TYPE_MAX) + j;

				fin.read(reinterpret_cast<char*>(&m_ModelData.pMaterialData[iIndex].iNameSize), sizeof(_uint));

				if (1 >= m_ModelData.pMaterialData[iIndex].iNameSize)
					continue;
				else
				{
					fin.read(m_ModelData.pMaterialData[iIndex].szName, m_ModelData.pMaterialData[iIndex].iNameSize);
					strcat_s(m_ModelData.pMaterialData[iIndex].szName, "\0");
				}
			}
		}
	}
#pragma endregion

	fin.close();

	return S_OK;
}

HRESULT CModel_Instance::Ready_HierarchyBones()
{
	_uint iNumBones = m_ModelData.iNumBones;

	m_Bones.reserve(iNumBones);

	for (_uint i = 0; i < iNumBones; i++)
	{
		CBone* pBone = CBone::Create(&m_ModelData.pBoneData[i]);
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.emplace_back(pBone);
	}

	return S_OK;
}

HRESULT CModel_Instance::Ready_Meshes(_uint iNumInstance)
{
	m_iNumMeshes = m_ModelData.iNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh_Instance* pMesh = CMesh_Instance::Create(m_pDevice, m_pContext, &m_ModelData.pMeshData[i], XMLoadFloat4x4(&m_PivotMatrix), this , iNumInstance);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	
	return S_OK;
}

HRESULT CModel_Instance::Ready_Materials()
{
	m_iNumMaterials = m_ModelData.iNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MESHMATERIALS	MeshMaterial;
		ZeroMemory(&MeshMaterial, sizeof MeshMaterial);

		for (_uint j = 0; j < TEXTURE_TYPE_MAX; ++j)
		{
			_uint iIndex = (i * TEXTURE_TYPE_MAX) + j;

			_uint iSize = m_ModelData.pMaterialData[iIndex].iNameSize;
			char szFilePath[MAX_PATH] = { "" };

			_bool isNormal = { true };

			if (1 >= iSize)
			{
				if (MESHMATERIALS::TextureType_NORMALS == j)
				{
					strcpy_s(szFilePath, "../../Client/Bin/Resources/Models/AlphaTexture.dds");
					isNormal = false;
				}
				else
					continue;
			}
			else
			{
				strcpy_s(szFilePath, m_ModelData.pMaterialData[iIndex].szName);
			}

			char		szDrive[MAX_PATH] = "";
			char		szDir[MAX_PATH] = "";

			_splitpath_s(m_ModelFilePath.c_str(), szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);


			char		szFileName[MAX_PATH] = "";
			char		szExt[MAX_PATH] = "";

			_splitpath_s(szFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			char		szFullPath[MAX_PATH] = "";

			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szRealFullPath[MAX_PATH] = TEXT("");

			if (true == isNormal)
				MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szRealFullPath, MAX_PATH);
			else
				MultiByteToWideChar(CP_ACP, 0, szFilePath, (int)strlen(szFilePath), szRealFullPath, MAX_PATH);

			MeshMaterial.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, szRealFullPath);
			if (nullptr == MeshMaterial.pTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(MeshMaterial);
	}

	return S_OK;
}

HRESULT CModel_Instance::Clear_LoadData()
{
	//BoneData
	if (0 != m_ModelData.iNumBones)
		Safe_Delete_Array(m_ModelData.pBoneData);

	//MeshData
	for (_uint i = 0; i < m_ModelData.iNumMeshes; i++)
	{
		Safe_Delete_Array(m_ModelData.pMeshData[i].pMeshVtxData);
		Safe_Delete_Array(m_ModelData.pMeshData[i].pMeshIdxData);
		//if (0 != m_ModelData.pMeshData[i].iNumBones)
		//	Safe_Delete_Array(m_ModelData.pMeshData[i].pAnimMeshData);
	}
	if (0 != m_ModelData.iNumMeshes)
		Safe_Delete_Array(m_ModelData.pMeshData);

	//MaterialData
	if (0 != m_ModelData.iNumMaterials)
		Safe_Delete_Array(m_ModelData.pMaterialData);

	return S_OK;
}

void CModel_Instance::Tick(_double TimeDelta)
{
	for (_uint i = 0; i< m_iNumMeshes; ++i)
		m_Meshes[i]->Tick(TimeDelta);
}

CModel_Instance * CModel_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pModelFilePath, _fmatrix PivotMatrix, _uint iMaxNumInstance)
{
	CModel_Instance*		pInstance = new CModel_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PivotMatrix, iMaxNumInstance)))
	{
		MSG_BOX("Failed to Created : CModel_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CComponent * CModel_Instance::Clone(void * pArg)
{
	CModel_Instance*		pInstance = new CModel_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_Instance::Free()
{
	__super::Free();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
	m_Meshes.clear();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}

	m_Materials.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	Safe_Release(m_pVBInstance);
}
