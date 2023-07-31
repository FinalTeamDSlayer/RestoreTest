#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT		Initialize(LEVELID eNextLevelID);
	virtual void		Tick(_double dTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	LEVELID				m_eNextLevelID = { LEVEL_END };
	class CLoader*		m_pLoader = { nullptr };
	class CBackGround*	m_pBackGround = { nullptr };

public:
	static			CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void	Free() override;
};

END