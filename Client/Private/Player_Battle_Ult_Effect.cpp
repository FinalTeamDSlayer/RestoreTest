#include "pch.h"
#include "..\Public\Player_Battle_Ult_Effect.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"
#include "Player.h"


CPlayer_Battle_Ult_Effect::CPlayer_Battle_Ult_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_Battle_Ult_Effect::CPlayer_Battle_Ult_Effect(const CPlayer_Battle_Ult_Effect & rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_Battle_Ult_Effect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Battle_Ult_Effect::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_UI_Desc = *(UIDESC*)pArg;

	m_Is_Reverse = m_UI_Desc.m_Is_Reverse;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	
	// Stack_Frame
	if (m_UI_Desc.m_Type == 0) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 180;
		m_Origin_Y = 180;
		m_Size_Param = 0.5f;
		m_Origin_Size_Param = m_Size_Param;
		m_Max_Size_Param = 0.65f;
		m_Size_Speed = 0.7f;
		m_Size_Dir = 1.f;
		m_UI_Layer = -1;
	}

	// Stack_Small
	if (m_UI_Desc.m_Type == 1) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 116;
		m_Origin_Y = 116;
		m_Size_Param = 0.5;
		m_Origin_Size_Param = m_Size_Param;
		m_UI_Layer = 6.5;
		m_Alpha = 0.5f;
		m_Max_Alpha = 0.5f;
		m_Alpha_Speed = 0.7f;
	}

	// Stack_Glow
	if (m_UI_Desc.m_Type == 2) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 132;
		m_Origin_Y = 132;
		m_Size_Param = 0.641718f;
		m_Size_Speed = 0.5f;
		m_UI_Layer = 1;
	}

	// Fire_Sprite_1
	if (m_UI_Desc.m_Type == 3) {
		m_fX = 174;
		m_fY = 637;
		m_Origin_X = 452;
		m_Origin_Y = 164;
		m_Size_Param = 0.618374f;
		m_UI_Layer = -3;
	}

	// Fire_Sprite_2
	if (m_UI_Desc.m_Type == 4) {
		m_fX = 174;
		m_fY = 637;
		m_Origin_X = 452;
		m_Origin_Y = 164;
		m_Size_Param = 0.618374f;
		m_UI_Layer = -3;
	}

	// Stack_Up
	if (m_UI_Desc.m_Type == 5) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 116;
		m_Origin_Y = 116;
		m_Size_Param = 0.618374f;
		m_Origin_Size_Param = m_Size_Param;
		m_UI_Layer = 5;
		m_Size_Speed = 0.7f;
		m_Size_Dir = 1.f;
		m_Max_Alpha = 0.9f;
		m_Alpha = 0.f;
	}

	// Stack_Down
	if (m_UI_Desc.m_Type == 6) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 116;
		m_Origin_Y = 116;
		m_Size_Param = 0.618374f;
		m_Origin_Size_Param = m_Size_Param;
		m_UI_Layer = 5;
		m_Size_Speed = 0.7f;
		m_Size_Dir = 1.f;
		m_Max_Alpha = 0.9f;
		m_Alpha = 0.f;
	}
	

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CPlayer_Battle_Ult_Effect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	if (m_UI_Desc.m_Type == 0) {
		Size_Change(TimeDelta);
	}
	
	if (m_UI_Desc.m_Type == 1)
		Alpha_Change(TimeDelta);
	

	m_Sprite += (_float)TimeDelta * 10.f;

	if (m_Sprite > 5) {
		m_Sprite = 0;
	}

	

	Set_UI();

}

void CPlayer_Battle_Ult_Effect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	Get_Player_Info(TimeDelta);


	if (m_Pre_Ult_Stack != m_Ult_Stack) {

		if (m_Pre_Ult_Stack < m_Ult_Stack)
			m_Is_Stack_Up = true;
		
		if(m_Pre_Ult_Stack > m_Ult_Stack)
			m_Is_Stack_Down = true;
		

		m_Pre_Ult_Stack = m_Ult_Stack;
		
	}


	if (m_Is_Stack_Up == true && m_UI_Desc.m_Type == 5)
		Stack_Up(TimeDelta);

	if (m_Is_Stack_Down == true && m_UI_Desc.m_Type == 6)
		Stack_Down(TimeDelta);


	if (m_UI_Desc.m_Type == 0) {
		if (m_Ult_Stack >= 1 && m_Ult_State < 2) {
			m_Size_Up = true;
		}
		else {
			m_Size_Up = false;
		}

		// 임시 처리
		if ((m_Ult_Stack == 0 && m_Ult_State != 2) || m_Ult_State == 3) {
			m_Is_Render = false;
		}
		else {
			m_Is_Render = true;
		}
	}

	if (m_UI_Desc.m_Type == 1) {
		if (m_Ult_Stack >= 1) {
			m_Is_Render = true;
		}
		else {
			m_Is_Render = false;
		}
	}

	if (m_UI_Desc.m_Type == 2) {
		if (m_Ult_State < 2) {
			m_Is_Render = true;
		}
		else {
			m_Is_Render = false;
		}

		if (m_Ult_Stack == 0) {
			m_Is_Render = false;
		}
	}

	if (m_UI_Desc.m_Type == 3) {
		if (m_Ult_State == 2) {
			m_Is_Render = true;
		}
		else {
			m_Is_Render = false;
		}
	}

	if (m_UI_Desc.m_Type == 4) {
		if (m_Ult_State == 3) {
			m_Is_Render = true;
		}
		else {
			m_Is_Render = false;
		}
	}

	Get_Mouse_Pos();

	m_Pt_In = Pt_InUI();

	if (m_UI_Desc.m_Type == 3) {
		Tool_Funtion(TimeDelta);
	}

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CPlayer_Battle_Ult_Effect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (m_Is_Reverse == false)
		m_pShaderCom->Begin(1);
	else {
		m_pShaderCom->Begin(2);
	}

	
	if (m_Is_CutScene == false && m_Is_Render == true) {

		m_pVIBufferCom->Render();

	}
	
	
	
	return S_OK;
}

HRESULT CPlayer_Battle_Ult_Effect::Add_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))		
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;	

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	if (m_UI_Desc.m_Type == 0 || m_UI_Desc.m_Type == 1 || m_UI_Desc.m_Type == 2 || m_UI_Desc.m_Type == 5 || m_UI_Desc.m_Type == 6) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Stock_Eff"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	if (m_UI_Desc.m_Type == 3) {

		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Awk"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

	}
	if (m_UI_Desc.m_Type == 4) {

		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Swk"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}




	
	

	return S_OK;
}

HRESULT CPlayer_Battle_Ult_Effect::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;	

	if (m_Alpha < 0) {
		_float Alpha = 0.f;

		if (FAILED(m_pShaderCom->SetUp_RawValue("g_Alpha", &Alpha, sizeof(_float))))
			return E_FAIL;
	}
	else {
		if (FAILED(m_pShaderCom->SetUp_RawValue("g_Alpha", &m_Alpha, sizeof(_float))))
			return E_FAIL;
	}
	
	
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (m_UI_Desc.m_Type == 0 || m_UI_Desc.m_Type == 1 || m_UI_Desc.m_Type == 2 || m_UI_Desc.m_Type == 5 || m_UI_Desc.m_Type == 6) {

		// 0번은 완전개방때 파란색으로 바꿔야함
		if (m_UI_Desc.m_Type == 0 || m_UI_Desc.m_Type == 1 || m_UI_Desc.m_Type == 2) {
			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_UI_Desc.m_Type)))
				return E_FAIL;
		}

		if (m_UI_Desc.m_Type == 5 || m_UI_Desc.m_Type == 6) {
			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 3)))
				return E_FAIL;
		}


	}
	if (m_UI_Desc.m_Type == 3) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", (_uint)m_Sprite)))
		   return E_FAIL;
	}
	if (m_UI_Desc.m_Type == 4) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", (_uint)m_Sprite)))
			return E_FAIL;
	}
	

	return S_OK;
}

void CPlayer_Battle_Ult_Effect::Set_UI()
{
	m_fSizeX = m_Origin_X * (_double)m_Size_Param;
	m_fSizeY = m_Origin_Y * (_double)m_Size_Param;

	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		Viewport;
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	_matrix		TransformMatrix = XMMatrixIdentity();

	TransformMatrix.r[0] = TransformMatrix.r[0] * (_float)m_fSizeX;
	TransformMatrix.r[1] = TransformMatrix.r[1] * (_float)m_fSizeY;

	TransformMatrix.r[3] = XMVectorSetX(TransformMatrix.r[3], (_float)m_fX - Viewport.Width * 0.5f);
	TransformMatrix.r[3] = XMVectorSetY(TransformMatrix.r[3], -(_float)m_fY + Viewport.Height * 0.5f);

	XMStoreFloat4x4(&m_WorldMatrix, TransformMatrix);

}

void CPlayer_Battle_Ult_Effect::Get_Mouse_Pos()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		Viewport;
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	m_MouseX = ptCursor.x;
	m_MouseY = ptCursor.y;

	m_ScreenWidth = g_iWinSizeX;
	m_ScreenHeight = g_iWinSizeY;

	if (m_MouseX < 0) { m_MouseX = 0; }
	if (m_MouseY < 0) { m_MouseY = 0; }

	if (m_MouseX > m_ScreenWidth) { m_MouseX = m_ScreenWidth; }
	if (m_MouseY > m_ScreenHeight) { m_MouseY = m_ScreenHeight; }


	Safe_Release(pGameInstance);
}

void CPlayer_Battle_Ult_Effect::Change_Info(_double TimeDelta)
{

	_long		MouseMove = 0;

	if (m_Is_Fix == false) {
		CGameInstance*		pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (fabs(MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_X)) > 1.f)
		{
			m_fX += TimeDelta * MouseMove * 40.f;
		}

		if (fabs(MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMS_Y)) > 1.f)
		{
			m_fY += TimeDelta * MouseMove * 40.f;
		}

		if (GetKeyState(VK_UP) < 0) {
			m_Size_Param += (_float)TimeDelta * 0.5f;

		}
		if (GetKeyState(VK_DOWN) < 0) {
			m_Size_Param -= (_float)TimeDelta * 0.5f;

			if (m_Size_Param <= 0) {
				m_Size_Param = 0;
			}
		}
		if (GetKeyState(VK_LEFT) < 0) {

		}
		if (GetKeyState(VK_RIGHT) < 0) {

		}

		Safe_Release(pGameInstance);
	}
}

void CPlayer_Battle_Ult_Effect::Tool_Funtion(_double TimeDelta)
{
	//// 툴 전용 
	//if (CImGUI_Manager::Get_Instance()->Get_Mouse_Layer() == m_UI_Layer || CImGUI_Manager::Get_Instance()->Get_Mouse_Layer() == 99) {

	//	if (m_Pt_In) {

	//		_long		MouseMove = 0;

	//		if (GetKeyState(VK_LBUTTON) < 0) {

	//			if (GetKeyState('F') < 0) {
	//				m_Is_Fix = true;
	//			}
	//			if (GetKeyState('G') < 0) {
	//				m_Is_Fix = false;
	//			}

	//			if (m_Is_Fix == false)
	//				m_Alpha = 0.5f;

	//			Change_Info(TimeDelta);

	//			CImGUI_Manager::Get_Instance()->Update_UI_Info(m_fX, m_fY, m_Origin_X, m_Origin_Y, m_Size_Param, m_UI_Layer);

	//		}
	//		else {
	//			m_Alpha = 1.f;
	//		}

	//	}
	//	else {
	//		m_Alpha = 1.f;
	//	}
	//}
	//else {
	//	m_Alpha = 1.f;
	//}
}

void CPlayer_Battle_Ult_Effect::Get_Player_Info(_double TimeDelta)
{
	
	if (GetKeyState('U') < 0) {

		m_Player_Ult += TimeDelta * 0.5;

		if (m_Player_Ult <= 0) {
			m_Player_Ult = 0;
		}

		if (m_Player_Ult >= 1) {

			m_Player_Ult = 0;

			m_Ult_Stack++;

			if (m_Ult_Stack > 3) {
				m_Ult_Stack = 3;
			}
		}
	}

	if (GetKeyState('I') < 0) {
		m_Player_Ult = 0;
		m_Ult_Stack = 0;
	}

	if (GetKeyState('0') < 0) {
		m_Ult_State = 0;
	}
	if (GetKeyState('1') < 0) {
		m_Ult_State = 1;
	}
	if (GetKeyState('2') < 0) {
		m_Ult_Stack = 0;
		m_Ult_State = 2;
	}
	if (GetKeyState('3') < 0) {
		m_Ult_Stack = 1;
		m_Ult_State = 3;
	}

}

void CPlayer_Battle_Ult_Effect::Size_Change(_double TimeDelta)
{
	if (m_Size_Up == true) {

		m_Size_Param += (_float)TimeDelta * m_Size_Speed * m_Size_Dir;
		
		if (m_Size_Param > m_Max_Size_Param) {
			m_Size_Param = m_Max_Size_Param;
			m_Size_Dir = -1.f;
			m_Size_Speed = 0.7f;
		}

		if (m_Size_Param < 0.5f) {
			m_Size_Param = 0.5f;
		}

		m_Size_Time += (_float)TimeDelta;

		if (m_Size_Time > 1.2f) {
			m_Size_Time = 0.0f;
			m_Size_Dir = 1.f;
			m_Size_Speed = 0.7f;
		}
		
	}
	else {
		m_Size_Param = m_Origin_Size_Param;
		m_Size_Time = 0.0f;
		m_Size_Dir = 1.f;
	}

}

void CPlayer_Battle_Ult_Effect::Alpha_Change(_double TimeDelta)
{

	if (m_Alpha_Up == true) {
		m_Alpha = m_Max_Alpha;
		m_Alpha_Up = false;
	}

	m_Alpha -= (_float)TimeDelta * m_Alpha_Dir * m_Alpha_Speed;

	if (m_Alpha <= 0.0f) {
		m_Alpha = 0.f;
	}

	m_Alpha_Time += (_float)TimeDelta;

	if (m_Alpha_Time > 1.2f) {
		m_Alpha_Time = 0.f;
		m_Alpha_Up = true;
	}

	if(m_Is_Render == false)
		m_Alpha_Time = 0.f;

}

void CPlayer_Battle_Ult_Effect::Stack_Up(_double TimeDelta)
{
	m_Stack_Up_Time += (_float)TimeDelta * m_Stack_Dir;

	if (m_Stack_Up_Time > 0.2f && m_Is_Stack_Up_Change == false) {

		if (m_Alpha <= 0.f) {
			m_Alpha = m_Max_Alpha;
		}

		m_Is_Stack_Up_Change = true;
		m_Stack_Dir = 0.f;
	}

	if (m_Is_Stack_Up_Change) {

		m_Alpha -= (_float)TimeDelta * m_Alpha_Dir * 1.5f;

		m_Size_Param += (_float)TimeDelta * m_Size_Dir * 1.3f;

		if (m_Alpha <= 0.f) {
			m_Alpha = 0.f;
			m_Size_Param = m_Origin_Size_Param;
			m_Is_Stack_Up = false;
			m_Is_Stack_Up_Change = false;
			m_Stack_Dir = 1.f;
			m_Stack_Up_Time = 0.f;
		}

	}
}

void CPlayer_Battle_Ult_Effect::Stack_Down(_double TimeDelta)
{

	m_Stack_Down_Time += (_float)TimeDelta * m_Stack_Dir;

	if (m_Stack_Down_Time > 0.0f && m_Is_Stack_Down_Change == false) {

		if (m_Alpha <= 0.f) {
			m_Alpha = m_Max_Alpha;
		}

		m_Is_Stack_Down_Change = true;
		m_Stack_Dir = 0.f;
	}

	if (m_Is_Stack_Down_Change) {

		m_Alpha -= (_float)TimeDelta * m_Alpha_Dir * 1.3f;

		if (m_Alpha <= 0.f) {
			m_Alpha = 0.f;
			m_Is_Stack_Down = false;
			m_Is_Stack_Down_Change = false;
			m_Stack_Dir = 1.f;
			m_Stack_Down_Time = 0.f;
		}

	}




}

CPlayer_Battle_Ult_Effect * CPlayer_Battle_Ult_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer_Battle_Ult_Effect*		pInstance = new CPlayer_Battle_Ult_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Battle_Ult_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CPlayer_Battle_Ult_Effect::Clone(void * pArg)
{
	CPlayer_Battle_Ult_Effect*		pInstance = new CPlayer_Battle_Ult_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Battle_Ult_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Battle_Ult_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

}
