
/*Address Mode
 WRAP -> 텍스처를 반복 연속적으로 나타냄 0.0, 1.0 텍스처를 한바퀴 돌아서 동일한 지점을 가르킴
 MIRROR -> 텍스처를 반복하지만 ,반복되는 구간에서 좌표의 진행 방향이 번갈아가면서 뒤집힘
 CLAMP -> 텍스처 좌표의 경계에 도달하면 경계 값으로 고정
 BORDER  -> 텍스처 좌표를 텍스처의 경계에 도달하면 사용자가 지정한 경계 색상으로 텍스처를 채움
 MIRROR_ONCE -> 텍스처 반복하지만 좌표 0,1 범위를 벗어나면 MIRRO와 유사하게 뒤집힘  -> 이 반복은 한번만 일어남
*/
sampler LinearSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

sampler PointSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;

	AddressU = WRAP;
	AddressV = WRAP;
};

sampler LinearClampSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler PointClampSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


/* For.RasterizerState */
RasterizerState		RS_Default
{
	FillMode = Solid;
	CullMode = BACK;
	FrontCounterClockwise = false;
};
RasterizerState		RS_Default2
{
	FillMode = Solid;
	CullMode = BACK;
	FrontCounterClockwise = true;
};

RasterizerState		RS_Wireframe
{
	FillMode = Wireframe;
	CullMode = BACK;
	FrontCounterClockwise = false;
};

RasterizerState		RS_None
{
	FillMode = Solid;
	CullMode = NONE;
	FrontCounterClockwise = false;
};


RasterizerState		RS_CULL_CW
{
	FillMode = Solid;
	CullMode = FRONT;
	FrontCounterClockwise = false;
};

BlendState	 BS_Default
{
	/* [0] : 0번째 렌더타겟에 그릴때 블렌딩을 할꺼다 말꺼다.*/
	BlendEnable[0] = false;
};

BlendState	 BS_AlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = Add;
};

BlendState	 BS_OneByOne
{
	BlendEnable[0] = true;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = Add;
};


BlendState    BS_OneByOne_Engine
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = Add;
};

DepthStencilState DS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DS_None_ZEnable
{
	DepthEnable = true;
	DepthWriteMask = zero;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DS_None_ZEnable_None_ZWrite
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

DepthStencilState DS_None_Test
{
	DepthEnable = false;
	DepthWriteMask = zero;
	DepthFunc = LESS_EQUAL;
};


