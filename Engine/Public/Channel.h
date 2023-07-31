#pragma once
#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(CHANNELDATA* pChannelData, const char* pName, _uint iBoneIndex);
	void	Invalidate(class CModel* pModel, _uint& pCurrentKeyFrame, _double TrackPosition);
	void	Invalidate_Linear(class CModel* pModel, KEYFRAME LastKeyFrame_Prev, _double TrackPosition  );
	

public:
	const char* Get_Name() const { return m_szName; }

	void	Set_Root(_bool isRoot) { m_isRoot = isRoot; }
	_float3 Get_RootPosition() { return m_RootPosition; }
	

	KEYFRAME Get_LastKeyFrame();

private:
	char			m_szName[MAX_PATH] = { "" };

	_uint			m_iBoneIndex = { 0 };

private://root animation 용
	_bool		m_isRoot = { false };
	_float3		m_RootPosition = { 0.f, 0.f, 0.f };

	

private:
	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;
	

	//재생과 역재생 구별
	_double  m_dSave_TrackPosition = { 0.0 };
	_bool	 m_isReverse = { false };

public:
	static CChannel* Create(CHANNELDATA* pChannelData, const char* pName, _uint iBoneIndex);
	virtual void Free() override;
};

END