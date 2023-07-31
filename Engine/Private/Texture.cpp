#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_ppSRVs(rhs.m_ppSRVs)
{
	for (_uint i = 0; i < m_iNumTextures; i++)
		Safe_AddRef(m_ppSRVs[i]);
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTexFilePath, _uint iNumTextures)
{
	m_ppSRVs = new ID3D11ShaderResourceView*[iNumTextures];

	m_iNumTextures = iNumTextures;

	_tchar szTexFilePath[MAX_PATH] = TEXT("");

	for (_uint i = 0; i < iNumTextures; i++)
	{
		ID3D11ShaderResourceView*	pSRV = { nullptr };

		wsprintf(szTexFilePath, pTexFilePath, i);

		_tchar szExt[MAX_PATH] = TEXT("");

		//C:\Frame\Client\Bin\Resources\Textures\Default.jpg
		_wsplitpath_s(szTexFilePath,
			nullptr, 0,			//C:, 길이 - 상대경로일경우 채워지지 않음
			nullptr, 0,			//\Frame\Client\Bin\Resource\Textures, 길이
			nullptr, 0,			//Default, 길이
			szExt, MAX_PATH		//.jpg, 길이
		);

		if (false == lstrcmp(szExt, TEXT(".dds")))
		{
			if (FAILED(CreateDDSTextureFromFile(m_pDevice, szTexFilePath, nullptr, &pSRV)))
			{
				MSG_BOX("Failed to Created : DDSTexture");
				return E_FAIL;
			}
		}
		else if (false == lstrcmp(szExt, TEXT("tga")))
		{		
			MSG_BOX("Can not Created : TGATexture");
			return E_FAIL;		
		}
		else
		{
			if (FAILED(CreateWICTextureFromFile(m_pDevice, szTexFilePath, nullptr, &pSRV)))
			{
				MSG_BOX("Failed to Created : WICTexture");
				return E_FAIL;
			}
		}

		m_ppSRVs[i] = pSRV;
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResourceView(CShader* pShader, const char* pConstantName, _uint iTextureIndex)
{
	if (m_iNumTextures <= iTextureIndex)
	{
		MSG_BOX("Out of Range : TextureIndex");
		return E_FAIL;
	}

	return pShader->SetUp_SRV(pConstantName, m_ppSRVs[iTextureIndex]);
}

HRESULT CTexture::Bind_ShaderResourceViews(CShader* pShader, const char* pConstantName)
{
	return pShader->SetUp_SRV_Array(pConstantName, m_ppSRVs, m_iNumTextures);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTexFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTexFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Created : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_iNumTextures; i++)
		Safe_Release(m_ppSRVs[i]);

	if (false == m_isCloned)
		Safe_Delete_Array(m_ppSRVs);
}