#pragma once

#include "VIBuffer_Instance.h"
#include "Model_Instance.h"

BEGIN(Engine)

class CMesh_Instance final : public CVIBuffer_Instance
{
private:
	CMesh_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh_Instance(const CMesh_Instance& rhs);
	virtual ~CMesh_Instance() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	void Set_NumInstance(_uint iNumInstance) { m_iNumInstance = iNumInstance; }

public:
	virtual HRESULT Initialize_Prototype(MESHDATA* pMeshData, _fmatrix PivotMatrix, class CModel_Instance* pModel, _uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();
	HRESULT Bind_Resources();

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iMaterialIndex = { 0 };

private:
	_float4x4				m_PivotMatrix;
	class CModel_Instance* m_pModel = { nullptr };

private:
	vector<_float4>			m_VertexPos;
	vector<_uint3>			m_Faces;

private:
	_uint					m_iNumFaces = { 0 };

private:
	ifstream*				 m_pFin = { nullptr };

private:
	_uint			m_iMaxNumInstance = { 0 };
	_uint			m_iNumIndicesPerInstance = { 0 };
	_uint			m_iInstanceStride = { 0 };

private:
	HRESULT Ready_VertexBuffer_NonAnim(MESHDATA* pMeshData, _fmatrix PivotMatrix);

public:
	static CMesh_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MESHDATA* pMeshData, _fmatrix PivotMatrix, class CModel_Instance* pModel, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END