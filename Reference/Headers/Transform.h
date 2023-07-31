#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct tagTransformDesc
	{
		_double dSpeedPerSec;
		_double dRadianRotationPerSec;
	}TRANSFORMDESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public: //Getter
	_vector Get_State(STATE eState) { return XMLoadFloat4x4(&m_WorldMatrix).r[eState]; }

	_float3 Get_Scaled();

	_matrix		Get_WorldMatrix() { return XMLoadFloat4x4(&m_WorldMatrix); }
	_float4x4	Get_WorldFloat4x4() { return m_WorldMatrix; }

	_matrix		Get_WorldMatrix_Inverse();
	_float4x4	Get_WorldFloat4x4_Inverse();

	void	Set_State(STATE eState, _fvector vState);
	void	Set_Speed(_double dSpeed) { m_TransformDesc.dSpeedPerSec = dSpeed; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const char* pConstantName);

public:
	void Go_Straight(_double dTimeDelta);
	void Go_Backward(_double dTimeDelta);
	void Go_Right(_double dTimeDelta);
	void Go_Left(_double dTimeDelta);
	void Go_Up(_double dTimeDelta);
	void Go_Down(_double dTimeDelta);
	void Go_Dir(_double dTimeDelta, _fvector vDirection);	//vDirection 방향으로 이동
	void Go_Arc(_double dTimeDelta, _fvector vStartPosition, _fvector vArrivePosition, _float fArriveTime, _float fAngle);



public:
	void Translation(_float3 vTranslation);			//현재 위치에서 (Right, Up, Look)만큼 이동
	void Rotation(_fvector _vAxis, _float fDegree);	//초기상태 기준 Axis 축으로 디그리 각도만큼 회전
	void Rotation(_float3 vAngle); // 초기상태 기준 x, y, z 기준으로 디그리 각도만큼 회전
	void Turn(_float fDegree, _fvector vAxis);		//현재상태 기준 Axis 축으로 디그리 각도만큼 회전
	void Turn(_fvector vAxis, _double dTimeDelta);	//현재상태 기준 Axis 축으로 자체 회전속도로 회전
	void Scaling(const _float3& vScale);			//(Right, Up, Look)만큼 스케일 조정
	void Scaling(_float fScale);					//fScale만큼 전체 스케일 조정
	void Chase(_fvector vTargetPos, _double dTimeDelta, _float fMinDis);
	void Chase_Target(_fvector vTargetPos, _double dTimeDelta, _double ChaseSpeed = 1.0);
	void LookAt(_fvector vTargetPos);
	void Set_Look(_float4 vDir_0);
	void LerpVector(_fvector vTargetLook, _float weight);

private:
	_float4x4		m_WorldMatrix;
	TRANSFORMDESC	m_TransformDesc;

public:
	static CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END