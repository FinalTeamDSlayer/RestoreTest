#include "..\Public\PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

void CPipeLine::Tick()
{
	for (_uint i = 0; i < D3DTS_END; i++)
		XMStoreFloat4x4(&m_TransformState_Inverse[i], 
			XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformState[i])));
}

void CPipeLine::Free()
{
}
