#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPlayer_Battle_Ult_Effect final : public CUI
{

public:
	typedef struct tagUIDesc
	{
		_bool    m_Is_Reverse = { false };
		_uint    m_Type = { 0 };
		_uint    m_C_Num = { 0 };
	}UIDESC;


private:
	CPlayer_Battle_Ult_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Battle_Ult_Effect(const CPlayer_Battle_Ult_Effect& rhs);
	virtual ~CPlayer_Battle_Ult_Effect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private: /* 내가 사용하고하는 컴포넌트의 기능을 검색없이 바로 사용할 수 있다. */	
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CRenderer*				m_pRendererCom = { nullptr };	
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

public:
	void Set_UI();
	void Get_Mouse_Pos();
	void Change_Info(_double TimeDelta);
	void Tool_Funtion(_double TimeDelta);
	void Get_Player_Info(_double TimeDelta);
	void Size_Change(_double TimeDelta);
	void Alpha_Change(_double TimeDelta);
	void Stack_Up(_double TimeDelta);
	void Stack_Down(_double TimeDelta);


private:
	// Ult 스택증가 및 사용 시 이펙트 관련
	_uint m_Ult_Stack = { 0 };
	_uint m_Pre_Ult_Stack = { 0 };
	_bool m_Is_Ult_Use = { 0 };

	_uint m_Ult_Stack_Frame = { 0 };

	_bool m_Size_Up = { false };
	_bool m_Alpha_Up = { false };

	_float m_Size_Time = { 1.f };
	_float m_Alpha_Time = { 1.f };

	// 0 : 스택 제로 // 1 : 스택 1 이상 // 2 : 개방 // 3 : 완전개방
	_uint m_Ult_State = { 0 };

	_float m_Sprite = { 0 };

	_float m_Stack_Up_Time = { 0.f };
	_float m_Stack_Down_Time = { 0.f };
	_float m_Stack_Dir = { 1.f };

	_bool m_Is_Stack_Up = { false };
	_bool m_Is_Stack_Up_Change = { false };
	
	_bool m_Is_Stack_Down = { false };
	_bool m_Is_Stack_Down_Change = { false };



private:
	_double m_Player_Max_Ult = { 1.000 };
	_double m_Player_Ult = { 1.000 };

	_uint m_Ult_Cnt = { 0 };

private:
	UIDESC                  m_UI_Desc = {};

	_float4x4				m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;	

	_bool                   m_Is_Reverse = { false };

	_float                  m_Alpha = { 1.f };

	_tchar					m_szX[MAX_PATH] = TEXT("");
	_tchar					m_szY[MAX_PATH] = TEXT("");

	_uint m_Level_Index = { 0 };

	_bool m_Is_CutScene = false;

	_bool m_Pt_In = { false };

	_float m_Size_Param = {};

	_float m_Origin_Size_Param = {};
	_float m_Max_Size_Param = {};

	_float m_Size_Dir = { 1.f };
	_float m_Size_Speed = { 1.f };

	_float m_Max_Alpha = { 1.f };
	_float m_Alpha_Dir = { 1.f };
	_float m_Alpha_Speed = { 1.f };



	_int m_Origin_X = {};
	_int m_Origin_Y = {};

	_bool m_Is_Fix = { true };


private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();


public:
	static CPlayer_Battle_Ult_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END