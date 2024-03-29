//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
///{{Declaration
struct cTransformBuffer
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4 cameraUp;
	  float4 cameraPos;
	  float4 cameraDir;
	  float4 cameraArg;//[Near , Far , Fov , Aspect]
};

cTransformBuffer TransformBuffer;

sampler2D    DiffuseTexture : register(s0);
float        AlphaRef;

struct PS_INPUT
{
      float4 Pos       : POSITION0;
      float4 Color     : COLOR;
      float4 Tex       : TEXCOORD;  
      float4 wPosition : TEXCOORD2;
};

///}}

///>>Import = libDip

///{{SourceCode
float4 main( PS_INPUT input) : COLOR0
{
    float4 vDiffuse =  float4(1.0,1.0,1.0,1.0);//input.Color ;
///}}

    /*
    ///{{Default = ColorModify
    vDiffuse = float4(vDiffuse.xxx , 1.0);
    ///}}
    */

/*
///{{Socket = ColorModify // Socket = 名字, 在这里定义一个插入点
    Input
    {
          float4    vDiffuse        : COLOR0;
		  float4    input.Color     : VERTEXCOLOR;
          float4    input.Tex       : TEXCOORD;
          float4    input.Tex       : TEXCOORD0;
          float4    input.Tex       : TEXCOORD1;
         
          float4    input.wPosition : WPOSITION;

          sampler2D DiffuseTexture  : TEXTURE0;
          
          //Camera Infomation
          float4 TransformBuffer.cameraUp       : CAMERAUP ;
          float4 TransformBuffer.cameraPos      : CAMERAPOSITION ;
          float4 TransformBuffer.cameraDir      : CAMERADIR ;
          float4 TransformBuffer.cameraArg      : CAMERAARG ;
    }
    Output 
    {
          float4 vDiffuse : COLOR0;       
    }
///}}
*/

///{{SourceCode
    //vDiffuse.w = 1.0f;
    if(vDiffuse.w <= AlphaRef)
       discard;
    return float4(vDiffuse.x , vDiffuse.y , vDiffuse.z , vDiffuse.w);// clamp(vDiffuse.w , 0.0  , 1.0) );
}
///}}
