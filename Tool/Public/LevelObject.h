#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
END

BEGIN(Tool)

class CLevelObject abstract : public CGameObject
{
public:
	typedef struct tagLevelDesc

protected:
	CLevelObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLevelObject(const CLevelObject& rhs);
	virtual ~CLevelObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

protected:
	virtual HRESULT Add_Components();

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END