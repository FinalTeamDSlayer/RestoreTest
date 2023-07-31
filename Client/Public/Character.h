#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "Navigation.h"
#include "Transform.h"
#include "Collider.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
END

BEGIN(Client)

class CCharacter abstract : public CLandObject
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
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

public:
	//virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT	Read_Animation_Control_File(const char* szBinfilename);
	void	RootAnimation(_double dTimeDelta);
	_bool	EventCallProcess( );
	void	Reset_Decleration(_float fResetSpeed);

	//이동 관련
	void	Go_Straight_Deceleration(_double dTimeDelta, _int AnimIndex, _float ResetSpeed, _float fDecrease);
	void	Go_Left_Deceleration(_double dTimeDelta, _int AnimIndex, _float ResetSpeed, _float fDecrease);
	void	Go_Right_Deceleration(_double dTimeDelta, _int AnimIndex, _float ResetSpeed, _float fDecrease);
	void	Go_Dir_Deceleration(_double dTimeDelta, _int AnimIndex, _float ResetSpeed, _float fDecrease, _float4 Dir);
	void	Go_Straight_Constant(_double dTimeDelta, _int AnimIndex, _float constantSpeed);
	void	Gravity(_double dTimeDelta);
	void	Ground_Animation_Play(_int CurAnim, _int GroundAnim);
	void	Jumping( _float ResetSpeed, _float fFallDecrease);
	void	JumpStop(_double dDuration);


protected:
	void	Set_FallingStatus(_float fFallSpeed, _float fGravityAcc) { m_fJump_Acc = -fFallSpeed; m_fGravity_Fall = fGravityAcc; }
	
protected:
	CHARACTERDESC	m_CharacterDesc;

protected:
	CModel* m_pModelCom = { nullptr };		
	CShader* m_pShaderCom = { nullptr };	
	CRenderer* m_pRendererCom = { nullptr };
	CCollider* m_pColliderCom[CCollider::TYPE_END] = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

protected:
	_float4		m_Save_RootPos = { 0.0f, 0.0f, 0.0f, 1.0f };


	//Attack MoveControl
	_float	m_fAtk_MoveControl = { 0.0f };
	_bool	m_isReset_Atk_MoveControl = { false };

	//Jump 
	_float	m_fGravity_Fall = { 0.1f};
	_double m_dDelay_Fall = { 0.0 };
	_bool	m_isJumpOn = { false };
	_float	m_fJump_Acc = { 0.0f };

	_bool	m_isJumpStop = { false };
	_double m_dTime_JumpStop = { 0.0 };
	_double	m_dTime_JumpStop_Duration = { 0.0 };

	_bool	m_isFirst_JumpAtk = { true };

	//EventCallIndex
	_int	m_iEvent_Index = { 0 };


protected:
	HRESULT Add_Components();
	void	SetUp_Height();
	void	Tick_Collider(_double dTimeDelta);


public:
	//virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

};

END