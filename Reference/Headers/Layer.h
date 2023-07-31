#pragma once
#include "Base.h"

BEGIN(Engine)

class CLayer final: public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
	
public:
	class CComponent*	Get_Component(const _tchar* pComponentTag);
	class CGameObject*	Get_GameObject(_uint iIndex);	//레이어의 iIndex번째 객체 리턴
	list<class CGameObject*>*	Get_GameObjects() {		//레이어의 객체 list(모든 객체들) 리턴
		return &m_GameObjects;
	}

public:
	void Set_Keep(_bool isKeep) {
		m_isKeep = isKeep;
	}

public:
	HRESULT		Initialize(_bool isKeep);
	HRESULT		Add_GameObject(class CGameObject* pGameObject);
	void		Tick(_double dTimeDelta);
	void		LateTick(_double dTimeDelta);

public:
	void		Clear_Layer();

private:
	list<class CGameObject*>	m_GameObjects;
	_bool						m_isKeep = { false };
public:
	static CLayer* Create(_bool isKeep);
	virtual void Free() override;
};

END