#pragma once
#include "Base.h"

BEGIN(Engine)

class CLight_Manager final :public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC*	Get_Light(_uint iIndex) const;
	void				Set_Light(_uint iIndex, _uint iOption, _float4 vLightValue);
public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	HRESULT	Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT	Clear_Light();
private:
	list<class CLight*>		m_Lights;

public:
	virtual void Free() override;
};

END