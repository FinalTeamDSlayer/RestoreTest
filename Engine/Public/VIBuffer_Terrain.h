#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	_uint	Get_VerticesX() { return m_iNumVerticesX; }
	_uint	Get_VerticesZ() { return m_iNumVerticesZ; }
	_float4* Get_VerticesPos() { return m_pVerticesPos; }
public:
	virtual HRESULT Initialize_Prototype(const _tchar * pHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_uint		m_iNumVerticesX = { 0 };
	_uint		m_iNumVerticesZ = { 0 };
	_float4*	m_pVerticesPos;
public:
	static CVIBuffer_Terrain* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END