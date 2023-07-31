#include "pch.h"
#include "..\Public\Player_Battle_Ult_Frame.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"
#include "Player.h"


CPlayer_Battle_Ult_Frame::CPlayer_Battle_Ult_Frame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CPlayer_Battle_Ult_Frame::CPlayer_Battle_Ult_Frame(const CPlayer_Battle_Ult_Frame & rhs)
	: CUI(rhs)
{
}

HRESULT CPlayer_Battle_Ult_Frame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Battle_Ult_Frame::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_UI_Desc = *(UIDESC*)pArg;

	m_Is_Reverse = m_UI_Desc.m_Is_Reverse;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	// Frame
	if (m_UI_Desc.m_Type == 0) {
		m_fX = 133;
		m_fY = 658.0 - 5;
		m_Origin_X = 368;
		m_Origin_Y = 68;
		m_Size_Param = 0.633341f;
		m_UI_Layer = 1;
	}

	// Stack_Frame
	if (m_UI_Desc.m_Type == 1) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 140;
		m_Origin_Y = 140;
		m_Size_Param = 0.541668f;
		m_UI_Layer = 2;
	}

	// Bar
	if (m_UI_Desc.m_Type == 2) {
		m_fX = 168.0 + 5;
		m_fY = 656.0 - 6;
		m_Origin_X = 252;
		m_Origin_Y = 28;
		m_Size_Param = 0.583356f;
		m_UI_Layer = 4;
	}

	// Bar_Bg
	if (m_UI_Desc.m_Type == 3) {
		m_fX = 168.0 + 5;
		m_fY = 656.0 - 6;
		m_Origin_X = 271;
		m_Origin_Y = 66;
		m_Size_Param = 0.583356f;
		m_UI_Layer = 3;
	}

	// Stack_Small
	if (m_UI_Desc.m_Type == 4) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 92;
		m_Origin_Y = 92;
		m_Size_Param = 0.558332f;
		m_UI_Layer = 5;
	}

	// Bar_Color
	if (m_UI_Desc.m_Type == 5) {
		m_fX = 169.0 + 5;
		m_fY = 656.0 - 6;
		m_Origin_X = 252;
		m_Origin_Y = 22;
		m_Size_Param = 0.573356f;
		m_UI_Layer = 5;
	}

	// Stack_Num
	if (m_UI_Desc.m_Type == 6) {
		m_fX = 81;
		m_fY = 631;
		m_Origin_X = 92;
		m_Origin_Y = 92;
		m_Size_Param = 0.574936f;
		m_Origin_Size_Param = m_Size_Param;
		m_UI_Layer = 6;
	}

	// New_Bar
	if (m_UI_Desc.m_Type == 7) {
		m_fX = 168.0 + 5;
		m_fY = 656.0 - 6;
		m_Origin_X = 280;
		m_Origin_Y = 36;
		m_Size_Param = 0.583356f;
		m_UI_Layer = 5;
	}

	// Flower
	if (m_UI_Desc.m_Type == 9) {
		m_fX = 94;
		m_fY = 663;
		m_Origin_X = 76;
		m_Origin_Y = 48;
		m_Size_Param =0.624898f;
		m_UI_Layer = 4;
	}

	// Lang_Frame_Up
	if (m_UI_Desc.m_Type == 10) {
		m_fX = 172;
		m_fY = 588;
		m_Origin_X = 272;
		m_Origin_Y = 64;
		m_Size_Param = 0.574892f;
		m_UI_Layer = 5;
	}

	// Lang_Frame_Down
	if (m_UI_Desc.m_Type == 11) {
		m_fX = 196;
		m_fY = 620.0 - 2;
		m_Origin_X = 272;
		m_Origin_Y = 64;
		m_Size_Param = 0.574892f;
		m_UI_Layer = 5;
	}

	// Lang_Up
	if (m_UI_Desc.m_Type == 12) {
		m_fX = 170;
		m_fY = 590;
		m_Origin_X = 252;
		m_Origin_Y = 48;
		m_Size_Param = 0.5967f;
		m_UI_Layer = 6;
	}
	// Lang_Down
	if (m_UI_Desc.m_Type == 13) {
		m_fX = 190;
		m_fY = 624.0 - 2;
		m_Origin_X = 252;
		m_Origin_Y = 48;
		m_Size_Param = 0.599899f;
		m_UI_Layer = 7;
	}


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CPlayer_Battle_Ult_Frame::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


	if (m_UI_Desc.m_Type == 6) {
		Size_Change(TimeDelta);
	}

	Set_UI();

}

void CPlayer_Battle_Ult_Frame::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	Get_Player_Info(TimeDelta);

	if (m_UI_Desc.m_Type == 6) {
		if (m_Ult_Stack != m_Pre_Ult_Stack) {

			if (m_Ult_Stack > m_Pre_Ult_Stack) {
				m_Size_Up = true;
			}

			m_Pre_Ult_Stack = m_Ult_Stack;
		}
	}

	if (m_UI_Desc.m_Type == 5) {
		m_fX = 169.0 + 5 + 0.287 * 252 * (m_Player_Max_Ult - m_Player_Ult);
		m_Origin_X = (_int)(252 * m_Player_Ult);
	}

	if (m_UI_Desc.m_Type == 7) {
		if (m_Ult_State >= 2) {
			m_Is_Render = true;
		}
		else {
			m_Is_Render = false;
		}
	}

	if (m_UI_Desc.m_Type == 5) {
		if (m_Ult_State == 2) {
			m_UI_Layer = 5.5;
		}
		else {
			m_UI_Layer = 5;
		}
	}

	
	Lang_Render_Control(TimeDelta);


	Get_Mouse_Pos();

	m_Pt_In = Pt_InUI();

	Tool_Funtion(TimeDelta);
	

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CPlayer_Battle_Ult_Frame::Render()
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

HRESULT CPlayer_Battle_Ult_Frame::Add_Components()
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


	if (m_UI_Desc.m_Type == 0 || m_UI_Desc.m_Type == 1 || m_UI_Desc.m_Type == 2 || m_UI_Desc.m_Type == 3) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 4) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Stack"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 5) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Bar_Color"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 6) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Stack_Num"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 7) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Eff_Gauge"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 9) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Ult_Flower"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 10 || m_UI_Desc.m_Type == 11 || m_UI_Desc.m_Type == 12 || m_UI_Desc.m_Type == 13) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player_Battle_Lang_Ult"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CPlayer_Battle_Ult_Frame::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;	

	if (FAILED(m_pShaderCom->SetUp_RawValue("g_Alpha", &m_Alpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->SetUp_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->SetUp_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (m_UI_Desc.m_Type == 0 || m_UI_Desc.m_Type == 1 || m_UI_Desc.m_Type == 2 || m_UI_Desc.m_Type == 3) {

		if (m_UI_Desc.m_Type == 1) {
			if (m_Ult_State == 2) {
				if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 4)))
					return E_FAIL;
			}
			else if (m_Ult_State == 3) {
				if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 5)))
					return E_FAIL;
			}
			else {
				if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 1)))
					return E_FAIL;
			}
		}
		else {
			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_UI_Desc.m_Type)))
				return E_FAIL;
		}
	}
	else if (m_UI_Desc.m_Type == 4) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_Ult_Stack_Frame)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 5) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 5) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 6) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_Ult_Stack)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 7) {
		if (m_Ult_State == 2) {
			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
				return E_FAIL;
		}
		else if (m_Ult_State == 3) {
			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 1)))
				return E_FAIL;
		}

	}
	else if (m_UI_Desc.m_Type == 8) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 2)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 9) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 10) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 3)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 11) {

		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 3)))
			return E_FAIL;

	}
	else if (m_UI_Desc.m_Type == 12) {

		if (m_Ult_Stack > 0) {

			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 1)))
				return E_FAIL;

		}
		if (m_Ult_Stack > 0 && m_Ult_State == 2) {

			if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 2)))
				return E_FAIL;

		}
		
	
	}
	else if (m_UI_Desc.m_Type == 13) {

		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;

	}

	return S_OK;
}

void CPlayer_Battle_Ult_Frame::Set_UI()
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

void CPlayer_Battle_Ult_Frame::Get_Mouse_Pos()
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

void CPlayer_Battle_Ult_Frame::Change_Info(_double TimeDelta)
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

void CPlayer_Battle_Ult_Frame::Tool_Funtion(_double TimeDelta)
{
	//// Åø Àü¿ë 
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

void CPlayer_Battle_Ult_Frame::Get_Player_Info(_double TimeDelta)
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


	if (m_Ult_Stack == 0) {
		m_Ult_Stack_Frame = 0;
	}
	else
	{
		m_Ult_Stack_Frame = 1;
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

void CPlayer_Battle_Ult_Frame::Size_Change(_double TimeDelta)
{
	if (m_Size_Up == true) {
		m_Size_Param = 2.0;
		
		m_Size_Up = false;
		
	}

	if (m_Size_Up == false) {
		m_Size_Param -= (_float)TimeDelta * 7;
		if (m_Size_Param <= m_Origin_Size_Param) {
			m_Size_Param = m_Origin_Size_Param;
		}
	}

}

void CPlayer_Battle_Ult_Frame::Lang_Render_Control(_double TimeDelta)
{
	if (m_UI_Desc.m_Type == 10) {
		if (m_Ult_Stack == 0) {

			m_Is_Render = false;

		}
		if (m_Ult_Stack > 0 ) {

			m_Is_Render = true;

		}
		if (m_Ult_State == 3) {

			m_Is_Render = false;

		}
	}

	if (m_UI_Desc.m_Type == 11) {
		if (m_Ult_Stack == 0) {

			m_Is_Render = false;

		}
		if (m_Ult_Stack > 0) {

			m_Is_Render = true;

		}
		if (m_Ult_Stack > 0 && m_Ult_State == 3) {

			m_Is_Render = true;

		}
	}

	if (m_UI_Desc.m_Type == 12) {
		if (m_Ult_Stack == 0) {

			m_Is_Render = false;

		}
		if (m_Ult_Stack > 0) {

			m_Is_Render = true;

		}
		if (m_Ult_State == 3) {

			m_Is_Render = false;

		}
	}

	if (m_UI_Desc.m_Type == 13) {
		if (m_Ult_Stack == 0) {

			m_Is_Render = false;

		}
		if (m_Ult_Stack > 0) {

			m_Is_Render = true;

		}
		if (m_Ult_Stack > 0 && m_Ult_State == 3) {

			m_Is_Render = true;

		}
	}
}

CPlayer_Battle_Ult_Frame * CPlayer_Battle_Ult_Frame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer_Battle_Ult_Frame*		pInstance = new CPlayer_Battle_Ult_Frame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Battle_Ult_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CPlayer_Battle_Ult_Frame::Clone(void * pArg)
{
	CPlayer_Battle_Ult_Frame*		pInstance = new CPlayer_Battle_Ult_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Battle_Ult_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Battle_Ult_Frame::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

}
