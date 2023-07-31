#include "..\Public\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{

}

const LIGHTDESC* CLight_Manager::Get_Light(_uint iIndex) const
{
	if (m_Lights.size() <= iIndex)
		return nullptr;

	auto iter = m_Lights.begin();
	
	for (_uint i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_LightDesc();
}

void CLight_Manager::Set_Light(_uint iIndex, _uint iOption, _float4 vLightValue)
{
	if (m_Lights.size() <= iIndex)
		return;

	auto iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; i++)
		++iter;

	(*iter)->Set_LightDesc(iOption, vLightValue);
}

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight* pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.emplace_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		if (nullptr != pLight)
			pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

HRESULT CLight_Manager::Clear_Light()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();

	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
