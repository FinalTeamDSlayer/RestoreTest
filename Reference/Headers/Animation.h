#pragma once
#include "Base.h"

#include "Transform.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	typedef struct tagAnimation
	{
		char						m_szName[MAX_PATH] = { "" };
		_double						m_dDuration = { 0.0 };
		_double						m_dTickPerSecond = { 0.0 };
		_double						m_dTimeAcc = { 0.0 };
		_bool						m_isFinish = { false };

		_uint						m_iNumChannels = { 0 };

		vector<class CChannel*>		m_Channels;

		vector<_uint>				m_iCurrentKeyFrames;

	}ANIMATIONDESC;

	typedef struct tagEvent
	{
		_double m_dTime;
		_bool	m_isFirst = { true };
	}EVENTDESC;

	//애니메이션 제어를 위한 구조체
	typedef struct tagAnimation_Control
	{
		//애니메이션 스피드
		_float			m_fAnimationSpeed = { 1.0f };

		//다음 애니메이션 연결
		_int			m_iConnect_Anim = { 0 };

		// 콤보 애니메이션, 콤보 연결
		_bool			m_isCombo = { false };
		_int			m_iConnect_ComboAnim = { 0 };

		// 루트 애니메이션 여부
		_bool			m_isRootAnimation = { false };

		// 이벤트 관련
		vector<EVENTDESC>	m_vecTime_Event;

		_bool	m_isEventCall = { false };
		_int	m_iEventIndex = { 0 };

	}CONTROLDESC;

private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(ANIMATIONDATA* pAnimationData, class CModel* pModel);
	_int	Invalidate_TransformationMatrices(class CModel* pModel, _double dTimeDelta, _bool Play , _bool Combo );
	_bool	Invalidate_Linear_TransformationMatrices(class CModel* pModel, _double dTimeDelta, _bool Play, vector<KEYFRAME> vecLastKey );
	//_float4  Get_Pos_RootAnimation(CTransform* pTransformCom);

public:
	//Get
	ANIMATIONDESC Get_AnimationDesc() { return m_AnimationDesc; }
	CONTROLDESC	  Get_ControlDesc() { return m_ControlDesc; }

	class CChannel* Get_Channel(const char* pChannelName);

	_float3		Get_RootPosition() { return m_RootPosition; }
	
	vector<KEYFRAME> Get_LastKeys();


	//Set
	void	Set_AnimationDesc(ANIMATIONDESC animdesc) { m_AnimationDesc = animdesc; }
	void	Reset_TimeAcc() { m_AnimationDesc.m_dTimeAcc = 0.0; }

	void	Set_ControlDesc(CONTROLDESC control) { m_ControlDesc = control; }
	void	Set_EarlyEnd(_bool bEnd) { m_isEarlyEnd = bEnd; }


private:
	ANIMATIONDESC	m_AnimationDesc;
	CONTROLDESC		m_ControlDesc;
	

private:
	_float3		m_RootPosition ;

	_float4		m_Save_RootPos = { 0.0f, 0.0f ,0.0f, 1.0f };
	
	_bool		m_isFirst_EventCall = { true };
	_bool		m_isFirst_ComboDuration = { true };

	_bool		m_isEarlyEnd = { false };

public:
	static CAnimation* Create(ANIMATIONDATA* pAnimationData, class CModel* pModel);
	CAnimation* Clone();
	virtual void Free() override;
};

END