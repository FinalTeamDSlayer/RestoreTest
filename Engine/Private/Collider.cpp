#include "..\Public\Collider.h"

#include "DebugDraw.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
	, m_eColliderType(rhs.m_eColliderType)
#ifdef _DEBUG
	, m_pPrimitiveBatch(rhs.m_pPrimitiveBatch)
	, m_pEffects(rhs.m_pEffects)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG

}

void CCollider::ReMake_Collider(TYPE eColliderType, _fmatrix TransformMatrix)
{
	switch (m_eColliderType)
	{
	case TYPE_AABB:
		m_pAABB[DATA_ORIGIN]->Transform(*m_pAABB[DATA_ORIGIN], Remove_Rotation(TransformMatrix));
		break;
	case TYPE_OBB:
		m_pOBB[DATA_ORIGIN]->Transform(*m_pOBB[DATA_ORIGIN], TransformMatrix);
		break;
	case TYPE_SPHERE:
		m_pSphere[DATA_ORIGIN]->Transform(*m_pSphere[DATA_ORIGIN], TransformMatrix);
		break;
	}
}

void CCollider::ReMake_Collider(_float fRadius)
{
	m_pSphere[DATA_ORIGIN]->Radius = fRadius;
}

HRESULT CCollider::Initialize_Prototype(TYPE eColliderType)
{
	m_eColliderType = eColliderType;

#ifdef _DEBUG
	m_pPrimitiveBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pContext);
	m_pEffects = new BasicEffect(m_pDevice);

	m_pEffects->SetVertexColorEnabled(true);

	const void* pShaderByteCode = { nullptr };
	size_t iLength = { 0 };

	m_pEffects->GetVertexShaderBytecode(&pShaderByteCode, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, pShaderByteCode, iLength, &m_pInputLayout)))
		return E_FAIL;
#endif // _DEBUG


	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_ColliderDesc, pArg, sizeof m_ColliderDesc);

	for (_uint i = 0; i < DATA_END; i++)
	{
		switch (m_eColliderType)
		{
		case TYPE_AABB:
			m_pAABB[i] = new BoundingBox(m_ColliderDesc.vPosition, _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f));
			break;
		case TYPE_OBB:
			m_pOBB[i] = new BoundingOrientedBox(m_ColliderDesc.vPosition,
				_float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f),
				_float4(0.f, 0.f, 0.f, 1.f));

			m_pOBB[i]->Transform(*m_pOBB[i], XMMatrixRotationX(m_ColliderDesc.vRotation.x) * XMMatrixRotationY(m_ColliderDesc.vRotation.y) * XMMatrixRotationZ(m_ColliderDesc.vRotation.z));
			break;
		case TYPE_SPHERE:
			m_pSphere[i] = new BoundingSphere(m_ColliderDesc.vPosition, m_ColliderDesc.vSize.x);
			break;
		}
	}

	return S_OK;
}

void CCollider::Tick(_fmatrix TransformMatrix, _double dTimeDelta)
{
	switch (m_eColliderType)
	{
	case TYPE_AABB:
		m_pAABB[DATA_ORIGIN]->Transform(*m_pAABB[DATA_CURRENT], Remove_Rotation(TransformMatrix));
		break;
	case TYPE_OBB:
		m_pOBB[DATA_ORIGIN]->Transform(*m_pOBB[DATA_CURRENT], TransformMatrix);
		break;
	case TYPE_SPHERE:
		m_pSphere[DATA_ORIGIN]->Transform(*m_pSphere[DATA_CURRENT], TransformMatrix);
		break;
	}

	m_dCoolTime += dTimeDelta;
}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pEffects->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	m_pEffects->SetView(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffects->SetProjection(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	Safe_Release(pPipeLine);

	m_pEffects->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pPrimitiveBatch->Begin();

	_vector vColor = m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	switch (m_eColliderType)
	{
	case TYPE_AABB:
		DX::Draw(m_pPrimitiveBatch, *m_pAABB[DATA_CURRENT], vColor);
		break;
	case TYPE_OBB:
		DX::Draw(m_pPrimitiveBatch, *m_pOBB[DATA_CURRENT], vColor);
		break;
	case TYPE_SPHERE:
		DX::Draw(m_pPrimitiveBatch, *m_pSphere[DATA_CURRENT], vColor);
		break;
	}

	m_pPrimitiveBatch->End();

	return S_OK;
}
#endif // DEBUG

_bool CCollider::Intersect(CCollider* pTargetCollider)
{
	m_isColl = { false };
	pTargetCollider->m_isColl = { false };

	if (TYPE_AABB == m_eColliderType)
	{
		if (TYPE_AABB == pTargetCollider->m_eColliderType)
			m_isColl = m_pAABB[DATA_CURRENT]->Intersects(*pTargetCollider->m_pAABB[DATA_CURRENT]);
		if (TYPE_OBB == pTargetCollider->m_eColliderType)
			m_isColl = m_pOBB[DATA_CURRENT]->Intersects(*pTargetCollider->m_pOBB[DATA_CURRENT]);
		if (TYPE_SPHERE == pTargetCollider->m_eColliderType)
			m_isColl = m_pSphere[DATA_CURRENT]->Intersects(*pTargetCollider->m_pSphere[DATA_CURRENT]);
	}
	if (TYPE_OBB == m_eColliderType)
	{
		if (TYPE_AABB == pTargetCollider->m_eColliderType)
			m_isColl = m_pAABB[DATA_CURRENT]->Intersects(*pTargetCollider->m_pAABB[DATA_CURRENT]);
		if (TYPE_OBB == pTargetCollider->m_eColliderType)
			m_isColl = m_pOBB[DATA_CURRENT]->Intersects(*pTargetCollider->m_pOBB[DATA_CURRENT]);
		if (TYPE_SPHERE == pTargetCollider->m_eColliderType)
			m_isColl = m_pSphere[DATA_CURRENT]->Intersects(*pTargetCollider->m_pSphere[DATA_CURRENT]);
	}
	if (TYPE_SPHERE == m_eColliderType)
	{
		if (TYPE_AABB == pTargetCollider->m_eColliderType)
			m_isColl = m_pAABB[DATA_CURRENT]->Intersects(*pTargetCollider->m_pAABB[DATA_CURRENT]);
		if (TYPE_OBB == pTargetCollider->m_eColliderType)
			m_isColl = m_pOBB[DATA_CURRENT]->Intersects(*pTargetCollider->m_pOBB[DATA_CURRENT]);
		if (TYPE_SPHERE == pTargetCollider->m_eColliderType)
			m_isColl = m_pSphere[DATA_CURRENT]->Intersects(*pTargetCollider->m_pSphere[DATA_CURRENT]);
	}

	pTargetCollider->m_isColl = m_isColl;

	return m_isColl;
}

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_vector		vScale, vRotation, vTranslation;

	XMMatrixDecompose(&vScale, &vRotation, &vTranslation, TransformMatrix);

	return XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), vTranslation);
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);

	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffects);
		Safe_Delete(m_pPrimitiveBatch);
	}
#endif // _DEBUG


	for (_uint i = 0; i < DATA_END; i++)
	{
		Safe_Delete(m_pAABB[i]);
		Safe_Delete(m_pOBB[i]);
		Safe_Delete(m_pSphere[i]);
	}
}
