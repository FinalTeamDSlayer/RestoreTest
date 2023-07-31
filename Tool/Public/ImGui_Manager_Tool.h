#pragma once

#include "Tool_Defines.h"
#include "Base.h"

#include "Camera.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Tool)

class CImGui_Manager_Tool final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager_Tool)
private:
	CImGui_Manager_Tool();
	virtual ~CImGui_Manager_Tool() = default;

public:
	void ImGui_Set();
	void ImGUI_ShowDemo();


#pragma region Camera Set
public:
	CCamera::CAMERADESC Get_CameraDesc() { return m_CameraDesc; }
	void Set_CameraDesc(void* pArg) { memcpy(&m_CameraDesc, pArg, sizeof m_CameraDesc); }
private:
	CCamera::CAMERADESC		m_CameraDesc;
#pragma endregion

#pragma region StageSet
public:
	_bool	Get_IsLoad(_uint iStageNum) const {
		return m_isLoad[iStageNum - 2];
	}
	void	Set_IsLoad(_uint iStageNum) {
		m_isLoad[iStageNum - 2] = true;
	}

private:
	void	GoToStage(_int iSelected);

private:
	_int	m_iSelectedStage = { 0 };
	_bool	m_isLoad[6] = {};

#pragma endregion

#pragma region Collider Set

public:
	void Set_DefaultLight_ImGui(LIGHTDESC LightDesc);

private:
	_float4 m_vLightDesc[4];

#pragma endregion
	
#pragma region toCopy

#pragma endregion

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_bool	m_isGUINoMove = { true };
	_bool	m_isGUINoResize = { true };

public:
	HRESULT Initialize_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick_ImGui();
	HRESULT Render_ImGui();
	void Release_ImGui();

public:
	virtual void Free() override;
};

END