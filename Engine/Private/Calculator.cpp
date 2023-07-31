#include "..\Public\Calculator.h"

#include "GameInstance.h"

#include "VIBuffer_Terrain.h"
#include "Mesh.h"

CCalculator::CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CCalculator::CCalculator(const CCalculator& rhs)
	: CComponent(rhs)
{
}

HRESULT CCalculator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCalculator::Initialize(void* pArg)
{
	return S_OK;
}

_float4 CCalculator::Picking_Plane(HWND hWnd, _uint iWindSizeX, _uint iWinSizeY, CVIBuffer_Terrain* pVIBuffer)
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_float4 vMousePos = {
	ptMouse.x / (iWindSizeX * 0.5f) - 1.f,
	ptMouse.y / -(iWinSizeY * 0.5f) + 1.f,
	0.f,
	1.f
	};

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	XMStoreFloat4(&vMousePos, XMVector3TransformCoord(XMLoadFloat4(&vMousePos), pGameInstance->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ)));

	XMStoreFloat4(&vMousePos, XMVector3TransformCoord(XMLoadFloat4(&vMousePos), pGameInstance->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW)));

	_float4 vCameraPos = pGameInstance->Get_CameraPosition();
	_vector vRayPos = XMLoadFloat4(&vCameraPos);
	_vector	vRayDir = XMVector4Normalize(XMLoadFloat4(&vMousePos) - vRayPos);

	Safe_Release(pGameInstance);

	_float fDis;

	_uint iNumVerticesX = pVIBuffer->Get_VerticesX();
	_uint iNumVerticesZ = pVIBuffer->Get_VerticesZ();

	const _float4* pVerticesPos = pVIBuffer->Get_VerticesPos();

	_float4 vIntersectPoint = { 0.f, 0.f, 0.f, 0.f };

	for (_uint i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; j++)
		{
			_uint iIndex = i * iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + iNumVerticesX,
				iIndex + iNumVerticesX + 1,  
				iIndex + 1,
				iIndex
			};

			if (Intersects(vRayPos, vRayDir, XMLoadFloat4(&pVerticesPos[iIndices[1]]), XMLoadFloat4(&pVerticesPos[iIndices[0]]), XMLoadFloat4(&pVerticesPos[iIndices[2]]), fDis))
			{
				XMStoreFloat4(&vIntersectPoint, (vRayPos + vRayDir * fDis));

				return vIntersectPoint;
			}

			if (Intersects(vRayPos, vRayDir, XMLoadFloat4(&pVerticesPos[iIndices[3]]), XMLoadFloat4(&pVerticesPos[iIndices[2]]), XMLoadFloat4(&pVerticesPos[iIndices[0]]), fDis))
			{
				XMStoreFloat4(&vIntersectPoint, (vRayPos + vRayDir * fDis));

				return vIntersectPoint;
			}
		}
	}

	return vIntersectPoint;
}

_float4 CCalculator::Picking_Model(HWND hWnd, _uint iWindSizeX, _uint iWinSizeY, CModel* pModel)
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_float4 vMousePos = {
	ptMouse.x / (iWindSizeX * 0.5f) - 1.f,
	ptMouse.y / -(iWinSizeY * 0.5f) + 1.f,
	0.f,
	1.f
	};

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	XMStoreFloat4(&vMousePos, XMVector3TransformCoord(XMLoadFloat4(&vMousePos), pGameInstance->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ)));

	XMStoreFloat4(&vMousePos, XMVector3TransformCoord(XMLoadFloat4(&vMousePos), pGameInstance->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW)));

	_float4 vCameraPos = pGameInstance->Get_CameraPosition();
	_vector vRayPos = XMLoadFloat4(&vCameraPos);
	_vector	vRayDir = XMVector4Normalize(XMLoadFloat4(&vMousePos) - vRayPos);

	Safe_Release(pGameInstance);

	_float fMinDis = D3D11_FLOAT32_MAX;

	_uint iNumMeshes = pModel->Get_NumMeshes();

	_float4 vIntersectPoint = { 0.f, 0.f, 0.f, 0.f };

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		CMesh* pMesh = pModel->Get_Mesh(i);

		vector<_float4> VertexPos = pMesh->Get_VertexPos();
		vector<_uint3> Faces = pMesh->Get_Faces();

		_uint iNumFaces = (_uint)Faces.size();

		for (_uint j = 0; j < iNumFaces; j++)
		{
			_float fDis;

			if (Intersects(vRayPos, vRayDir, XMLoadFloat4(&VertexPos[Faces[j].x]), XMLoadFloat4(&VertexPos[Faces[j].y]), XMLoadFloat4(&VertexPos[Faces[j].z]), fDis))
			{
				if (D3D11_FLOAT32_MAX == fMinDis)
					fMinDis = fDis;

				fMinDis = min(fDis, fMinDis);
			}
		}
	}

	if (D3D11_FLOAT32_MAX != fMinDis)
		XMStoreFloat4(&vIntersectPoint, (vRayPos + vRayDir * fMinDis));

	return vIntersectPoint;
}

CCalculator* CCalculator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCalculator* pInstance = new CCalculator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCalculator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCalculator::Clone(void* pArg)
{
	CCalculator* pInstance = new CCalculator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCalculator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCalculator::Free()
{
	__super::Free();
}
