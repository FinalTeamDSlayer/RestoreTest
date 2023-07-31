#include "..\Public\Light.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CLight::CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

void CLight::Set_LightDesc(_uint iOption, _float4 vLightValue)
{
	switch (iOption)
	{
	case 0:
		m_LightDesc.vLightDir = vLightValue;
		break;
	case 1:
		m_LightDesc.vLightDiffuse = vLightValue;
		break;
	case 2:
		m_LightDesc.vLightAmbient = vLightValue;
		break;
	case 3:
		m_LightDesc.vLightSpecular = vLightValue;
		break;
	}
}

HRESULT CLight::Initialize(const LIGHTDESC& LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint iPassIndex = { 0 };

	if (LIGHTDESC::TYPE_DIRECTION == m_LightDesc.eType)
	{
		iPassIndex = 1;

		if (FAILED(pShader->SetUp_RawValue("g_vLightDir", &m_LightDesc.vLightDir, sizeof(_float4))))
			return E_FAIL;

	}
	else if (LIGHTDESC::TYPE_POINT == m_LightDesc.eType)
	{
		iPassIndex = 2;
	}

	if (FAILED(pShader->SetUp_RawValue("g_vLightDiffuse", &m_LightDesc.vLightDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->SetUp_RawValue("g_vLightAmbient", &m_LightDesc.vLightAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pShader->SetUp_RawValue("g_vLightSpecular", &m_LightDesc.vLightSpecular, sizeof(_float4))))
		return E_FAIL;

	pShader->Begin(iPassIndex);

	pVIBuffer->Render();

	return S_OK;
}

CLight* CLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	CLight* pInstance = new CLight(pDevice, pContext);

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		MSG_BOX("Failed to Cloned : CLight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
