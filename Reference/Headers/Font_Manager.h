#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)
private:
	CFont_Manager();
	virtual ~CFont_Manager() = default;

public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Draw_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor);
	HRESULT Draw_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, const _float2& vScale);

private:
	map<const _tchar*, class CCustomFont*>		m_Fonts;

private:
	class CCustomFont* Find_Font(const _tchar* pFontTag);

public:
	virtual void Free();
};

END