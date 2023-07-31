#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;

END

BEGIN(Client)

class CMapObject  abstract : public CGameObject
{
public:
	enum MAPOBJECT_TYPE { MAPOBJECT_STATIC, MAPOBJECT_TERRAIN, MAPOBJECT_ROTATION, MAPOBJECT_INSTANCE, MAPOBJECT_END };
	enum INSTANCE_TYPE { INSTANCE_NONE, INSTANCE_DEFAULT, INSTANCE_GRASS, INSTANCE_END };
	enum ARRAGNGEMENT_TYPE { ARRANGEMENT_RECT, ARRANGEMENT_LINEX, ARRANGEMENT_LINEY, ARRANGEMENT_END };
	enum SCENETYPE { SCENE_STATIC, SCENE_GAMEPLAY, SCENE_VILLAGE, SCENE_HOUSE, SCENE_TRAIN, SCENE_FINALBOSS, SCENE_END };

	typedef struct tagMapObjectInfo
	{
		_float4		vPos;
		_float3		vRotAngle;
		_float3		vScale;

		_tchar		szMeshName[128];
		_uint		iMapObjectType;

		_uint		iNumInstance;
		_float		fRange;
		_float		fMinSize;
		_float		fMaxSize;

		_uint		iInstanceType = 0;

		_bool		bRandomRatationY = false;
		_uint		iArrangementType = 0;

		_uint		iSceneType = 0;

	}MAPOBJECT_INFO;

protected:
	CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapObject(const CMapObject& rhs);
	virtual ~CMapObject() = default;

public:
	_tchar* Get_PrototypeObjectTag() {
		return m_PrototypeObjectTag;
	}

	MAPOBJECT_INFO Get_ObjectInfo() {
		return m_MapObject_Info;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

protected:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:
	_tchar					m_PrototypeObjectTag[MAX_PATH] = L"";

	MAPOBJECT_INFO			m_MapObject_Info;

public:
	virtual void Free() override;
};

END
