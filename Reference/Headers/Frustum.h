#pragma once

#include "Base.h"

/* 카메라의 시야범위 : 절두체 (CFrustum) */

BEGIN(Engine)

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void Tick();

public:
	void Transform_ToLocalSpace(_fmatrix WorldMatrix);
	_bool isIn_WorldSpace(_fvector vWorldPos, _float fRange = 0.f);
	_bool isIn_LocalSpace(_fvector vLocalPos, _float fRange = 0.f) const;

private:
	_float3				m_vPoints_Original[8];
	_float3				m_vPoints_World[8];
	_float4				m_vPlane_World[6];
	_float4				m_vPlane_Local[6];

private:
	void Make_Plane(const _float3* pPoints, _Inout_ _float4* pPlanes);

public:
	virtual void Free() override;
};

END