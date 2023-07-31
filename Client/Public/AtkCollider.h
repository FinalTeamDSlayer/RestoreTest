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
public:
	typedef struct tagAtkCollDesc
	{
		CTransform* pTransform = { nullptr };
		CCollider::COLLIDERDESC ColliderDesc;
		_double		dLifeTime = { 0.0 };

		_float		fDamage = { 0.0 };
	}ATKCOLLDESC;
private:
	CAtkCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAtkCollider(const CAtkCollider& rhs);
	virtual ~CAtkCollider() = default;

public:
	void Reset_AtkCollider(ATKCOLLDESC* pAtkCollDesc);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double dTimeDelta) override;
	virtual void LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	ATKCOLLDESC		m_AtkCollDesc;
	_double			m_dTimeAcc = { 0.0 };

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