#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc{
		
		_float4		vEye;
		_float4		vAt;
		_float4		vAxisY;
		
		_float		fFovY, fAspect, fNearZ, fFarZ;

		_double		dSensitivity = { 0.5 };		//°¨µµ

		CTransform::TRANSFORMDESC TransformDesc;
	}CAMERADESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	_double Get_Sensitivity() { return m_CameraDesc.dSensitivity; }

	void	Set_Sensitivity(_double dSensitivity) { m_CameraDesc.dSensitivity += dSensitivity; }

	void	Set_Fov(_float fFovY) { m_CameraDesc.fFovY += fFovY; }


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double dTimeDelta);
	virtual void	LateTick(_double dTimeDelta);
	virtual			HRESULT Render();
	
public:
	void	Tick_Shake(_double TimeDelta);
	void	Shake(const _double& ShakeTime = 0.3, const _int& iShakePower = 100); // default  time = 0.3 / power = 100

protected:
	CTransform*			m_pTransformCom = { nullptr };
	CAMERADESC			m_CameraDesc;
	class CPipeLine*	m_pPipeLine = { nullptr };

protected: // Camera Shake
	_double				m_Time = { 0.0 };
	_double				m_ShakeTime = { 0.0 };
	_int				m_iShakePower = { 0 };
	_bool				m_bShake = { false };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END	