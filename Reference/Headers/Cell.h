#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE {LINE_AB, LINE_BC, LINE_CA, LINE_END};

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	const _float3* Get_Point(POINT ePoint) const {
		return &m_vPoints[ePoint];
	}
	
	void Get_Points(_float3* pPoints) { memcpy(pPoints, m_vPoints, sizeof(_float3) * POINT_END); }

	void SetUp_Neighbor(LINE eLine, CCell* pNeigbor) {
		m_iNeighborIndex[eLine] = pNeigbor->m_iIndex;
	}

public:
	HRESULT	Initialize(const _float3* pPoints, _uint iIndex);
	_bool Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);
	_float3 CheckPoint(_float3 vTargetPos, _float fDistance);
	_vector	Sliding(_fvector vPosition, _fvector vCurrentPos);

#ifdef _DEBUG
public:
	HRESULT Render_Debug(const _float4* pColor, _float fY);
#endif // _DEBUG

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

#ifdef _DEBUG
private:
	class CVIBuffer_Cell*	m_pVIBuffer = { nullptr };
	class CShader*			m_pShader = { nullptr };

private:
	HRESULT Ready_Debug();
#endif // _DEBUG

private:
	_float3		m_vPoints[POINT_END];
	_float3		m_vNormals[LINE_END];
	_int		m_iIndex = { 0 };
	_int		m_iNeighborIndex[LINE_END] = { -1, -1, -1 };

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END