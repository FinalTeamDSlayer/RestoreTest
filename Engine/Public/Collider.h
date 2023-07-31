#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum DATA { DATA_ORIGIN, DATA_CURRENT, DATA_END};

public:
	typedef struct tagColliderDesc
	{
		_float3 vPosition;
		_float3 vSize;
		_float3 vRotation;
	}COLLIDERDESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	_float Get_Collider() {
		return m_pSphere[DATA_ORIGIN]->Radius;
	}
	_bool	Get_Coll() {
		return m_isColl;
	}

public:
	void	Set_Coll(_bool isColl) {
		m_isColl = isColl;
	}

public:
	void ReMake_Collider(TYPE eColliderType, _fmatrix TransformMatrix);
	void ReMake_Collider(_float fRadius);

public:
	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg) override;
	void			Tick(_fmatrix TransformMatrix, _double dTimeDelta);

public:
	_bool Intersect(CCollider* pTargetCollider);

#ifdef _DEBUG
public:
	virtual HRESULT			Render() override;
#endif // _DEBUG

private:
	BoundingBox*			m_pAABB[DATA_END] = { nullptr };
	BoundingOrientedBox*	m_pOBB[DATA_END] = { nullptr };
	BoundingSphere*			m_pSphere[DATA_END] = { nullptr };

	TYPE					m_eColliderType = { TYPE_END };
	COLLIDERDESC			m_ColliderDesc;
	_bool					m_isColl = { false };
	_double					m_dCoolTime = { 3.0 };

#ifdef _DEBUG
private:
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pPrimitiveBatch = { nullptr };
	BasicEffect*									m_pEffects = { nullptr };
	ID3D11InputLayout*								m_pInputLayout = { nullptr };
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END