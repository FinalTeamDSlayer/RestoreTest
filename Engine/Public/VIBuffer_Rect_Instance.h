#pragma once
#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagInstanceDesc
	{
		_float3		vRange;
		_float		fMinSpeed, fMaxSpeed;
	}INSTANCEDESC;
private:
	CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

private:
	_float*			m_pSpeed = { nullptr };
	INSTANCEDESC	m_InstanceDesc;

public:
	virtual void Tick(_double dTimeDelta) override;

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END