#pragma once

#include "Tool_Defines.h"
#include "MapObject.h"

BEGIN(Tool)

class CTerrainMapObject  final : public CMapObject
{
private:
	CTerrainMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrainMapObject(const CTerrainMapObject& rhs);
	virtual ~CTerrainMapObject() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	CTexture* m_pSplatingTexture = { nullptr };
	CTexture* m_pMaskTexture = { nullptr };

public:
	static CTerrainMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
