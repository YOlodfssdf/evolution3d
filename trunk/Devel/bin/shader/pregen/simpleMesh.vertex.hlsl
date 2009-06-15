
cbuffer TransformBuffer
{
   matrix matWorld;
   matrix matView;
   matrix matProject;
}

struct STATICMESHVS_INPUT
{
    float4 Pos      : POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
	float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
    float4 Nor      : NORMAL;
    float4 Color    : COLOR;
    float4 Tan      : TANGENT;
	float2 Tex      : TEXCOORD;    
};

#define VS_INPUT STATICMESHVS_INPUT


PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( matWorld   , input.Pos );
    output.Pos = mul( matView    , output.Pos);
    output.Pos = mul( matProject , output.Pos);
	output.Tex   = input.Tex;
    output.Color = input.Color.xyzw;
    output.Tan   = input.Tan;
    output.Nor   = input.Nor;


    return output;
}
