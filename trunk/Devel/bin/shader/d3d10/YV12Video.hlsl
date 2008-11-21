//һ��������ʾ��Shader������, PixelShader

///{{Declaration
//��Ƶ�����PS
sampler2D YTextue; 
sampler2D UTextue;
sampler2D VTextue;
float     cTime;
struct PS_INPUT 
{
   float2 Texcoord :        TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 rgbColor : COLOR0;
};
///}}

/*
///{{ParamTable //Shader�Ĳ���������
   texture   YTexture : <file="" , manager="" , width="" , height="" /> ;
   float     cTime    : TIME ;
///}}
*/

///{{SourceCode
PS_OUTPUT main( PS_INPUT Input )
{      
   //===����������=====================================
   float4 yuvColor;
   yuvColor.x       = tex2D( YTextue, Input.Texcoord ).x;
   yuvColor.y       = tex2D( UTextue, Input.Texcoord ).x;
   yuvColor.z       = tex2D( VTextue, Input.Texcoord ).x;
   yuvColor.w       = 1.0f;
   PS_OUTPUT RetVal  ;
   //================================================== 
///}}

/*
///{{Socket = ColorModify // Socket = ����, �����ﶨ��һ�������
   Input
   {
       float4 yuvColor  : YUVCOLOR;
   }
   Output var = RetVal
   {
       float4  rgbColor : COLOR0;
   }
///}}
*/

///{{DefaultCode  = ColorModify 
   //Ĭ�ϵĴ��� �����Nameָ���Ĳ��û�в���Node����ô���Żᱻ�����ȥ
   RetVal.rgbColor = float4(yuvColor,1.0);
///}}

///{{SourceCode   
   //==================================================
   return RetVal;
}
///}}