#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		HWND hWnd;
		WINMODE eWinMode;
		unsigned int iWinSizeX;
		unsigned int iWinSizeY;
	}GRAPHICDESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTION, TYPE_POINT, TYPE_END };

		TYPE			eType;
		XMFLOAT4		vLightDir;
		XMFLOAT4		vLightDiffuse;
		XMFLOAT4		vLightAmbient;
		XMFLOAT4		vLightSpecular;

	}LIGHTDESC;

	typedef struct tagMeshMaterial
	{
		enum TEXTURETYPE{
			TextureType_NONE				= 0, 
			TextureType_DIFFUSE				= 1, 
			TextureType_SPECULAR			= 2,
			TextureType_AMBIENT				= 3,
			TextureType_EMISSIVE			= 4,
			TextureType_HEIGHT				= 5,
			TextureType_NORMALS				= 6,
			TextureType_SHININESS			= 7,
			TextureType_OPACITY				= 8,
			TextureType_DISPLACEMENT		= 9,
			TextureType_LIGHTMAP			= 10,
			TextureType_REFLECTION			= 11,
			TextureType_BASE_COLOR			= 12,
			TextureType_NORMAL_CAMERA		= 13,
			TextureType_EMISSION_COLOR		= 14,
			TextureType_METALNESS			= 15,
			TextureType_DIFFUSE_ROUGHNESS	= 16,
			TextureType_AMBIENT_OCCLUSION	= 17,
			TextureType_UNKNOWN				= 18,
		};

		class CTexture* pTexture[TEXTURE_TYPE_MAX];
	}MESHMATERIALS;

	typedef struct tagKeyFrame
	{
		double		dTime;
		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;
	}KEYFRAME;

#pragma region Buffer

	typedef struct tagVertex_Position
	{
		XMFLOAT3 vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		const static unsigned int iNumElements = { 1 };
		static D3D11_INPUT_ELEMENT_DESC Elements[1];
	}VTXPOS_DECL;

	typedef struct tagVertex_Position_UV
	{
		XMFLOAT3 vPosition;
		XMFLOAT2 vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Position_UV_Declaration
	{
		const static unsigned int iNumElements = { 2 };
		static D3D11_INPUT_ELEMENT_DESC Elements[2];
	}VTXTEX_DECL;

	typedef struct tagVertex_Position_Normal_UV
	{
		XMFLOAT3 vPosition;
		XMFLOAT3 vNormal;
		XMFLOAT2 vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_UV_Declaration
	{
		const static unsigned int iNumElements = { 3 };
		static D3D11_INPUT_ELEMENT_DESC Elements[3];
	}VTXNORTEX_DECL;

	typedef struct tagVertex_Position_Cube_UV
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Position_Cube_UV_Declaration
	{
		const static unsigned int			iNumElements = { 2 };
		static D3D11_INPUT_ELEMENT_DESC		Elements[2];
	}VTXCUBETEX_DECL;

	typedef struct tagVertex_Model
	{
		XMFLOAT3 vPosition;
		XMFLOAT3 vNormal;
		XMFLOAT2 vTexUV;
		XMFLOAT3 vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertex_Model_Declaration
	{
		const static unsigned int iNumElements = { 4 };
		static D3D11_INPUT_ELEMENT_DESC Elements[4];
	}VTXMODEL_DECL;

	typedef struct tagVertex_Animation_Model
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vNormal;
		XMFLOAT2	vTexUV;
		XMFLOAT3	vTangent;
		XMUINT4		vBlendIndices;
		XMFLOAT4	vBlendWeights;
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertex_Animation_Model_Declaration
	{
		const static unsigned int iNumElements = { 6 };
		static D3D11_INPUT_ELEMENT_DESC Elements[6];
	}VTXANIMMODEL_DECL;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;
		XMFLOAT4		vColor;
		XMFLOAT2		vPSize;
		float			fAngle;
	}VTXINSTANCE;

	typedef struct tagVertex_ModelInstance
	{
		XMFLOAT4				vRight;
		XMFLOAT4				vUp;
		XMFLOAT4				vLook;
		XMFLOAT4				vTranslation;

		unsigned int			iNumInstance;

	}VTXMODELINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Rect_Instance_Declaration
	{
		static const unsigned int			iNumElements = { 9 };
		static D3D11_INPUT_ELEMENT_DESC		Elements[9];
	}VTXRECTINSTANCE_DECL;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		const static unsigned int			iNumElements = { 8 };
		static D3D11_INPUT_ELEMENT_DESC		Elements[8];
	}VTXPOINTINSTANCE_DECL;

	typedef struct ENGINE_DLL tagVertex_MODEL_Instance_Declaration
	{
		const static unsigned int			iNumElements = { 8 };
		static D3D11_INPUT_ELEMENT_DESC		Elements[8];
	}VTXMODELINSTANCE_DECL;

	typedef struct tagModelInstanceDesc
	{
		unsigned int		iNumInstance;
		float				fRange;
		float				fMinSize;
		float				fMaxSize;

		bool		bRandomRatationY = false;
		unsigned int		iArrangementType = 0;

	}MODELINSTANCEDESC;

#pragma endregion

#pragma region Data
#pragma region Bones
	typedef struct tagBoneData
	{
		unsigned int	iNameSize;
		char			szName[MAX_PATH] = { "" };
		XMFLOAT4X4		TransformationMatrix;
		int				iParentIndex;
	}BONEDATA;
#pragma endregion
#pragma region Meshes
	typedef struct tagWeightData
	{
		unsigned int	iVertexID;
		float			fWeights;
	}WEIGHTDATA;

	typedef struct tagAnimMeshData
	{
		XMFLOAT4X4		OffsetMatrix;
		unsigned int	iNameSize;
		char			szName[MAX_PATH] = { "" };

		unsigned int	iNumWeights;
		WEIGHTDATA*		pWeightData;
	}ANIMMESHDATA;

	typedef struct tagMeshIdxData
	{
		unsigned int	iIndex0;
		unsigned int	iIndex1;
		unsigned int	iIndex2;
	}MESHIDXDATA;

	typedef struct tagMeshVtxData
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}MESHVTXDATA;

	typedef struct tagMeshData
	{
		unsigned int	iNameSize;
		char			szName[MAX_PATH] = { "" };
		unsigned int	iMaterialIndex;
		
		unsigned int	iNumVertices;
		MESHVTXDATA*	pMeshVtxData;
		
		unsigned int	iNumFaces;
		MESHIDXDATA*	pMeshIdxData;

		unsigned int	iNumBones;
		ANIMMESHDATA*	pAnimMeshData;
	}MESHDATA;

#pragma endregion
#pragma region Materials

	typedef struct tagMaterialData
	{
		unsigned int	iNameSize;
		char			szName[MAX_PATH] = { "" };
	}MATERIALDATA;
#pragma endregion
#pragma region Animations
	typedef struct tagKeyFrameData
	{
		unsigned int	iScaleKey;
		XMFLOAT3		vScale;
		double			dScaleTime;

		unsigned int	iRotationKey;
		XMFLOAT4		vRotation;
		double			dRotationTime;

		unsigned int	iPositionKey;
		XMFLOAT3		vPosition;
		double			dPositionTime;
	}KEYFRAMEDATA;

	typedef struct tagChannelData
	{
		unsigned int	iNameSize;
		char			szName[MAX_PATH] = { "" };

		unsigned int	iNumKeyFrames;
		KEYFRAMEDATA*	pKeyFrameData;
	}CHANNELDATA;

	typedef struct tagAnimationData
	{
		unsigned int	iNameSize;
		char			szName[MAX_PATH] = { "" };

		double			dDuration;
		double			dTickPerSecond;

		unsigned int	iNumChannels;
		CHANNELDATA*	pChannelData;
	}ANIMATIONDATA;
#pragma endregion

	typedef struct tagModelData
	{
		unsigned int	iNumBones;
		BONEDATA*		pBoneData;

		unsigned int	iNumMeshes;
		MESHDATA*		pMeshData;

		unsigned int	iNumMaterials;
		MATERIALDATA*	pMaterialData;

		unsigned int	iNumAnimations;
		ANIMATIONDATA*	pAnimationData;
	}MODELDATA;

#pragma endregion
}

