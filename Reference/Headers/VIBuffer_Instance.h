#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
	protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& rhs);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual void Tick(_double dTimeDelta) = 0;
	virtual HRESULT Render();
public:

protected:
	_uint			m_iNumIndicesPerInstance = { 0 };
	_uint			m_iInstanceStride = { 0 };
	_uint			m_iNumInstance = { 0 };
	_uint			m_iMaxNumInstance = { 0 };
	ID3D11Buffer*	m_pVBInstance = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END