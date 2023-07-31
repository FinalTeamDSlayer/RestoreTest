#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCamera_Free final : public CCamera
{
private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual			HRESULT Render() override;


public:
	_float4		Get_CameraLook() { return m_fCameraLook; }

private:
	void FreeCamera(_double dTimeDelta);
	void AdventureCamera(_double dTimeDelta);
	void BattleCamera(_double dTimeDelta);
	void CutInCamera(_double dTimeDelta);

private:
	void Turn_Camera(_double TimeDelta);
	void LockMouse();

private:
	_bool			m_bCamChange = { true };
	_bool			m_bLockMouse = { true };
	_bool           m_Is_Battle = { true };
	_bool           m_Is_Cut_In = { false };
	_bool           m_bIs_Combo_On = { false };

private:
	_float4			m_fCameraLook = {};
	_float			m_fDistance = {};
	_vector         m_vOffSet = {};
	_vector         m_vLookOffSet = {};
	_vector         m_vDist = { 0.f, 0.f, 1.f, 0.f };

	_vector         m_vTargetPos = {};
	_vector         m_vBattleTargetPos = {};
	_vector         m_vBattleCenter = {};

	_float          m_vCameraAngle = { 15.f };
	_float          m_vAdventureCameraAngle = { 0.f };

	_float          m_fDamping = { 5.f };
	
	_float          m_fLookDamping = { 7.f };


public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END