#pragma once

#include "Tool_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Tool)

class CCamera_Tool final : public CCamera
{
	typedef struct tagCameraMouseState
	{
		_bool LB;

	}CAMERAMOUSESTATE;
private:
	CCamera_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Tool(const CCamera_Tool& rhs);
	virtual ~CCamera_Tool() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual			HRESULT Render() override;

private:
	_float4				m_vTargetPos;
	CAMERAMOUSESTATE	m_MouseState;

private:
	HRESULT		Add_Components();

	void		KeyInput(_double dTimeDelta);

public:
	static CCamera_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END