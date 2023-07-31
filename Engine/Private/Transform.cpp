#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
	, m_TransformDesc(rhs. m_TransformDesc)
{
}

_float3 CTransform::Get_Scaled()
{
	_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		
	return _float3(XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_RIGHT])),
					XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_UP])),
					XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_LOOK])));
}

_matrix CTransform::Get_WorldMatrix_Inverse()
{
	_matrix WorldMatrixInverse = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));

	return WorldMatrixInverse;
}

_float4x4 CTransform::Get_WorldFloat4x4_Inverse()
{
	_float4x4 WorldMatrixInverse;

	XMStoreFloat4x4(&WorldMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	return WorldMatrixInverse;
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	_float4		vTemp;

	XMStoreFloat4(&vTemp, vState);

	memcpy(&m_WorldMatrix.m[eState][0], &vTemp, sizeof(_float4));
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	ZeroMemory(&m_TransformDesc, sizeof m_TransformDesc);

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof m_TransformDesc);

	return S_OK;
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const char* pConstantName)
{
	if (nullptr == pShader)
		return E_FAIL;

	return pShader->SetUp_Matrix(pConstantName, &m_WorldMatrix);
}

void CTransform::Go_Straight(_double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vUp = {0.0f, 1.0f, 0.0f, 0.0f};

	vPosition += XMVector3Normalize(vUp) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vDown = { 0.0f, -1.0f, 0.0f, 0.0f };

	vPosition += XMVector3Normalize(vDown) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Dir(_double dTimeDelta, _fvector vDirection)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vDir = vDirection;

	vPosition += XMVector3Normalize(vDir) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);
	
	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Arc(_double dTimeDelta, _fvector vStartPosition, _fvector vArrivePosition, _float fArriveTime, _float fAngle)
{
	/* StartPosition에서 ArrivePosition까지
	fAngle각도로 발사하는 포물선이 도착하는 시간 fArriveTime */

	_vector vDir = XMVectorSetY(vArrivePosition, 0.f) - XMVectorSetY(vStartPosition, 0.f);

	_float fDis = XMVectorGetX(XMVector3Length(vDir));

	_float fVelocityXZ = fDis / fArriveTime;
	_float fVelocityY = fVelocityXZ * tanf(XMConvertToRadians(fAngle));

	_float fDiffY = (XMVectorGetY(vArrivePosition) - XMVectorGetY(vStartPosition));

	_float fGravity = ((fVelocityY * fArriveTime - fDiffY) * 2.f) / (fArriveTime * fArriveTime);

	_vector vPosition = fVelocityXZ * XMVector3Normalize(vDir) * (_float)dTimeDelta;

	_float fHeight = ((fVelocityY * (_float)dTimeDelta) + ((_float)dTimeDelta * (_float)dTimeDelta * -fGravity / 2.f));

	vPosition = XMVectorSetY(vPosition, fHeight);

	Set_State(CTransform::STATE_POSITION, vStartPosition + vPosition);
}

void CTransform::Translation(_float3 vTranslation)
{
	_vector vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	_vector vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));

	_vector vPosition = Get_State(CTransform::STATE_POSITION);

	vRight = vRight * vTranslation.x;
	vUp = vUp * vTranslation.y;
	vLook = vLook * vTranslation.z;

	vPosition = vPosition + vRight + vUp + vLook;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Rotation(_fvector _vAxis, _float fDegree)
{
	_float3 vScale = Get_Scaled();

	_vector vAxis = XMVector3Normalize(_vAxis);

	_vector vRight	= XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector vUp		= XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector vLook	= XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	if (0.f == XMVectorGetX(vAxis) && 0.f == XMVectorGetY(vAxis) && 0.f == XMVectorGetZ(vAxis))
		return;
	else
	{
		_matrix RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fDegree));

		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
	}
}

void CTransform::Rotation(_float3 vAngle)
{
	_float3		vScale = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_vector		vAxisX = { 1.f, 0.f, 0.f, 0.f };
	_vector		vAxisY = { 0.f, 1.f, 0.f, 0.f };
	_vector		vAxisZ = { 0.f, 0.f, 1.f, 0.f };

	_matrix		RotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(vAngle.x), XMConvertToRadians(vAngle.y), XMConvertToRadians(vAngle.z));

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn(_float fDegree, _fvector vAxis)
{
	_vector vRight = Get_State(CTransform::STATE_RIGHT);
	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	_matrix RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fDegree));

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn(_fvector vAxis, _double dTimeDelta)
{
	_vector vRight	= Get_State(STATE_RIGHT);
	_vector vUp		= Get_State(STATE_UP);
	_vector vLook	= Get_State(STATE_LOOK);

	_matrix RotationMatrix = XMMatrixRotationAxis(vAxis, _float(m_TransformDesc.dRadianRotationPerSec * dTimeDelta));

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Scaling(const _float3& vScale)
{
	_vector vRight	= XMVector3Normalize(Get_State(STATE_RIGHT));
	_vector vUp		= XMVector3Normalize(Get_State(STATE_UP));
	_vector vLook	= XMVector3Normalize(Get_State(STATE_LOOK));

	Set_State(STATE_RIGHT, (vRight * vScale.x));
	Set_State(STATE_UP, (vUp * vScale.y));
	Set_State(STATE_LOOK, (vLook * vScale.z));
}

void CTransform::Scaling(_float fScale)
{
	_vector vRight = XMVector3Normalize(Get_State(CTransform::STATE_RIGHT));
	_vector vUp = XMVector3Normalize(Get_State(CTransform::STATE_UP));
	_vector vLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));

	Set_State(CTransform::STATE_RIGHT, (vRight * fScale));
	Set_State(CTransform::STATE_UP, (vUp * fScale));
	Set_State(CTransform::STATE_LOOK, (vLook * fScale));
}

void CTransform::Chase(_fvector vTargetPos, _double dTimeDelta, _float fMinDis)
{
	_vector vPosition = Get_State(STATE_POSITION);

	_vector vDir = vTargetPos - vPosition;

	_vector vLength = XMVector3Length(vDir);

	if (fMinDis < XMVectorGetX(vLength))
		vPosition += XMVector3Normalize(vDir) * _float(m_TransformDesc.dSpeedPerSec * dTimeDelta);
}

void CTransform::Chase_Target(_fvector vTargetPos, _double dTimeDelta, _double ChaseSpeed)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vDir = vTargetPos - vPosition;
	_double		Speed = m_TransformDesc.dSpeedPerSec * ChaseSpeed * dTimeDelta;


	if (XMVectorGetX(XMVector3Length(vDir)) <= (_float)Speed)
	{
		vPosition += vDir;
	}
	else
		vPosition += XMVector3Normalize(vDir) * (_float)Speed;

	Set_State(CTransform::STATE_POSITION, vPosition);
}



void CTransform::LerpVector(_fvector vTargetLook, _float weight)
{
	_float3      vScale = Get_Scaled();


	_vector      vCurrentLook = Get_State(CTransform::STATE_LOOK);
	_vector      vInterPolatedLook = XMVectorLerp(vCurrentLook, vTargetLook, weight);

	_vector      vLook = XMVector3Normalize(vInterPolatedLook) * vScale.z;

	_vector      vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;

	_vector      vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

}

void CTransform::LookAt(_fvector vTargetPos)
{
	_float3		vScale = Get_Scaled();

	_vector		vLook = XMVector3Normalize(vTargetPos - Get_State(CTransform::STATE_POSITION)) * vScale.z;

	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;

	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}



void CTransform::Set_Look(_float4 vDir_0)
{
	_vector Look = XMLoadFloat4(&vDir_0);

	_float3		vScale = Get_Scaled();

	_vector		vLook = XMVector3Normalize(Look) * vScale.z;

	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;

	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
