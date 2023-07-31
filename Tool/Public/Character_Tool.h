#pragma once

#include "Tool_Defines.h"
#include "LandObject.h"
#include "Navigation.h"
#include "Transform.h"
#include "Collider.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
END

BEGIN(Tool)

class CCharacter_Tool abstract : public CLandObject
{
public:
	enum COLLIDER { COLL_AABB, COLL_OBB, COLL_SPHERE, COLL_END };
public:
	typedef struct tagCharacterDesc
	{
		CGameObject::WORLDINFO		WorldInfo;
		CCollider::COLLIDERDESC		ColliderDesc[COLL_END];
		CTransform::TRANSFORMDESC	TransformDesc;
		CNavigation::NAVIDESC		NaviDesc;
	}CHARACTERDESC;
protected:
	CCharacter_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter_Tool(const CCharacter_Tool& rhs);
	virtual ~CCharacter_Tool() = default;

public:

public:
	//virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CHARACTERDESC	m_CharacterDesc;

protected:
	CModel* m_pModelCom = { nullptr };		
	CShader* m_pShaderCom = { nullptr };	
	CRenderer* m_pRendererCom = { nullptr };
	CCollider* m_pColliderCom[CCollider::TYPE_END] = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

protected:
	HRESULT Add_Components();
	void	SetUp_Height();
	void	Tick_Collider(_double dTimeDelta);

public:
	//virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

};

END