#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint	Get_MaterialIndex() const{
		return m_iMaterialIndex;
	}

	void	Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix, CModel* pModel);

public:
	const vector<_float4>&	Get_VertexPos() const { return m_VertexPos; }
	const vector<_uint3>	Get_Faces()		const { return m_Faces; }
public:
	HRESULT Initialize_Prototype(CModel::TYPE eModelType, MESHDATA* pMeshData, _fmatrix PivotMatrix, class CModel* pModel);
	HRESULT Initialize(void* pArg) override;

private:
	char	m_szName[MAX_PATH] = { "" };
	_uint	m_iMaterialIndex = { 0 };

private:
	_uint					m_iNumBones = { 0 };
	vector<_uint>			m_Bones;

private:
	vector<_float4>			m_VertexPos;
	vector<_uint3>			m_Faces;

private:
	HRESULT Ready_VertexBuffer_NonAnim(MESHDATA* pMeshData, _fmatrix PivotMatrix);
	HRESULT Ready_VertexBuffer_Anim(MESHDATA* pMeshData, CModel* pModel);

public:
	static CMesh* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, MESHDATA* pMeshData, _fmatrix PivotMatrix, class CModel* pModel);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
}; 

END