#include "pch.h"
#include "..\Public\ImGui_Manager_Tool.h"

#include "GameInstance.h"
#include "Level_Loading_Tool.h"

IMPLEMENT_SINGLETON(CImGui_Manager_Tool)

CImGui_Manager_Tool::CImGui_Manager_Tool()
{

}

void CImGui_Manager_Tool::ImGui_Set()
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    ImGui::SetNextWindowPos(ImVec2(g_iGUI_RightX, g_iGUI_RightY));
    ImGui::SetWindowSize(ImVec2(300, -1));
    ImGuiWindowFlags iWindow_Flags = { 0 };
    if (m_isGUINoMove)               iWindow_Flags |= ImGuiWindowFlags_NoMove;
    if (m_isGUINoResize)             iWindow_Flags |= ImGuiWindowFlags_NoResize;
    ImGui::Begin("Tool", nullptr, iWindow_Flags);
    
#pragma region Camera Set
    if (ImGui::TreeNode("Camera Set"))
    {
        _float fSpeed = (_float)m_CameraDesc.TransformDesc.dSpeedPerSec;
        ImGui::SliderFloat("Speed", &fSpeed, 0.f, 1000.0f, "%.1f");
        m_CameraDesc.TransformDesc.dSpeedPerSec = (_double)fSpeed;

        ImGui::SliderFloat("Far", &m_CameraDesc.fFarZ, 0.1f, 10000.0f, "%.1f");

        ImGui::SliderFloat("Fov", &m_CameraDesc.fFovY, 0.1f, 2.f, "%.1f");

        ImGui::TreePop();

    }
#pragma endregion

#pragma region StageSet
    if (ImGui::TreeNode("Stage Set"))
    {
        const char* Stage[] = { "00.Logo", "01.Tuto", "02.Village", "03.House", "04.Train", "05.FinalBoss" };

        ImGui::Text("Stage List : ");
        if(ImGui::BeginListBox("##listbox 1", ImVec2(-20.f, 6 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (_int i = 0; i < IM_ARRAYSIZE(Stage); i++)
            {
                const _bool isSelected = (m_iSelectedStage == i);
                if (ImGui::Selectable(Stage[i], isSelected))
                    m_iSelectedStage = i;

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            
            ImGui::EndListBox();
        }

        if (ImGui::Button("GoToStage"))
        {
            if(pGameInstance->Get_IsStage())
                GoToStage(m_iSelectedStage + 2);
        }
            

        ImGui::TreePop();
    }
#pragma endregion

#pragma region Collider Set
    if (ImGui::TreeNode("Collider Set"))
    {
        //ImGui::SeparatorText("Direction");


        ImGui::TreePop();
    }
#pragma endregion

    ImGui::SeparatorText("WindowSet");
    ImGui::Checkbox("No Move", &m_isGUINoMove);
    ImGui::Checkbox("No Resize", &m_isGUINoResize);

    ImGui::End();

    Safe_Release(pGameInstance);
}

void CImGui_Manager_Tool::ImGUI_ShowDemo()
{
	_bool show_deme_window = true;

	if (show_deme_window)
		ImGui::ShowDemoWindow(&show_deme_window);
}

#pragma region StageSet
void CImGui_Manager_Tool::GoToStage(_int iSelected)
{
    HRESULT hr = 0;

    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    if (false == m_isLoad[iSelected - 2])
    {
        //Load 해야함
        hr = pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading_Tool::Create(m_pDevice, m_pContext, (LEVELID)iSelected), false, false);
    }
    else
    {
        //스테를 갈아끼워야 함
        hr = pGameInstance->Swap_Level((LEVELID)iSelected);
    }
    Safe_Release(pGameInstance);

    if (FAILED(hr))
        return;
}
#pragma endregion

#pragma region Collider Set
void CImGui_Manager_Tool::Set_DefaultLight_ImGui(LIGHTDESC LightDesc)
{
    m_vLightDesc[0] = LightDesc.vLightDir;
    m_vLightDesc[1] = LightDesc.vLightDiffuse;
    m_vLightDesc[2] = LightDesc.vLightAmbient;
    m_vLightDesc[3] = LightDesc.vLightSpecular;
}
#pragma endregion

HRESULT CImGui_Manager_Tool::Initialize_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.Colors[ImGuiCol_WindowBg].w = 1.f;
    }

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);

    return S_OK;
}
void CImGui_Manager_Tool::Tick_ImGui() 
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

}

HRESULT CImGui_Manager_Tool::Render_ImGui()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    return S_OK;
}

void CImGui_Manager_Tool::Release_ImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


void CImGui_Manager_Tool::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
