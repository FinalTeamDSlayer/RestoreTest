#pragma once

#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_matrix		Get_TransformMatrix(D3DTRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_TransformState[eState]);
	}
	_float4x4	Get_TransformFloat4x4(D3DTRANSFORMSTATE eState) const {
		return m_TransformState[eState];
	}

	_matrix		Get_TransformMatrix_Inverse(D3DTRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_TransformState_Inverse[eState]);
	}
	_float4x4	Get_TransformFloat4x4_Inverse(D3DTRANSFORMSTATE eState) const {
		return m_TransformState_Inverse[eState];
	}

	_float4		Get_CameraPosition() {
		return *(_float4*)&m_TransformState_Inverse[D3DTS_VIEW].m[3][0];
	}

public:
	void		Set_Transform(D3DTRANSFORMSTATE eState, const _float4x4& TransformMatrix) {
		m_TransformState[eState] = TransformMatrix;
	}

public:
	void Tick();

private:
	_float4x4	m_TransformState[D3DTS_END];
	_float4x4	m_TransformState_Inverse[D3DTS_END];

public:
	virtual void Free() override;
};

END