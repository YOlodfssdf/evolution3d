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
///}}

///{{DefaultCode  = ColorModify 
//Ĭ�ϵĴ��� �����Nameָ���Ĳ��û�в���Node����ô���Żᱻ�����ȥ
#define  PS_OUTPUT float4;
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
   //================================================== 
///}}

/*
///{{Socket = ColorModify // Socket = ����, �����ﶨ��һ�������
   Input
   {
       float4 yuvColor  : YUVCOLOR;
   }
   Output  class = PS_OUTPUT
   {
   }
///}}
*/

///{{DefaultCode  = ColorModify 
   //Ĭ�ϵĴ��� �����Nameָ���Ĳ��û�в���Node����ô���Żᱻ�����ȥ
   float4 RetVal = float4(yuvColor);
   return RetVal;
}
///}}

