#pragma once

#include "Tool_Defines.h"
#include "Character_Tool.h"

BEGIN(Engine)

END

BEGIN(Tool)

class CPlayer_Tool final : public CCharacter_Tool
{
private:
	CPlayer_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Tool(const CPlayer_Tool& rhs);
	virtual ~CPlayer_Tool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	/* 임시 코드 */
	_uint	m_iNumAnim = { 0 };

private:
	HRESULT Add_Components();
	HRESULT	SetUp_ShaderResources();

public:
	static CPlayer_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END