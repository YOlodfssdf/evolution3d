///{{Declaration
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
///}}



///{{SourceCode
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
///}}

/*
///{{Socket = VertexModifier // Socket = ����, �����ﶨ��һ�������
   Input
   {
       float4 input.Pos      : POSITION;
       float4 input.Nor      : NORMAL;
	   float4 input.Tan      : TANGENT;

       float4 output.Pos     : TPOSITION;
       float4 output.Nor     : TNORMAL;
	   float4 output.Tan     : TTANGENT;

       float4 input.Color    : COLOR;
	   float4 input.Weights  : BLENDWEIGHT;
	   int4   input.BoneIdxs : BLENDINDICES;
	   float2 input.Tex      : TEXCOORD;  
   }
   Output 
   {
       float4 output.Pos      : TPOSITION;
       float4 output.Nor      : TNORMAL;
	   float4 output.Tan      : TTANGENT;

       float4 output.Color    : COLOR;
	   float2 output.Tex      : TEXCOORD;   
   }
///}}
*/


///{{SourceCode        
    return output;
}
///}}