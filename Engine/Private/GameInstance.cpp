#include "GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Font_Manager.h"
#include "Target_Manager.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager{ CTimer_Manager::GetInstance() }
	, m_pGraphic_Device{ CGraphic_Device::GetInstance() }
	, m_pInput_Device{ CInput_Device::GetInstance() }
	, m_pLevel_Manager{ CLevel_Manager::GetInstance() }
	, m_pObject_Manager{ CObject_Manager::GetInstance() }
	, m_pComponenet_Manager{ CComponent_Manager::GetInstance() }
	, m_pLight_Manager{ CLight_Manager::GetInstance() }
	, m_pFont_Manager{ CFont_Manager::GetInstance()}
	, m_pTarget_Manager{ CTarget_Manager::GetInstance() }
	, m_pPipeLine{ CPipeLine::GetInstance() }
	, m_pFrustum{ CFrustum::GetInstance() }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pComponenet_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pFrustum);
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Ready_DInput(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Containers(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponenet_Manager->Reserve_Containers(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pLevel_Manager->Reserve_Containers(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pFrustum->Initialize()))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_double dTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pInput_Device->Update_DInput();

	m_pObject_Manager->Tick(dTimeDelta);

	m_pPipeLine->Tick();

	m_pFrustum->Tick();

	m_pObject_Manager->LateTick(dTimeDelta);

	m_pLevel_Manager->Tick_Level(dTimeDelta);
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	m_pObject_Manager->Clear(iLevelIndex);
}

_double CGameInstance::Get_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0;

	return m_pTimer_Manager->Get_Timer(pTimerTag);
}

void CGameInstance::Set_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	m_pTimer_Manager->Set_Timer(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte ubyKeyID)
{
	if (nullptr == m_pInput_Device)
		return _byte();

	return m_pInput_Device->Get_DIKeyState(ubyKeyID);
}

_bool CGameInstance::Get_DIKeyDown(_ubyte ubyKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_DIKeyDown(ubyKeyID);
}

_bool CGameInstance::Get_DIKeyUp(_ubyte ubyKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_DIKeyUp(ubyKeyID);
}

_byte CGameInstance::Get_DIMouseState(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return _byte();

	return m_pInput_Device->Get_DIMouseState(eMouseID);
}

_bool CGameInstance::Get_DIMouseDown(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_DIMouseDown(eMouseID);
}

_bool CGameInstance::Get_DIMouseUp(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_DIMouseUp(eMouseID);
}

_long CGameInstance::Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return _long();

	return m_pInput_Device->Get_DIMouseMove(eMouseMoveID);
}

_bool CGameInstance::Get_AnyKeyPressing()
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Get_AnyKeyPressing();
}

CLevel* CGameInstance::Get_CulLevel() const
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->Get_CulLevel();
}

_uint CGameInstance::Get_CurLevelIdx() const
{
	if (nullptr == m_pLevel_Manager)
		return false;

	return m_pLevel_Manager->Get_CurLevelIdx();
}

CLevel* CGameInstance::Get_LoadedStage(_uint iLevelIndex) const
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->Get_LoadedStage(iLevelIndex);
}

_bool CGameInstance::Get_IsStage() const
{
	if (nullptr == m_pLevel_Manager)
		return false;

	return m_pLevel_Manager->Get_IsStage();
}

_bool CGameInstance::Get_IsLoadForAll() const
{
	if (nullptr == m_pLevel_Manager)
		return false;

	return m_pLevel_Manager->Get_IsLoadForAll();
}

void CGameInstance::Set_IsLoadForAll()
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Set_IsLoadForAll();
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pNextLevel, _bool isStage, _bool isRelease)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNextLevel, isStage, isRelease);
}

HRESULT CGameInstance::Swap_Level(_uint iLevelIndex)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Swap_Level(iLevelIndex);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(iLevelIndex, pLayerTag, iIndex);
}

list<class CGameObject*>* CGameInstance::Get_GameObjects(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObjects(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg, _bool isKeep)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg, isKeep);
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pGameObject, _bool isKeep)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pGameObject, isKeep);
}

CGameObject* CGameInstance::Clone_GameObject(const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_GameObject(pPrototypeTag, pArg);
}

void CGameInstance::Clear_Layer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear_Layer(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponenet_Manager)
		return E_FAIL;

	return m_pComponenet_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pComponenet_Manager)
		return nullptr;

	return m_pComponenet_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

const LIGHTDESC* CGameInstance::Get_Light(_uint iIndex) const
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_Light(iIndex);
}

void CGameInstance::Set_Light(_uint iIndex, _uint iOption, _float4 vLightValue)
{
	if (nullptr == m_pLight_Manager)
		return;

	m_pLight_Manager->Set_Light(iIndex, iOption, vLightValue);
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

HRESULT CGameInstance::Clear_Light()
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Clear_Light();
}

HRESULT CGameInstance::Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice, pContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Draw_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Draw_Font(pFontTag, pText, vPosition, vColor);
}

HRESULT CGameInstance::Draw_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, const _float2& vScale, _fvector vColor, _float fRotation, const _float2& vOrigin)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Draw_Font(pFontTag, pText, vPosition, vColor, fRotation, vOrigin, vScale);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE iTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(iTransformState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE iTransformState) const
{
	if (nullptr == m_pPipeLine)
		return _float4x4(1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_TransformFloat4x4(iTransformState);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE iTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix_Inverse(iTransformState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE iTransformState) const
{
	if (nullptr == m_pPipeLine)
		return _float4x4(1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_TransformFloat4x4_Inverse(iTransformState);
}

_float4 CGameInstance::Get_CameraPosition() const
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CameraPosition();
}

_bool CGameInstance::isIn_WorldSpace(_fvector vWorldPos, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}

void CGameInstance::Release_Engine()
{
	CPipeLine::GetInstance()->DestroyInstance();

	CGameInstance::GetInstance()->DestroyInstance();

	CObject_Manager::GetInstance()->DestroyInstance();

	CComponent_Manager::GetInstance()->DestroyInstance();

	CLight_Manager::GetInstance()->DestroyInstance();

	CTimer_Manager::GetInstance()->DestroyInstance();

	CLevel_Manager::GetInstance()->DestroyInstance();

	CFont_Manager::GetInstance()->DestroyInstance();

	CFrustum::GetInstance()->DestroyInstance();

	CTarget_Manager::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->DestroyInstance();

	CGraphic_Device::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pFrustum);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pComponenet_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTimer_Manager);
}
