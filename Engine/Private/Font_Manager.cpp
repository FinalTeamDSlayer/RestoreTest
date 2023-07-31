#include "..\Public\Font_Manager.h"
#include "CustomFont.h"

IMPLEMENT_SINGLETON(CFont_Manager)

CFont_Manager::CFont_Manager()
{
}

HRESULT CFont_Manager::Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath)
{
	if (nullptr != Find_Font(pFontTag))
		return E_FAIL;

	CCustomFont* pFont = CCustomFont::Create(pDevice, pContext, pFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(pFontTag, pFont);

	return S_OK;
}

HRESULT CFont_Manager::Draw_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor)
{
	CCustomFont* pFont = Find_Font(pFontTag);

	if (nullptr == pFont)
		return E_FAIL;

	return pFont->DrawFont(pText, vPosition, vColor);
}

HRESULT CFont_Manager::Draw_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, const _float2& vScale)
{
	CCustomFont* pFont = Find_Font(pFontTag);

	if (nullptr == pFont)
		return E_FAIL;

	return pFont->DrawFont(pText, vPosition, vColor, fRotation, vOrigin, vScale);
}

CCustomFont* CFont_Manager::Find_Font(const _tchar* pFontTag)
{
	auto	iter = find_if(m_Fonts.begin(), m_Fonts.end(), CTag_Finder(pFontTag));

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

void CFont_Manager::Free()
{
	for (auto& Pair : m_Fonts)
		Safe_Release(Pair.second);

	m_Fonts.clear();
}


// MakeSpriteFont "¸¶ºñ¿¾Ã¼" /FontSize:30 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 Mabi.spritefont