#pragma once

#include "Client_Defines.h"
#include "MapObject.h"

BEGIN(Client)

class CRotationMapObject  final : public CMapObject
{
private:
	CRotationMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRotationMapObject(const CRotationMapObject& rhs);
	virtual ~CRotationMapObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CRotationMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
