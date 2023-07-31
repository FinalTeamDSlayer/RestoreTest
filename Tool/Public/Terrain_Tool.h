#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END

BEGIN(Tool)

class CTerrain_Tool final : public CGameObject
{
public:
	typedef struct tagTreeDesc{

		_float4 vPosition;

	}TREEDESC;
private:
	CTerrain_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain_Tool(const CTerrain_Tool& rhs);
	virtual ~CTerrain_Tool() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_double dTimeDelta) override;
	virtual void	LateTick(_double dTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CModel*			m_pModelCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };
	CRenderer*		m_pRendererCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };

private:
	TREEDESC		m_TreeDesc;

private:
	HRESULT Add_Components();
	HRESULT	SetUp_ShaderResources();

public:
	static CTerrain_Tool* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END