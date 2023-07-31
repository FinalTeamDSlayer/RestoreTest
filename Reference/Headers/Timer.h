#pragma once
#include "Base.h"

BEGIN(Engine)

class CTimer : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	_double		Get_TimeDelta() { return m_dTimeDelta; }

public:
	HRESULT		Ready_Timer();
	void		Update_Timer();
	
private:
	LARGE_INTEGER			m_tFrameTime;
	LARGE_INTEGER			m_tFixTime;
	LARGE_INTEGER			m_tLastTime;
	LARGE_INTEGER			m_CpuTick;

	_double					m_dTimeDelta;

public:
	static CTimer*		Create();
	virtual void		Free()	override;
};

END
