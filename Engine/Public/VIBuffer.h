#pragma once

#include "Component.h"

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	_uint	Get_NumVertices() { return m_iNumVertices; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT	Render();
	

protected:
	ID3D11Buffer*	m_pVB = { nullptr };
	ID3D11Buffer*	m_pIB = { nullptr };
protected:
	D3D11_BUFFER_DESC		m_BufferDesc;
	D3D11_SUBRESOURCE_DATA	m_SubresourceData;

protected:
	_uint					m_iNumVertexBuffers = { 0 };	//버텍스 버퍼의 갯수
	_uint					m_iNumVertices = { 0 };			//정점의 갯수
	_uint					m_iStride = { 0 };				//정점의 크기 (정점의 구조체 크기)
	_uint					m_iNumIndices = { 0 };			//인덱스의 갯수 (삼각형 하나 당 3개)
	_uint					m_iIndexStride = { 0 };			//인덱스의 크기 short/long
	DXGI_FORMAT				m_eIndexFormat;					/* 인덱스 크기가 short이면 R16, long이면 R32*/
	D3D_PRIMITIVE_TOPOLOGY	m_eTopology;					/* 정점을 그리는 방식 - 검색 */

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};