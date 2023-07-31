#pragma once
#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance abstract : public CVIBuffer_Instance
{
public:
	typedef struct tagInstanceDesc
	{
		_float3		vRange;
		_float		fMinScale, fMaxScale;
		_float		fMinSpeed, fMaxSpeed;
	}INSTANCEDESC;
protected:
	CVIBuffer_Point_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	
public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

protected:
	_float* m_pScale = { nullptr };
	_float* m_pSpeed = { nullptr };
	_float* m_pAngle = { nullptr };
	INSTANCEDESC		m_InstanceDesc;

public:
	//virtual void Tick(_double dTimeDelta) override;

public:
	/*static CVIBuffer_Point_Instance* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;*/
	virtual void Free() override;
};

END