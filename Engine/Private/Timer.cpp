#include "Timer.h"

CTimer::CTimer()
	: m_dTimeDelta {0}
{
	ZeroMemory(&m_tFrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tFixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_tLastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_tFrameTime);
	QueryPerformanceCounter(&m_tLastTime);
	QueryPerformanceCounter(&m_tFixTime);
	QueryPerformanceFrequency(&m_CpuTick);
	
	return S_OK;
}

void Engine::CTimer::Update_Timer()
{
	QueryPerformanceCounter(&m_tFrameTime);

	if (m_tFrameTime.QuadPart - m_tFixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_tFixTime = m_tFrameTime;
	}

	m_dTimeDelta = ((m_tFrameTime.QuadPart) - (m_tLastTime.QuadPart)) / _double(m_CpuTick.QuadPart);

	m_tLastTime = m_tFrameTime;
}

CTimer * CTimer::Create()
{
	CTimer *	pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
		Safe_Release(pInstance);

	return pInstance;
}

void CTimer::Free()
{
}
