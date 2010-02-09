//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
struct PS_INPUT
{
      float4 Pos      : SV_POSITION;
      float4 Color    : COLOR;
};
///}}

///{{SourceCode
float4 main( PS_INPUT input) : SV_Target
{
      float4 vDiffuse =  input.Color ;
///}}

    /*
///{{Default = ColorModify
    vDiffuse = float4(vDiffuse.xxx , 1.0);
///}}
    */

/*
///{{Socket = ColorModify // Socket = ����, �����ﶨ��һ�������
   Input
   {
       float4    vDiffuse        : COLOR0;
       float4    input.Tex       : TEXCOORD;
       float4    input.Tex       : TEXCOORD0;
       float4    input.Tex1      : TEXCOORD1;
       Sampler   DefaultSampler  : SAMPLER;
       Texture2D DiffuseTexture  : TEXTURE0;
   }
   Output 
   {
      float4 vDiffuse : COLOR0;       
   }
///}}
*/

///{{SourceCode
     return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
