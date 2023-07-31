#pragma once

#include "Tool_Defines.h"
#include "Base.h"

#include "Camera.h"
#include "Animation.h"


#include "GameInstance.h"

BEGIN(Tool)

class CImGui_Animation_Tool final : public CBase
{
	DECLARE_SINGLETON(CImGui_Animation_Tool)
private:
	CImGui_Animation_Tool();
	virtual ~CImGui_Animation_Tool() = default;


public: // 함수
	void Animation_ImGui_Main();

	


public: // GetSet
	_bool	Get_Play() { return m_isPlay; }

	void Set_Animation(CAnimation* pAnim) { m_pAnimation = pAnim; }

	void Set_vecName(vector<char*> vecName);
	

	_int Get_AnimIndex() { return m_iAnimIndex; }
	void Set_AnimIndex(_int index) { m_iAnimIndex = index; }

	_bool Get_Signal_Change_Anim() { return m_Signal_to_Change_Anim; }
	void  Set_Signal_Change_Anim(_bool bchange) { m_Signal_to_Change_Anim = bchange; }


	_bool	Get_Save() { return m_isSave; }
	void	Set_Save(_bool bSave) { m_isSave = bSave; }

	void	Set_iTest(_int i) { m_iTest = i; }

private: 
	// 재생 play
	_bool	m_isPlay = { false };

	// 애니메이션 변화 신호주기
	_bool	m_Signal_to_Change_Anim = { false };

	//root
	_bool	m_isRootAnimation = { false };

	// 변수
	_bool	m_isFirst_ListBox = { true };

	CAnimation*		m_pAnimation = { nullptr };

	vector<char*> m_vecName_ForListBox;
	_int	m_iAnimIndex = { 0 };
	_int	m_iSave_AnimIndex = { 0 };

	//Slider 변수
	_double		m_dFixed_Time = { 0.0 };
	_double		m_dCur_Time = { 0.0 };
	_double		m_dEnd_Time = { 0.0 };

	// loop checkbox
	_bool	m_isCombo = { false };
	_int	m_iConnect_Combo_Index = { 0 };

	// Connect index
	_bool	m_isFirst_ConnectIndex = { true };
	_int	m_iConnectIndex = { 0 };
	
	//eventcall
	_int	m_iTest = { 0 };


	//Save
	_bool	m_isSave = { false };

	_bool	m_isGUINoMove[2] = { true, true };
	_bool	m_isGUINoResize[2] = { true, true };

public:
	virtual void Free() override;
};

END