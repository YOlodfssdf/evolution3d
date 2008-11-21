//===============================================
//Begin import lib
//Import lib name=libDip

float4 dip_filter(float3x3 _filter , SamplerState _sampler , Texture2D _texture , float2 _xy, float2 texSize)
{
     float2 _filter_pos_delta[3][3] = 
     {
        { float2(-1.0 , -1.0) , float2(0,-1.0), float2(1.0 , -1.0) },
        { float2( 0.0 , -1.0) , float2(0, 0.0), float2(1.0 ,  0.0) },
        { float2( 1.0 , -1.0) , float2(0, 1.0), float2(1.0 ,  1.0) },
     };
    
     float4 final_color = float4(0.0,0.0,0.0,0.0); 
     for(int i = 0 ; i < 3 ; i ++ )
     {
         for(int j = 0 ; j < 3 ; j ++)
         {
              float2 _xy_new = float2(_xy.x + _filter_pos_delta[i][j].x , _xy.y + _filter_pos_delta[i][j].y);
              float2 _uv_new = float2(_xy_new.x/texSize.x , _xy_new.y/texSize.y);
              final_color += _texture.Sample( _sampler , _uv_new ) * _filter[i][j];
         } 
     }
     return final_color;
}

//HDR Midle-Gray Exposure re-assign
float4  xposure(float4 _color , float gray , float ex)
{
     float b = ( 4 * ex -  1 );
     float a = 1 - b;
     float f = gray * ( a * gray + b );
     
     return   f * _color;
}

//Finish import lib
//============================================


//Insert Node Declaration Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\d3d10\simple2D.pixel.hlsl

SamplerState DefualtSampler : register(s0);
Texture2D    DiffuseTexture : register(t0);
struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
    float4 Tex      : TEXCOORD0;
    float4 Tex1     : TEXCOORD1;
    float4 Color    : COLOR;
   
};


//Insert Node Declaration Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\lib\simple.mask.hlsl

Texture2D    Texture1       : register(t1);;


//Insert Node Declaration Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\lib\RGBAGray.hlsl

struct CM_GrayRet
{
	float4 color;
};

CM_GrayRet CM_Gray(float4 ColorIn);

//==================================================

//============================================

//Insert Node Code Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\lib\simple.texture.hlsl

float4 COMMON_Texture(float4 _Color , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
     return _texture.Sample(_sampler, texCoord.xy) * _Color ;
}


//Insert Node Code Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\lib\simple.mask.hlsl

float4 CM_Mask(float4 vDiffuse , float2 texCoord , SamplerState _sampler)
{
   float4 maskColor = Texture1.Sample(_sampler, texCoord.xy);
   float4 ret = vDiffuse * maskColor;
   return ret;
}


//Insert Node Code Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\lib\simple.fakehdr.hlsl






float4 CM_FakeHDR(float4 vDiffuse , float2 texCoord , SamplerState _sampler , Texture2D _texture )
{
   float2    TexSize = float2(32 , 32);
   float2    intXY   = float2(texCoord.x * TexSize.x , texCoord.y * TexSize.y);
   
   float3x3 _smooth_fil = float3x3 (1/16.0 ,2/16.0,1/16.0 ,
                                    2/16.0 ,4/16.0,2/16.0 ,
                                    1/16.0 ,2/16.0,1/16.0 );
          
   float  _exposure  = 1.6;   
   float4 _dsColor   = dip_filter(_smooth_fil , _sampler , _texture , intXY, TexSize);
   float  _lum       = 0.3 * _dsColor.x + 0.59 * _dsColor.y + 0.11 * _dsColor.z;
   return float4(xposure(vDiffuse , _lum , _exposure ).xyz , vDiffuse.w);    
}



//Insert Node Code Node= P:\Projects\SVN\Evol3D\Devel\bin\shader\lib\RGBAGray.hlsl

CM_GrayRet CM_Gray(float4 ColorIn)
{
   CM_GrayRet ret;
   float gray = 0.3 * ColorIn.x + 0.59 * ColorIn.y + 0.11 * ColorIn.z;
   ret.color = float4(gray,gray,gray,ColorIn.w);
   return ret;
}

//============================================


//============================================
//Begin Main Node'code 

float4 main( PS_INPUT input) : SV_Target
{
    float4 vDiffuse =  input.Color ;


   //Ӧ���޸��� name=simple.texture 
   float4 Node_0_Ret = COMMON_Texture(vDiffuse , input.Tex , DefualtSampler , DiffuseTexture);

   //Ӧ���޸��� name=simple.mask 
   float4 Node_1_Ret = CM_Mask(Node_0_Ret , input.Tex1 , DefualtSampler);

   //Ӧ���޸��� name=simple.fakehdr 
   float4 Node_2_Ret = CM_FakeHDR(Node_1_Ret , input.Tex , DefualtSampler , DiffuseTexture);

   //Ӧ���޸��� name=RGBAGray 
   CM_GrayRet Node_3_Ret = CM_Gray(Node_2_Ret);

   vDiffuse = Node_3_Ret.color; 

   return vDiffuse;
}

