#include "..\Public\Frustum.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

HRESULT CFrustum::Initialize()
{
	/* 투영스페이스 상의 보여지는 범위(카메라 시야범위(절두체))를 지정했다. */
	m_vPoints_Original[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoints_Original[1] = _float3(1.f, 1.f, 0.f);
	m_vPoints_Original[2] = _float3(1.f, -1.f, 0.f);
	m_vPoints_Original[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoints_Original[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoints_Original[5] = _float3(1.f, 1.f, 1.f);
	m_vPoints_Original[6] = _float3(1.f, -1.f, 1.f);
	m_vPoints_Original[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Tick()
{
	/* 월드스페이스 까지 내린다. */
	CPipeLine*			pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_matrix		ProjMatrixInv = pPipeLine->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);
	_matrix		ViewMatrixInv = pPipeLine->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	_vector				vPoints[8];

	for (size_t i = 0; i < 8; i++)
	{
		vPoints[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoints_Original[i]), ProjMatrixInv);
		vPoints[i] = XMVector3TransformCoord(vPoints[i], ViewMatrixInv);

		XMStoreFloat3(&m_vPoints_World[i], vPoints[i]);
	}

	Make_Plane(m_vPoints_World, m_vPlane_World);

	Safe_Release(pPipeLine);
}

void CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrix)
{
	_matrix		WorldMatrixInv = XMMatrixInverse(nullptr, WorldMatrix);

	_float3		vPoint[8];

	/* 월드 공간에 있는 점들을 로컬로 이동시키낟. */
	for (size_t i = 0; i < 8; i++)
	{
		XMStoreFloat3(&vPoint[i],
			XMVector3TransformCoord(XMLoadFloat3(&m_vPoints_World[i]), WorldMatrixInv));
	}

	/* 로컬 점들을 이용하여 로컬상의 평면여섯개를 구성한다. */
	Make_Plane(vPoint, m_vPlane_Local);
}

_bool CFrustum::isIn_WorldSpace(_fvector vWorldPos, _float fRange)
{
	// ax + by + cz + d = ?
	for (size_t i = 0; i < 6; i++)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane_World[i]), vWorldPos)))
			return false;
	}
	return true;
}

_bool CFrustum::isIn_LocalSpace(_fvector vLocalPos, _float fRange) const
{
	// ax + by + cz + d = ?
	for (size_t i = 0; i < 6; i++)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlane_Local[i]), vLocalPos)))
			return false;
	}
	return true;
	return _bool();}

void CFrustum::Make_Plane(const _float3 * pPoints, _Inout_ _float4* pPlanes)
{
	/*+x*/
	XMStoreFloat4(&pPlanes[0]
	, XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[1]), 
		XMLoadFloat3(&pPoints[5]), 
		XMLoadFloat3(&pPoints[6])));

	/*-x*/
	XMStoreFloat4(&pPlanes[1]
		, XMPlaneFromPoints(
			XMLoadFloat3(&pPoints[4]),
			XMLoadFloat3(&pPoints[0]),
			XMLoadFloat3(&pPoints[3])));

	/*+y*/
	XMStoreFloat4(&pPlanes[2]
		, XMPlaneFromPoints(
			XMLoadFloat3(&pPoints[4]),
			XMLoadFloat3(&pPoints[5]),
			XMLoadFloat3(&pPoints[1])));

	/*-y*/
	XMStoreFloat4(&pPlanes[3]
		, XMPlaneFromPoints(
			XMLoadFloat3(&pPoints[3]),
			XMLoadFloat3(&pPoints[2]),
			XMLoadFloat3(&pPoints[6])));

	/*+z*/
	XMStoreFloat4(&pPlanes[4]
		, XMPlaneFromPoints(
			XMLoadFloat3(&pPoints[5]),
			XMLoadFloat3(&pPoints[4]),
			XMLoadFloat3(&pPoints[7])));

	/*-z*/
	XMStoreFloat4(&pPlanes[5]
		, XMPlaneFromPoints(
			XMLoadFloat3(&pPoints[0]),
			XMLoadFloat3(&pPoints[1]),
			XMLoadFloat3(&pPoints[2])));
}

void CFrustum::Free()
{

}

