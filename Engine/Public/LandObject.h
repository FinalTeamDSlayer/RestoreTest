#pragma once
#include "Engine_Defines.h"
#include "GameObject.h"
#include "Navigation.h"

BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	//virtual HRESULT Initialize_Prototype() override;
	//virtual HRESULT Initialize(void* pArg) override;
	//virtual void	Tick(_double dTimeDelta) override;
	//virtual void	LateTick(_double dTimeDelta) override;
	//virtual HRESULT Render() override;

protected:
	class CNavigation* m_pNavigationCom = { nullptr };

protected:
	_vector Compute_Height(_fvector vPos);

public:
	//virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END