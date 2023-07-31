#pragma once

#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Rect_Instance.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Rect.h"
#include "Calculator.h"
#include "Navigation.h"
#include "Transform.h"
#include "Collider.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Model_Instance.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT				Reserve_Containers(_uint iNumLevels);
	HRESULT				Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, class CComponent* pPrototype);
	class CComponent*	Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void* pArg);

private:
	_uint	m_iNumLevels = { 0 };

	unordered_map<const _tchar*, class CComponent*>* m_pPrototypes = { nullptr };
	typedef unordered_map<const _tchar*, class CComponent*> PROTYPES;
private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag);

public:
	virtual void Free() override;

};

END