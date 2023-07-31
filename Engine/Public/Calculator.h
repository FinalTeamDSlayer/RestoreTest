#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator final : public CComponent
{
private:
	CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCalculator(const CCalculator& rhs);
	virtual ~CCalculator() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	_float4 Picking_Plane(HWND hWnd, _uint iWindSizeX, _uint iWinSizeY, class CVIBuffer_Terrain * pVIBuffer);
	_float4 Picking_Model(HWND hWnd, _uint iWindSizeX, _uint iWinSizeY, class CModel * pModel);

public:
	static CCalculator* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END