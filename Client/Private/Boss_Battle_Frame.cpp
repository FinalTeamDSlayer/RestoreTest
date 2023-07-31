#include "pch.h"
#include "..\Public\Boss_Battle_Frame.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"
#include "Player.h"


CBoss_Battle_Frame::CBoss_Battle_Frame(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CBoss_Battle_Frame::CBoss_Battle_Frame(const CBoss_Battle_Frame & rhs)
	: CUI(rhs)
{
}

HRESULT CBoss_Battle_Frame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Battle_Frame::Initialize(void * pArg)
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
		m_fX = 978;
		m_fY = 66;
		m_Origin_X = 868;
		m_Origin_Y = 84;
		m_Size_Param = 0.608333f;
		m_UI_Layer = 1;
	}

	// Face
	if (m_UI_Desc.m_Type == 1) {
		m_fX = 1158;
		m_fY = 39;
		m_Origin_X = 240;
		m_Origin_Y = 240;
		m_Size_Param = 0.349990f;
		m_UI_Layer = 0;
	}

	// Name
	if (m_UI_Desc.m_Type == 2) {
		m_fX = 1071;
		m_fY = 48;
		m_Origin_X = 264;
		m_Origin_Y = 88;
		m_Size_Param = 0.249815f;
		m_UI_Layer = 2;
	}
		

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CBoss_Battle_Frame::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	
	if (m_UI_Desc.m_C_Num == 0) {

		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 1) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 2) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 3) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 4) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 5) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 6) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 7) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}
	if (m_UI_Desc.m_C_Num == 8) {
		if (m_UI_Desc.m_Type == 1) {

		}
		else {

		}
	}

	Set_UI();

}

void CBoss_Battle_Frame::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	
	Get_Mouse_Pos();

	m_Pt_In = Pt_InUI();

	Tool_Funtion(TimeDelta);
	

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CBoss_Battle_Frame::Render()
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

	
	if (m_Is_CutScene == false) {

		m_pVIBufferCom->Render();

	}
	
	
	
	return S_OK;
}

HRESULT CBoss_Battle_Frame::Add_Components()
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



	if (m_UI_Desc.m_Type == 0) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Frame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 1) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Face"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 2) {
		/* For.Com_Texture */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Battle_Name"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	


	return S_OK;
}

HRESULT CBoss_Battle_Frame::SetUp_ShaderResources()
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

	if (m_UI_Desc.m_Type == 0) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_UI_Desc.m_Type)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 1) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_UI_Desc.m_C_Num)))
			return E_FAIL;
	}
	else if (m_UI_Desc.m_Type == 2) {
		if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_Texture", m_UI_Desc.m_C_Num)))
			return E_FAIL;
	}





	return S_OK;
}

void CBoss_Battle_Frame::Set_UI()
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

void CBoss_Battle_Frame::Get_Mouse_Pos()
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

void CBoss_Battle_Frame::Change_Info(_double TimeDelta)
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

void CBoss_Battle_Frame::Tool_Funtion(_double TimeDelta)
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

CBoss_Battle_Frame * CBoss_Battle_Frame::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBoss_Battle_Frame*		pInstance = new CBoss_Battle_Frame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoss_Battle_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject * CBoss_Battle_Frame::Clone(void * pArg)
{
	CBoss_Battle_Frame*		pInstance = new CBoss_Battle_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBoss_Battle_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Battle_Frame::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

}
