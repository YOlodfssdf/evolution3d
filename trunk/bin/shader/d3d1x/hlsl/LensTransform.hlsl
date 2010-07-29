///{{Declaration
cbuffer TransformBuffer
{
      float4x4 matWorld;
      float4x4 matView;
      float4x4 matProject;
      float4x4 matTexture;
	  float4   cameraUp;
	  float4   cameraPos;
	  float4   cameraDir;
	  float4   cameraArg;//[Near , Far , Fov , Aspect]
}
cbuffer LensArg
{
      float4 m_fPt;
      float4 m_fLen;
      float4 m_rLen;
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
      float4 Tex      : TEXCOORD;  

      float4 wPosition : TEXCOORD2;
      float4 wNormal   : TEXCOORD3;  
      float4 wTangent  : TEXCOORD4;
};

#define VS_INPUT STATICMESHVS_INPUT
///}}



///{{SourceCode
PS_INPUT main( VS_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( matWorld   , input.Pos );
      output.Pos   = mul( matView    , output.Pos);
      output.Pos   = mul( matProject , output.Pos);
      output.Tex   = input.Tex;
      output.Color = input.Color.xyzw;
      output.Tan   = input.Tan;
      output.Nor   = input.Nor;

      output.wPosition = input.Pos; 
      output.wNormal   = input.Nor;
      output.wTangent  = input.Tan;
///}}

/*
///{{Socket = VertexModifier // Socket = ����, �����ﶨ��һ�������
      Input
      {
            //�����λ�ú��з�����
            float4 input.Pos      : POSITION;
            float4 input.Nor      : NORMAL;
            float4 input.Tan      : TANGENT;

            //��������ռ�
            float4 input.Pos      : WPOSITION;
            float4 input.Nor      : WNORMAL;
            float4 input.Tan      : WTANGENT;

            //����任���ֵ
            float4 output.Pos     : TPOSITION;
            float4 output.Nor     : TNORMAL;
            float4 output.Tan     : TTANGENT;

            float4 input.Color    : COLOR;
            float4 input.Weights  : BLENDWEIGHT;
            int4   input.BoneIdxs : BLENDINDICES;
            float4 input.Tex      : TEXCOORD;  
      }
      Output 
      {
            //����任���ֵ
            float4 output.Pos      : TPOSITION;
            float4 output.Nor      : TNORMAL;
            float4 output.Tan      : TTANGENT;

            //����任�������ռ��λ��,�з�����
            float4 output.wPosition: WPOSITION;
            float4 output.wNormal  : WNORMAL;
            float4 output.wTangent : WTANGENT;

            //�������꣬��ɫ��
            float4 output.Color    : COLOR;
            float4 output.Tex      : TEXCOORD;   
      }
///}}
*/

   
   
   //͸���任
   float x = input.Pos.x;
   float y = input.Pos.y;
   float z = input.Pos.z;
   
   float x0 = LensArg.m_fPt.x;
   float y0 = LensArg.m_fPt.y;
   float z0 = LensArg.m_fPt.z;
   
   //t = Z0 /(Z0 - Z);
   float t  = z0 / (z0 - z);
   
   //����õ㷢���Ĺ�����
   output.x = x0 + t * ( x - x0);
   output.y = y0 + t * ( y - y0);
   
   //ͶӰ������
   output.x = output.x/LensArg.m_m_rLen.x; 
   output.y = output.y/LensArg.m_m_rLen.y;
   
   output.z = z;
   output.w = 1.0;
   
///{{SourceCode 
    return output;
}
///}}