#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel_Instance final : public CComponent
{

private:
	CModel_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel_Instance(const CModel_Instance& rhs);
	virtual ~CModel_Instance() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	ID3D11Buffer* Get_VBInstance() {
		return m_pVBInstance;
	}

	_uint Get_NumInstance() {
		return m_iNumInstance;
	}

public:
	virtual HRESULT Initialize_Prototype(const char* pModelFilePath, _fmatrix PivotMatrix, _uint iMaxNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Render(_uint iMeshIndex);
	HRESULT Bind_ShaderResource(class CShader* pShader, _uint iMeshIndex, const char* pConstantName, MESHMATERIALS::TEXTURETYPE eType);

	virtual void Tick(_double TimeDelta);

private:
	HRESULT	Ready_ModelData();
	HRESULT Ready_HierarchyBones();
	HRESULT Ready_Meshes(_uint iNumInstance);
	HRESULT Ready_Materials();
	HRESULT Clear_LoadData();

private:
	_float4x4								m_PivotMatrix;

private:
	_uint									m_iNumMeshes = { 0 };
	vector<class CMesh_Instance*>			m_Meshes;

private:
	_uint									m_iNumMaterials = { 0 };
	vector<MESHMATERIALS>					m_Materials;

private:
	MODELDATA								m_ModelData;
	string									m_ModelFilePath;

private:
	MODELINSTANCEDESC						m_tModelInstanceDesc;

private:
	D3D11_BUFFER_DESC						m_BufferDesc;
	D3D11_SUBRESOURCE_DATA					m_SubresourceData;

private:
	_uint									m_iNumInstance = { 0 };	// 몇개의 도형을 그릴것인가?
	_uint									m_iMaxNumInstance = { 0 };
	ID3D11Buffer*							m_pVBInstance = { nullptr };

private:
	vector<class CBone*>		m_Bones;

public:
	static CModel_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pModelFilePath, _fmatrix PivotMatrix, _uint iMaxNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END