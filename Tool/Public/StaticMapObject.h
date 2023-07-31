#pragma once

#include "Tool_Defines.h"
#include "MapObject.h"

BEGIN(Tool)

class CStaticMapObject  final : public CMapObject
{
private:
	CStaticMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStaticMapObject(const CStaticMapObject& rhs);
	virtual ~CStaticMapObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CStaticMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
