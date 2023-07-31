#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"

BEGIN(Engine)

#ifdef _DEBUG
class CRenderer;
#endif // _DEBUG
END

BEGIN(Client)

class CAtkCollider final : public CGameObject
{
private:
	CAtkCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAtkCollider(const CAtkCollider& rhs);
	virtual ~CAtkCollider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double dTimeDelta) override;
	virtual void LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

#ifdef _DEBUG
private:
	CRenderer* m_pRendererCom = { nullptr };
#endif // _DEBUG

private:
	HRESULT Add_Components();
public:
	static CAtkCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END