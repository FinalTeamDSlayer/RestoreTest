
matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector			g_vColor;

struct VS_IN
{
	float3		vPosition : POSITION;	
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;	
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT		Out = (VS_OUT)0;	

	matrix		matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matrix		matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;	
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;	
};


PS_OUT  PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;
	
	Out.vColor = g_vColor;

	return Out;		
}

technique11 DefaultTechnique
{		
	pass General
	{		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}
