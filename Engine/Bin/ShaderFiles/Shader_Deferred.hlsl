#include "Shader_Defines.hpp"

matrix         g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix         g_ViewMatrixInv, g_ProjMatrixInv;
texture2D      g_Texture;
vector         g_vCamPosition;

vector         g_vLightDir;
vector         g_vLightPos;
float          g_fLightRange;
vector         g_vLightDiffuse;
vector         g_vLightAmbient;
vector         g_vLightSpecular;

vector         g_vMtrlAmbient = vector(0.5f, 0.5f, 0.5f, 1.f);
vector         g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D      g_NormalTexture;
texture2D      g_DiffuseTexture;
texture2D      g_ShadeTexture;
texture2D      g_SpecularTexture;
texture2D      g_DepthTexture;
texture2D      g_ShadowDepthTexture;
texture2D      g_SSAOTexture;
texture2D      g_BlurTexture;
texture2D      g_BlurXTexture;
texture2D      g_BlurYTexture;
texture2D      g_CombineBlurTexture;
texture2D      g_FinalTexture;

matrix         g_matProj;
matrix         g_matViewInv;
matrix         g_matProjInv;
matrix         g_matLightView;

//===================================================
float g_fRadius = 0.001f;
float g_fFar = 300.f;
float g_fFalloff = 0.000002f;
float g_fStrength = 0.0007f;
float g_fTotStrength = 1.38f;
float g_fInvSamples = 1.f / 16.f;

float3 g_vRandom[16] =
{
    float3(0.2024537f, 0.841204f, -0.9060141f),
    float3(-0.2200423f, 0.6282339f, -0.8275437f),
    float3(0.3677659f, 0.1086345f, -0.4466777f),
    float3(0.8775856f, 0.4617546f, -0.6427765f),
    float3(0.7867433f, -0.141479f, -0.1567597f),
    float3(0.4839356f, -0.8253108f, -0.1563844f),
    float3(0.4401554f, -0.4228428f, -0.3300118f),
    float3(0.0019193f, -0.8048455f, 0.0726584f),
    float3(-0.7578573f, -0.5583301f, 0.2347527f),
    float3(-0.4540417f, -0.252365f, 0.0694318f),
    float3(-0.0483353f, -0.2527294f, 0.5924745f),
    float3(-0.4192392f, 0.2084218f, -0.3672943f),
    float3(-0.8433938f, 0.1451271f, 0.2202872f),
    float3(-0.4037157f, -0.8263387f, 0.4698132f),
    float3(-0.6657394f, 0.6298575f, 0.6342437f),
    float3(-0.0001783f, 0.2834622f, 0.8343929f),
};

//외곽선 쉐이딩
float3x3      Kx = { -1, 0, 1,
                  -2, 0, 2,
                  -1, 0, 1 };

float3x3      Ky = { 1, 2, 1,
                  0, 0, 0,
                  -1, -2, -1 };
float2         g_Pixeloffset;

sampler ShadowDepthSampler = sampler_state
{
    texture = g_ShadowDepthTexture;
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;
};

sampler DepthSampler = sampler_state
{
    texture = g_DepthTexture;
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;
};
sampler NormalSampler = sampler_state
{
    texture = g_NormalTexture;
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;
};

sampler BlurSampler = sampler_state
{
    //texture = g_SSAOTexture;
   // texture = g_DiffuseTexture;

    texture = g_BlurTexture;
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;
   
};
sampler BlurXSampler = sampler_state
{
    //texture = g_SSAOTexture;
   // texture = g_DiffuseTexture;

    texture = g_BlurXTexture;
    filter = min_mag_mip_linear;
    AddressU = clamp;
    AddressV = clamp;

};

struct VS_IN
{
    float3      vPosition : POSITION;
    float2      vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4      vPosition : SV_POSITION;
    float2      vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT      Out = (VS_OUT)0;

    matrix      matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matrix      matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;

    return Out;
}

struct PS_IN
{
    float4      vPosition : SV_POSITION;
    float2      vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    vector      vColor : SV_TARGET0;
};


PS_OUT  PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT   Out = (PS_OUT)0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

    return Out;
}

struct PS_OUT_LIGHT
{
    vector      vShade : SV_TARGET0;
    vector      vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT      Out = (PS_OUT_LIGHT)0;

    vector      vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexUV);
    vector      vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
   

    Out.vShade = g_vLightDiffuse * (max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient));
    Out.vShade.a = 1.f;

    vector      vReflect = reflect(normalize(g_vLightDir), vNormal);

    vector      vDepth = g_DepthTexture.Sample(PointSampler, In.vTexUV);
    float      fViewZ = vDepth.x * 300.f;


    vector      vWorldPos;

    /* 투영공간상에 위치 .*/
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepth.y;
    vWorldPos.w = 1.f;

    /* 뷰스페이스상에 위치 .*/
    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    /* 월드 스페이스상에 위치 .*/
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

   

    vector      vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular.xyz = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);

    return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT      Out = (PS_OUT_LIGHT)0;

    vector      vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexUV);
    vector      vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    vector      vDepth = g_DepthTexture.Sample(PointSampler, In.vTexUV);
    float      fViewZ = vDepth.x * 300.f;
    vector      vWorldPos;

    /* 투영공간상에 위치 .*/
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepth.y;
    vWorldPos.w = 1.f;

    /* 뷰스페이스상에 위치 .*/
    vWorldPos *= fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    /* 월드 스페이스상에 위치 .*/
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);


    vector      vLightDir = vWorldPos - g_vLightPos;

    float      fDistance = length(vLightDir);


    /* 0 ~ 1 */
    float      fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);



    Out.vShade = g_vLightDiffuse * (max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
    Out.vShade.a = 1.f;

    vector      vReflect = reflect(normalize(vLightDir), vNormal);

    vector      vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular.xyz = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * fAtt;

    return Out;
}




PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
    PS_OUT         Out = (PS_OUT)0;

    vector      vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    vector      vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
    vector      vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
    vector      vDepth = g_DepthTexture.Sample(PointSampler, In.vTexUV);
    //vector      vSSAO = g_SSAOTexture.Sample(PointClampSampler, In.vTexUV);
   

    if (vDiffuse.a == 0.f)
        discard;
    //vector adjustedDiffuse = vDiffuse * vSSAO;
    //vector adjustedShade = vShade * vSSAO;

    //// 새로 계산된 결과를 적용하여 최종 결과를 만듦
    //Out.vColor = adjustedDiffuse * adjustedShade;


    if (vShade.r < 0.21f)
        vShade.rgb = float3(0.2f, 0.2f, 0.2f);
    else if (vShade.r >= 0.21f && vShade.r < 0.41f)
        vShade.rgb = float3(0.4f, 0.4f, 0.4f);
    else if (vShade.r >= 0.41f && vShade.r <= 1.f)
        vShade.rgb = float3(0.7f, 0.7f, 0.7f);
    
    Out.vColor = (vDiffuse) * (vShade);
    /*if (Out.vColor.a == 0.f)
        discard;*/
   
    //그림자 적용

    vector      vDepthInfo = g_DepthTexture.Sample(DepthSampler, In.vTexUV);
    float      fViewZ = vDepthInfo.x * 300.0f;

    vector      vPosition;

    vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
    vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
    vPosition.z = vDepthInfo.y * fViewZ;
    vPosition.w = fViewZ;

    vPosition = mul(vPosition, g_matProjInv);
       
    vPosition = mul(vPosition, g_matViewInv);
    
    vPosition = mul(vPosition, g_matLightView);
    
    vector      vUVPos = mul(vPosition, g_matProj);
    
    float2      vNewUV;

    vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
    vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

   

    vector      vShadowDepthInfo = g_ShadowDepthTexture.Sample(ShadowDepthSampler, vNewUV);

    if (vPosition.z - 0.1f > vShadowDepthInfo.r * 300.0f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 1.f);
        Out.vColor *= vColor;
    }

    if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.1f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.1f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.2f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.2f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.3f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.3f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.4f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.4f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.5f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.5f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.6f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.6f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.7f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.7f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.8f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.8f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 0.9f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 0.9f);
        Out.vColor *= vColor;
    }
    else if (vPosition.z > vShadowDepthInfo.r * 300.0f + 1.f)
    {
        vector vColor = vector(0.7f, 0.7f, 0.7f, 1.f);
        Out.vColor *= vColor;
    }
        

    return Out;
}



PS_OUT PS_MAIN_DEFERRED_Test(PS_IN In)
{
    PS_OUT         Out = (PS_OUT)0;

    vector      vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    vector      vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
    vector      vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);

    Out.vColor = vDiffuse * vShade + vSpecular;

    if (0.f == Out.vColor.a)
        discard;

    return Out;
}
//==============================Blur======================================
float m_TexW = 1280.f;
float m_TexH = 720.f;

static const float Weight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
    1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
     /* 0.01, 0.03, 0.08, 0.15, 0.3, 0.6,
    1,    0.6,  0.3, 0.15, 0.08, 0.03, 0.01*/
};
static const float Total = 6.2108;
//static const float Total = 5;


PS_OUT PS_BlurX(PS_IN _In)
{
    PS_OUT         Out = (PS_OUT)0;

    float2	t = _In.vTexUV;
    float2	uv = 0;

    float	tu = 1.f / m_TexW;

    for (int i = -6; i < 6; ++i)
    {
        uv = t + float2(tu * i, 0);
        Out.vColor += Weight[6 + i] * g_BlurTexture.Sample(BlurSampler, uv);
    }

    Out.vColor /= Total;
    if (Out.vColor.a == 0.f)
        discard;
    if (Out.vColor.a == 1.f)
        discard;
    if (Out.vColor.r == float(1.f) && Out.vColor.g == float(1.f) && Out.vColor.b == float(1.f))
        discard;
    if (Out.vColor.r == float(0.f) && Out.vColor.g == float(0.f) && Out.vColor.b == float(0.f))
        discard;

    return Out;
}

PS_OUT PS_BlurY(PS_IN _In)
{
    PS_OUT         Out = (PS_OUT)0;

    float2 t = _In.vTexUV;
    float2 uv = 0;

    float tv = 1.f / (m_TexH /*/ 2.f*/);

    for (int i = -6; i < 6; ++i)
    {
        uv = t + float2(0, tv * i);
        Out.vColor += Weight[6 + i] * g_BlurTexture.Sample(BlurSampler, uv);
    }

    Out.vColor /= Total;
   
    if (Out.vColor.a == 0.f)
        discard;
    if (Out.vColor.a == 1.f)
        discard;
    if (Out.vColor.r == float(1.f) && Out.vColor.g == float(1.f) && Out.vColor.b == float(1.f))
        discard;
    if (Out.vColor.r == float(0.f) && Out.vColor.g == float(0.f) && Out.vColor.b == float(0.f))
        discard;

    return Out;
}

PS_OUT PS_Combine_Blur(PS_IN In)
{
    PS_OUT      Out = (PS_OUT)0;

    vector      vFinal = g_Texture.Sample(LinearSampler, In.vTexUV);
    vector      vBlurX = g_BlurXTexture.Sample(LinearSampler, In.vTexUV);
    vector      vBlurY = g_BlurYTexture.Sample(LinearSampler, In.vTexUV);
        
    vector      vSSAO = g_SSAOTexture.Sample(LinearSampler, In.vTexUV);
    if (vFinal.a == 0.f)
        discard;
   vFinal *= vSSAO.r;
   Out.vColor = ((vFinal + vBlurX + vBlurY) / 3.f);
 
    if (Out.vColor.a == 0.f)
        discard;
    /*if (Out.vColor.a == 1.f)
        discard;*/
   /* if (Out.vColor.r == float(1.f) && Out.vColor.g == float(1.f) && Out.vColor.b == float(1.f))
        discard;
    if (Out.vColor.r == float(0.f) && Out.vColor.g == float(0.f) && Out.vColor.b == float(0.f))
        discard;*/

    return Out;
}
PS_OUT PS_Combine_SSAOBlur(PS_IN In)
{
    PS_OUT      Out = (PS_OUT)0;

    vector      vFinal = g_Texture.Sample(LinearSampler, In.vTexUV);
    vector      vBlurX = g_BlurXTexture.Sample(LinearSampler, In.vTexUV);
    vector      vBlurY = g_BlurYTexture.Sample(LinearSampler, In.vTexUV);

   
    if (vFinal.a == 0.f)
        discard;
    //vFinal *= vSSAO.r;
    Out.vColor = ((vFinal + vBlurX + vBlurY) / 3.f);

    if (Out.vColor.a == 0.f)
        discard;
    /*if (Out.vColor.a == 1.f)
        discard;*/
        /* if (Out.vColor.r == float(1.f) && Out.vColor.g == float(1.f) && Out.vColor.b == float(1.f))
             discard;
         if (Out.vColor.r == float(0.f) && Out.vColor.g == float(0.f) && Out.vColor.b == float(0.f))
             discard;*/

    return Out;
}

//==============================SSAO======================================

float3 randomNormal(float2 tex)
{
    float noiseX = (frac(sin(dot(tex, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
    float noiseY = (frac(sin(dot(tex, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
    float noiseZ = (frac(sin(dot(tex, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
    return normalize(float3(noiseX, noiseY, noiseZ));
}

PS_OUT PS_SSAO_Test(PS_IN _In)
{
    PS_OUT         Out = (PS_OUT)0;

    //float4 vDepth = g_DepthTexture.Sample(LinearClampSampler, _In.vTexUV);
    //float4 vNormal = g_NormalTexture.Sample(PointClampSampler, _In.vTexUV);

    float4      vNormalDesc = g_NormalTexture.Sample(NormalSampler, _In.vTexUV);
    float4      vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

     float4 vDepth = g_DepthTexture.Sample(DepthSampler, _In.vTexUV);

    
   
     //vNormal = float4(vNormal.xyz * 2.f - 1.f, 0.f);
    if (vNormal.a != 0.f)
    {
        Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
        return Out;
    }
    vNormal = normalize(vNormal);

    float fViewZ = vDepth.r * g_fFar;
    half3 vHNormal = vNormal.rgb;

    vector      vPosition;

    vPosition.x = (_In.vTexUV.x * 2.f - 1.f) * fViewZ;
    vPosition.y = (_In.vTexUV.y * -2.f + 1.f) * fViewZ;
    vPosition.z = vDepth.y * fViewZ;
    vPosition.w = fViewZ;

    vPosition = mul(vPosition, g_matProjInv);
    vPosition = mul(vPosition, g_matViewInv);

    //vector      vUVPos = mul(vPosition, g_matProj);

    float2      vNewUV;

    vNewUV.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f;
    vNewUV.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f;
    
    float fDepth = vDepth.g * g_fFar * fViewZ;

    half3 vRay;
    half3 vReflect;
    half2 vRandomUV;
    float fOccNorm;
    float4 vRandomDepth;

    int iColor = 0;

    for (int i = 0; i < 16; ++i)
    {

        vRay = reflect(randomNormal(vNewUV), g_vRandom[i]);
        vReflect = normalize(reflect(vRay, vHNormal)) * g_fRadius;
        vReflect.x *= -1.f;
        vRandomUV = _In.vTexUV + vReflect.xy;
        vRandomDepth = g_DepthTexture.Sample(DepthSampler, vRandomUV);
        fOccNorm = vRandomDepth.g * g_fFar * fViewZ;

        if (fOccNorm <= fDepth + 0.0003f)
            ++iColor;
    }

    vector vAmbient = abs((iColor / 16.f) - 1);

    Out.vColor = (1.f - vAmbient);

    if (Out.vColor.a == 0.f)
        discard;
   /* if (Out.vColor.r == 0.f && Out.vColor.g == 0.f && Out.vColor.b == 0.f)
        discard;*/
    

    return Out;
}


technique11 DefaultTechnique
{
    pass Debug
    {//0
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_Default, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass Light_Diretional
    {//1
        SetRasterizerState(RS_Default);
        SetBlendState(BS_OneByOne_Engine, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Light_Point
    {//2
        SetRasterizerState(RS_Default);
        SetBlendState(BS_OneByOne_Engine, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }

    pass Deferred_Blend // 디퍼드
    {//3
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
    }

    pass Deferred_Test
    {//4
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_Test();
    }

    pass SSAO_Test
    {//5
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO_Test();
    }

    pass BlurX
    {//6
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BlurX();
    }

    pass BlurY
    {//7
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BlurY();
    }

    pass CombineBlur
    {//8
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Combine_Blur();
    }

    pass CombineSSAOBlur
    {//8
        SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DS_None_ZEnable_None_ZWrite, 0);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Combine_SSAOBlur();
    }
}