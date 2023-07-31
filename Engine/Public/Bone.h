#pragma once
#include "Base.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const char* Get_Name() const { return m_szName; }

	_matrix Get_OffsetMatrix() const { return XMLoadFloat4x4(&m_OffsetMatrix); }

	_matrix Get_CombinedTransformationMatrix() const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

public:
	HRESULT		Initialize(BONEDATA* pModelData);
	void		Invalidate_CombinedTransformationMatrix(class CModel* pModel);

private:
	char		m_szName[MAX_PATH] = { "" };
	_float4x4	m_OffsetMatrix;
	_float4x4	m_TransformationMatrix;
	_float4x4	m_CombinedTransformationMatrix;
	_int		m_iParentIndex = { -1 };

public:
	static CBone* Create(BONEDATA* pModelData);
	CBone* Clone();
	virtual void Free() override;
};

END