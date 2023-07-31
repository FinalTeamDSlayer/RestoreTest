#pragma once
#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class CLevel_Loading_Tool final : public CLevel
{
private:
	CLevel_Loading_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading_Tool() = default;

public:
	virtual HRESULT		Initialize(LEVELID eNextLevelID);
	virtual void		Tick(_double dTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	LEVELID				m_eNextLevelID = { LEVEL_END };
	class CLoader_Tool* m_pLoader = { nullptr };

public:
	static			CLevel_Loading_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void	Free() override;
};

END 