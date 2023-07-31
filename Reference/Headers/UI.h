#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUI abstract : public CGameObject
{
protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_float	Get_UI_Layer(void) {
		return m_UI_Layer;
	}

protected:
	_double					m_fX, m_fY, m_fSizeX, m_fSizeY = {};
	_float4x4				m_ProjMatrix = {};

	_int                   m_ScreenWidth = {};
	_int                   m_ScreenHeight = {};
	_int                   m_MouseX = {};
	_int                   m_MouseY = {};

	_bool                  m_Is_Render = { true };

protected:
	_bool	Pt_InUI();
	_float  m_UI_Layer = { 0 };



public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END