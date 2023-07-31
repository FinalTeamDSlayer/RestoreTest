#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	_bool	Get_IsStage()  const {
		return m_isStage;
	}
	void	Set_IsStage(_bool isStage) {
		m_isStage = isStage;
	}

public:
	virtual HRESULT		Initialize();
	virtual void		Tick(_double dTimeDelta);
	virtual HRESULT		Render();
protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_bool	m_isStage = { false };
public:
	virtual void Free() override;
};

END