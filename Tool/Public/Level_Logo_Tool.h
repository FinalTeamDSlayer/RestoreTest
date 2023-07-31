#pragma once
#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class CLevel_Logo_Tool final : public CLevel
{
private:
	CLevel_Logo_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo_Tool() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual void		Tick(_double dTimeDelta) override;
	virtual HRESULT		Render() override;

private:
	HRESULT		Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static			CLevel_Logo_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void	Free() override;
};

END