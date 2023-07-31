
#include "Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix		g_BoneMatrices[256];

float4		g_vCamPosition;

texture2D	g_DiffuseTexture;

float		g_fFar = 300.f;

//OutLineColor_JH
float4			g_lineColor = float4(0.f, 0.f, 0.f, 1.f);
float			g_OutlineThickness;
float			g_OutlineFaceThickness;

//외곽선 쉐이딩
float3x3      Kx = { -1, 0, 1,
				  -2, 0, 2,
				  -1, 0, 1 };

float3x3      Ky = { 1, 2, 1,
				  0, 0, 0,
				  -1, -2, -1 };

struct VS_IN
{
	float3		vPosition	: POSITION;
	float3		vNormal		: NORMAL;
	float2		vTexUV	: TEXCOORD0;
	float3		vTangent	: TANGENT;
	uint4		vBlendIndices : BLENDINDEX;
	float4		vBlendWeights : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_Main(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matrix		matWVP = mul(matWV, g_ProjMatrix);

	/*모델의 특정 메시파트가 혹여 뼈의 영향을 받지 않고 그려진다라면. BlendIndices, BlednWeight모두다 0으로 채워져서 들어오는 경우가 발생하낟. */
	float		fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

	/* */
	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;

	/* 로컬스페이스 내에서 애님움직임에 ㅁ맞도록 정점을 변환시키낟. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT VS_Outline(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matrix		matWVP = mul(matWV, g_ProjMatrix);

	/*모델의 특정 메시파트가 혹여 뼈의 영향을 받지 않고 그려진다라면. BlendIndices, BlednWeight모두다 0으로 채워져서 들어오는 경우가 발생하낟. */
	float		fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

	/* */
	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;

	/* 로컬스페이스 내에서 애님움직임에 ㅁ맞도록 정점을 변환시키낟. */
	vector		vPosition = mul(vector(In.vPosition.xyz + In.vNormal.xyz * g_OutlineThickness, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	return Out;
}
VS_OUT VS_OutlineFace(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matrix		matWVP = mul(matWV, g_ProjMatrix);

	/*모델의 특정 메시파트가 혹여 뼈의 영향을 받지 않고 그려진다라면. BlendIndices, BlednWeight모두다 0으로 채워져서 들어오는 경우가 발생하낟. */
	float		fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

	/* */
	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;

	/* 로컬스페이스 내에서 애님움직임에 ㅁ맞도록 정점을 변환시키낟. */
	vector		vPosition = mul(vector(In.vPosition.xyz + In.vNormal.xyz * g_OutlineFaceThickness, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);


	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT VS_MAIN_SHADOW(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 하드웨어 스키닝. */
	float		fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;

	vector		vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = normalize(mul(float4(In.vNormal, 0.f), BoneMatrix));

	/*vector		vPosition = mul(vector(In.vPosition.xyz + In.vNormal.xyz * 0.005f, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);*/

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};

struct PS_OUT_DEFERRED
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
};

PS_OUT  PS_Main(PS_IN _In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearClampSampler, _In.vTexUV);

	//if (vMtrlDiffuse.a < 0.1f)
	//	discard;

	Out.vDiffuse = vMtrlDiffuse;
	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(_In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(_In.vProjPos.w / 1.f, _In.vProjPos.z / _In.vProjPos.w, 0.f, 0.f);
	//(뷰 스페이스의 z, 투영 스페이스의 z, 0.f, 0.f)

	return Out;
};
//OutLine_PS
PS_OUT  PS_Outline(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	

	
	float4 outlineColor = g_lineColor;

	float4 diffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (diffuseColor.a < 0.1f)
		discard;

	// 외곽선 색상을 덮어씌우기 위해 Alpha 값을 1로 설정
	outlineColor.a = 1.0f;

	// 외곽선 두께를 적용하여 외곽선 색상과 일반 렌더링 결과를 합성
	float outlineFactor = saturate(length(In.vNormal.xyz) * g_OutlineThickness);
	float blendFactor = smoothstep(0.f, 1.f, outlineFactor); // smoothstep(float edge0, float edge1, float x); edge 0 : 보간시작 edge 1 : 보간 끝나느 값 x: 보간대상

	vector Color = lerp(diffuseColor, outlineColor, blendFactor);

	Out.vDiffuse = Color;

	// In.vNormal xyz각각이 -1 ~ 1
	// Out.vNormal 저장받을 수 있는 xyz각각 0 ~ 1
	//Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	//Out.vDepth = vector(In.vProjPos.w / 300, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	return Out;
}
PS_OUT_DEFERRED PS_MAIN_SHADOW(PS_IN In)
{
	PS_OUT_DEFERRED		Out = (PS_OUT_DEFERRED)0;

	Out.vDiffuse.r = In.vProjPos.w / 300.f;

	Out.vDiffuse.a = 1.f;

	return Out;
}
	

technique11 DefaultTechnique
{
	pass General // 0
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_Default, 0);

		VertexShader = compile vs_5_0 VS_Main();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_Main();
	}
	
	pass Outline // 1
	{
		SetRasterizerState(RS_CULL_CW);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_Default, 0);

		VertexShader = compile vs_5_0 VS_Outline();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_Outline();
	}

	pass OutlineFace // 2
	{
		SetRasterizerState(RS_CULL_CW);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DS_Default, 0);

		VertexShader = compile vs_5_0 VS_OutlineFace();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_Outline();
	}

	pass Shadow  // 3
	{
		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

		VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
	}
};



////======================================셰이더 참고용=============================================
//// 셰이더 = 사용자 정의 렌더링 파이프 라인
//// DX9 = 고정기능 렌더링 파이프 라인
//
//// 장치가 알아서 해주던 렌더링 파이프라인을 사용하지 않고
//// 직접 렌더링 파이프라인의 기능을 구현하면서
//// 기능 추가 및 최적화를 하는것 = 셰이더를 사용한다
//
//// 셰이더가 구동되는 순서
//// 셰이더 코드를 객체화시켜야 한다 - 셰이더 클래스 컴포넌트를 만들어서 처리할 것
//// ID3DX11Effect* pEffect 객체 - 셰이더 클래스가 갖고있을것
//// pEffect에서 Apply() - 셰이더로 그릴것이다.
//
//// m_pContext->DrawIndexed();
//// 인덱스 버퍼의 첫번째 인덱스의 정점을 꺼내오고
//// 그 정점을 인자로 버텍스 셰이더 함수를 호출한다 (VS_Main())
//// 함수 호출이 끝나고, 뷰포트 변환을 수행, 뷰포트 변환까지 끝나면
//// 다음 인덱스의 정점을 가져와서 다시 연산을 시작한다.
//// 3번째 정점까지 연산이 끝나면 (삼각형을 그릴 수 있게 되면)
//// 래스터라이즈를 진행한다. - 정점의 정보를 기반으로 픽셀을 생성해나가는 과정
//
////셰이더 형식이 /fx (효과) 셰이더이기 때문에 technique11을 선언한다
////어떤 함수를 호출알 것인지 셰이더 파일 안에서 알려주는것
////다른 형식일 경우에는 셰이더 파일에서 함수만 만들고, 클라이언트에서 어떤 함수를 호출할지 설정해준다.
////각각의 하드웨어에 맞는 셰이더 버전으로 셰이더를 구동해 줄 때 다수의 technique11을 선언한다
//
//struct VS_IN
//{
//	// 클라에서 던져주는 정점과 같은 구조체 - 정점의 종류 별로 다른 셰이더 파일을 만들 것
//	// ex)VTXTEX
//	// : POSITION - 시맨틱 - D3DDECLUSAGE 열거체의 _POSITION, _NORMAL 같은것
//	// 멤버변수의 시맨틱은 가독성을 위해 사용할것
//	float3	vPosition : POSITION;
//	float2	vTexCoord : TEXCOORD0;
//};
//
//struct VS_OUT
//{
//	// 현재 코드에서는 VS_Main()에서 월드, 뷰, 투영 변환을 모두 진행한다
//	// 투영 행렬을 곱한 뒤에 w나누기까지 해야 투영변환이 완료된다.
//	// VS_IN 값에서 Position은 float3이라서 w가 없다
//	// 투영변환을 하려면 w값이 있어야 w나누기를 할 수 있기때문에 float4로 내보내야 한다
//	// IN과 OUT의 자료형이 달라지기 때문에 IN, OUT을 따로 설정했다.
//
//
//	float4 vPosition : SV_POSITION;
//	float2 vTexCoord : TEXCOORD0;
//};
//
//VS_OUT VS_Main(VS_IN _In)
//{
//	//정점 하나를 받아서 정의한 기능을 수행할 정점 셰이더
//
//	//_In.vPosition * 월드 * 뷰 * 투영
//	// mul() - 행렬을 곱할 때 사용
//	// DX9에서 사용했던 TransformCoord와 다르게 자동으로 z나누기를 수행하지 않는다.
//	// 단순히 곱하기의 기능
//}
//
////위에서 월드, 뷰, 투영 행렬을 모두 곱하고 함수가 종료되면
//// 자동으로 z 나누기를 수행해서 투영 변환을 완료한다.
//// 이후 뷰포트 변환을 한다.
//
////래스터라이즈
//struct PS_IN
//{
//	// 픽셀 셰이더에서 사용하는 구조체
//	// 픽셀은 정점을 기반으로 만들어졌기 때문에 VS_OUT으로 사용해도 무방하지만
//	// 가독성을 위해 PS_IN으로 만든것
//
//	//픽셀 셰이딩 단계에서는 이미 픽셀의 위치가 정해져있기 때문에 Position은 사실 필요하지 않다 - 생략해도 무관
//	float4 vPosition : SV_POSITION;
//	float2 vTexCoord : TEXCOORD0;
//};
//
//struct PS_OUT
//{
//	//픽셀 셰이더의 OUT구조체
//	//픽셀 셰이더는 픽셀의 색을 결정하기 때문에 색을 담은 float4(vector)만 있으면 된다.
//	//TARGET는 0~7까지 존재, 각 번호의 렌더타겟에 멤버변수의 색으로 그린다
//	vector vColor : SV_TARGET0;
//	vector vShade : SV_TARGET1;
//};
//
//
//// vector PS_Main(PS_IN _In) : SV_TARGET0 으로 사용해도 되지만 OUT 구조체를 따로 만들어 사용할것(정쌤의 경우)
//PS_OUT PS_Main(PS_IN _In)
//{
//	// 픽셀의 최종적인 색을 결정한다.
//	// 픽셀의 수 만큼 수행되기 때문에 최대한 가볍게 만드는게 좋다
//
//	PS_OUT vOut = (PS_OUT)0;	//vOut을 0으로 초기화
//
//
//	return vOut;
//}
//
//technique11 DefaultTechnique
//{
//	//사용하지 않을 셰이더를 명시하지 않으면 다른 셰이더 구동 시
//	//사용하지 않을 셰이더가 구동된다.
//	//따라서 사용하지 않도록 명시해 줄 것이다. (정쌤의 경우)
//
//	//VertexShader = compile vs_5_0 VS_Main();
//	// 버텍스 셰이더 = vs_5_0버전으로 번역하고, VS_Main() 함수를 호출한다
//	// VS_Main()함수가 존재하지 않으면 오류가 발생한다
//	//GeometryShader = NULL;
//	//HullShader = NULL;
//	//DomainShader = NULL;
//	//PixelShader = compile vs_5_0 PS_Main();
//	// 픽셀 셰이더 = vs_5_0버전으로 번역하고, PS_Main() 함수를 호출한다
//
//
//
//	//pass - 기능의 캡슐화
//
//	//0번 상태 - 일반 - 그림자, 노말맵, 모델링 등을 렌더
//	//1번 상태 - 은폐 - 노이즈맵으로 블러효과만을 렌더
//	//등의 형태로 같은 정점에 렌더 상태를 다르게 할 경우에 사용하기 좋다
//	//[같은 정점] -- 중요
//	pass General
//	{
//		VertexShader = compile vs_5_0 VS_Main();
//		GeometryShader = NULL;
//		HullShader = NULL;
//		DomainShader = NULL;
//		PixelShader = compile vs_5_0 PS_Main();
//	};
//
//	pass Cloaking
//	{
//		VertexShader = compile vs_5_0 VS_Main();
//		GeometryShader = NULL;
//		HullShader = NULL;
//		DomainShader = NULL;
//		PixelShader = compile vs_5_0 PS_Main();
//	};
//};
