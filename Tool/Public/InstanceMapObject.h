#pragma once

#include "Tool_Defines.h"
#include "MapObject.h"

BEGIN(Engine)
class CModel_Instance;
END

BEGIN(Tool)

class CInstanceMapObject  final : public CMapObject
{
private:
	CInstanceMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInstanceMapObject(const CInstanceMapObject& rhs);
	virtual ~CInstanceMapObject() = default;

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
	 CModel_Instance* m_pModelInstanceCom = { nullptr };

private:
	_float				m_fTimeDelta = { 0.f };

public:
	static CInstanceMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
