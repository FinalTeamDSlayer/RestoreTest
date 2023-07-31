#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
END

BEGIN(Client)

class CColliderManager final : public CBase
{
	DECLARE_SINGLETON(CColliderManager)

public:
	enum COLLIDERGROUP {
		COLLIDER_PLAYER, COLLIDER_PLAYER_BULLET,
		COLLIDER_MONSTER, COLLIDER_MONSTER_BULLET, COLLIDER_END
	};
private:
	CColliderManager();
	virtual ~CColliderManager() = default;

public:
	HRESULT Inititalize();
	HRESULT Check_Collider(_uint iLevelIndex, _double dTimeDelta);

private:

private:
	HRESULT Check_PlayerToMonster(_uint iLevelIndex, _double dTimeDelta);
	HRESULT Check_MonsterToMonster(_uint iLevelIndex, _double dTimeDelta);

public:
	virtual void Free() override;
};

END