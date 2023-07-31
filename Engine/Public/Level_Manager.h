#pragma once
#include "Base.h"
#include "Level.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
    DECLARE_SINGLETON(CLevel_Manager)
private:
    CLevel_Manager();
    virtual ~CLevel_Manager() = default;

public:
    CLevel* Get_CulLevel() const {
        return m_pCurrentLevel; 
    }

    _uint   Get_CurLevelIdx() const {
        return m_iLevelIndex;
    }

    CLevel* Get_LoadedStage(_uint iLevelIndex) const {
        return m_pLoadedLevels[iLevelIndex];
    }

    _bool   Get_IsStage() const {
        return m_pCurrentLevel->Get_IsStage();
    }

    _bool   Get_IsLoadForAll() const {
        return m_isLoadForAll;
    }

    void    Set_IsLoadForAll() {
        m_isLoadForAll = true;
    }

public:
    HRESULT Reserve_Containers(_uint iNumLevels);
    HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNextLevel, _bool isStage, _bool isRelease);   //레벨 이동 시 사용할 것
    HRESULT Swap_Level(_uint iLevelIndex);
    void    Tick_Level(_double dTimeDelta);

private:
    vector<CLevel*> m_pLoadedLevels;
    _uint           m_iNumLevels;
    _bool           m_isLoadForAll = { false };

    CLevel*     m_pCurrentLevel = { nullptr };  //현재 레벨
    _uint       m_iLevelIndex = { 0 };

public:
    virtual void Free() override;
};

END