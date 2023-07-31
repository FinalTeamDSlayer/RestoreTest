#pragma once
#include "Component.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNaviDesc
	{
		_vector vStartPosition;
		_int	iCurrentIndex = { -1 };
	}NAVIDESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	void Get_CurrentPoints(_float3* pPoints) { m_Cells[m_NaviDesc.iCurrentIndex]->Get_Points(pPoints); }
	_int Get_CurrentIndex() const { return m_NaviDesc.iCurrentIndex; }
public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFile);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT SetUp_Neighbors();
	_bool	is_MoveOnNavigation(_fvector vPosition, class CTransform* pTransform);

#ifdef _DEBUG
public:
	virtual HRESULT Render() override;

#endif // _DEBUG

private:
	vector<CCell*>		m_Cells;

	NAVIDESC			m_NaviDesc;

private:
	_int CheckIndex(_fvector vPosition);

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END