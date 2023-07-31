#include "..\Public\Navigation.h"
#include "Transform.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation& rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFile)
{
	_ulong		dwByte = { 0 };
	HANDLE		hFile = CreateFile(pNavigationDataFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	_float3 vPoints[CCell::POINT_END];

	while (true)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_Cells.size());
		if (nullptr == pCell)
			break;

		m_Cells.push_back(pCell);
	}

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr != pArg)
		m_NaviDesc = *(NAVIDESC*)pArg;

	return S_OK;
}

HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

_bool CNavigation::is_MoveOnNavigation(_fvector vPosition, CTransform* pTransform)
{
	if (-1 == m_NaviDesc.iCurrentIndex)
		return false;

	_int iNeighborIndex = { -1 };

	//셀 안에 있다
	if (true == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPosition, &iNeighborIndex))
	{
		return true;
	}
	else //셀 안에 없다(나갔다)
	{
		if (-1 != iNeighborIndex)	//나간 방향에 이웃이 있다
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
					break;
			}

			m_NaviDesc.iCurrentIndex = iNeighborIndex;
			return true;

		}
		else //나간 방향에 이웃이 없다
		{
			// 슬라이딩 벡터를 타게 만들 것
			//_vector vSliding = m_Cells[m_NaviDesc.iCurrentIndex]->Sliding(vPosition, pTransform->Get_State(CTransform::STATE_POSITION));

			//pTransform->Set_State(CTransform::STATE_POSITION, pTransform->Get_State(CTransform::STATE_POSITION) + (vSliding));

			return false;
		}
	}

	return _bool();
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	if (-1 == m_NaviDesc.iCurrentIndex)
	{
		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
			{
				_float4 fColor = { 0.f,1.f,0.f,1.f };
				pCell->Render_Debug(&fColor, 0.f);
			}
		}
	}
	else
	{
		_float4 fColor = { 1.f,0.f,0.f,1.f };
		m_Cells[m_NaviDesc.iCurrentIndex]->Render_Debug(&fColor, 0.01f);
	}

	return S_OK;
}

#endif // _DEBUG

_int CNavigation::CheckIndex(_fvector vPosition)
{
	_int iNeighborIndex = { -1 };
	_int iIndex = { 0 };

	if (false == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPosition, &iNeighborIndex))
	{
		for (auto& pCell : m_Cells)
		{
			if (true == pCell->isIn(vPosition, &iNeighborIndex))
				break;

			iIndex++;
		}
	}
	else
		iIndex = m_NaviDesc.iCurrentIndex;
	

	if (m_Cells.size() <= iIndex)
		iIndex = 0;
	
	return iIndex;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFile)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();
}
