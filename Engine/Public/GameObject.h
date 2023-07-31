#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagWorldInfo
	{
		XMFLOAT3	vScale;
		XMFLOAT3	vAxis;
		float		fDegree;
		XMFLOAT4	vPosition;
	}WORLDINFO;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
	
public:
	_bool	Get_Dead() const {	//昏力贸府侩
		return m_isDead;
	}

	void Set_Dead() {			//昏力贸府侩
		m_isDead = true;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double dTimeDelta);
	virtual void	LateTick(_double dTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth(); // Shadow
	virtual HRESULT Render_SSAO(); // Shadow

public:
	class CComponent*	Get_Component(const _tchar * pComponentTag);

public:
	HRESULT				Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, _Inout_ class CComponent** ppOut, void* pArg = nullptr);

public:
	class CComponent*	Find_Component(const _tchar* pComponentTag);

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	WORLDINFO				m_WorldInfo;
	_bool					m_isDead = { false };

protected:
	unordered_map<const _tchar*, class CComponent*>		m_Components;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END