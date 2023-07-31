#pragma once
#include "Component_Manager.h"
#include "Input_Device.h"
#include "PipeLine.h"

BEGIN(Engine)
class ENGINE_DLL CGameInstance final :
    public CBase
{
        DECLARE_SINGLETON(CGameInstance)
private:
    CGameInstance();
    virtual ~CGameInstance() = default;

public: /*for Engine*/
    HRESULT     Initialize_Engine(_In_ HINSTANCE hInst, _In_ _uint iNumLevels, _In_ const GRAPHICDESC & GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
    void        Tick_Engine(_double dTimeDelta);
    void        Clear(_uint iLevelIndex);

public: /*for Timer_Manager*/
    _double		Get_Timer(const _tchar * pTimerTag);
    void		Set_Timer(const _tchar * pTimerTag);
    HRESULT		Ready_Timer(const _tchar * pTimerTag);

public: /*for Graphic_Device*/
    HRESULT     Clear_BackBuffer_View(_float4 vClearColor);
    HRESULT     Clear_DepthStencil_View();
    HRESULT     Present();

public: /*for Input_Device*/
    _byte		Get_DIKeyState(_ubyte ubyKeyID);
    _bool		Get_DIKeyDown(_ubyte ubyKeyID);
    _bool		Get_DIKeyUp(_ubyte ubyKeyID);
    _byte		Get_DIMouseState(CInput_Device::MOUSEKEYSTATE eMouseID);
    _bool		Get_DIMouseDown(CInput_Device::MOUSEKEYSTATE eMouseID);
    _bool		Get_DIMouseUp(CInput_Device::MOUSEKEYSTATE eMouseID);
    _long		Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID);
    _bool		Get_AnyKeyPressing();

public: /*for Level_Manager*/
    class CLevel*   Get_CulLevel() const;
    _uint           Get_CurLevelIdx() const;
    class CLevel*   Get_LoadedStage(_uint iLevelIndex) const;
    _bool           Get_IsStage() const;
    _bool           Get_IsLoadForAll() const;
    void            Set_IsLoadForAll();
    HRESULT         Open_Level(_uint iLevelIndex, class CLevel* pNextLevel, _bool isStage = false, _bool isRelease = true);
    HRESULT         Swap_Level(_uint iLevelIndex);

public: /*for Object_Manager*/
    class CComponent*           Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag);
    class CGameObject*          Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex = 0);
    list<class CGameObject*>*   Get_GameObjects(_uint iLevelIndex, const _tchar * pLayerTag);
    HRESULT		                Add_Prototype(const _tchar * pPrototypeTag, class CGameObject* pPrototype);
    HRESULT		                Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void* pArg = nullptr);
    class CGameObject*          Clone_GameObject(const _tchar * pPrototypeTag, void* pArg);
    void                        Clear_Layer(_uint iLevelIndex, const _tchar * pLayerTag);

public: /*for Component_Manager*/
    HRESULT             Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, class CComponent* pPrototype);
    class CComponent*   Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void* pArg = nullptr);

public: /*for Light_Manager*/
    const LIGHTDESC*    Get_Light(_uint iIndex = 0) const;
    void                Set_Light(_uint iIndex, _uint iOption, _float4 vLightValue);
    HRESULT             Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc);
    HRESULT             Clear_Light();

public: /* For.Font_Manager */
    HRESULT Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontTag, const _tchar * pFontFilePath);
    HRESULT Draw_Font(const _tchar * pFontTag, const _tchar * pText, const _float2 & vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f));
    HRESULT Draw_Font(const _tchar * pFontTag, const _tchar * pText, const _float2 & vPosition, const _float2 & vScale, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRotation = 0.f, const _float2 & vOrigin = _float2());

public: /*for PipeLine*/
    _matrix		Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE iTransformState) const;
    _float4x4	Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE iTransformState) const;
    _matrix		Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE iTransformState) const;
    _float4x4	Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE iTransformState) const;
    _float4		Get_CameraPosition() const;

public: /* For.Frustum */
    _bool isIn_WorldSpace(_fvector vWorldPos, _float fRange = 0.f);

public: /*for Engine*/
    static void Release_Engine();

private:
    class CTimer_Manager*       m_pTimer_Manager        = { nullptr };
    class CGraphic_Device*      m_pGraphic_Device       = { nullptr };
    class CInput_Device*        m_pInput_Device         = { nullptr };
    class CLevel_Manager*       m_pLevel_Manager        = { nullptr };
    class CObject_Manager*      m_pObject_Manager       = { nullptr };
    class CComponent_Manager*   m_pComponenet_Manager   = { nullptr };
    class CLight_Manager*       m_pLight_Manager        = { nullptr };
    class CFont_Manager*        m_pFont_Manager         = { nullptr };
    class CTarget_Manager*      m_pTarget_Manager       = { nullptr };
    class CPipeLine*            m_pPipeLine             = { nullptr };
    class CFrustum*             m_pFrustum = { nullptr };

public:
    virtual void Free() override;

};

END