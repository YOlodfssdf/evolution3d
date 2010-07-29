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

struct BILLBOARD_INPUT
{
      float4 Pos      : POSITION;
      float4 Color    : COLOR;
      float2 Tex      : TEXCOORD;  
};


struct PS_INPUT
{
      float4 Pos       : POSITION0;
      float4 Color     : COLOR;
      float4 Tex       : TEXCOORD;  
      float4 wPosition : TEXCOORD2;
};


///}}

///>>Import = libMatrix

///{{SourceCode
PS_INPUT main( BILLBOARD_INPUT input )
{
      PS_INPUT output = (PS_INPUT)0;
      output.Pos   = mul( input.Pos  , TransformBuffer.matWorld  );
      output.Pos   = mul( output.Pos , TransformBuffer.matView   );
      output.Pos   = mul( output.Pos , TransformBuffer.matProject);
      output.Tex   = float4(input.Tex , 1.0f , 1.0f);
      output.Color = input.Color.xyzw;

      //��������ϵ�е�
      output.wPosition = mul( input.Pos  , TransformBuffer.matWorld );
///}}

/*
///{{Socket = VertexModifier // Socket = ����, �����ﶨ��һ�������
      Input
      {
            //�����λ�ú��з�����
            float4 input.Pos      : POSITION;

            //��������ռ�
            float4 input.Pos      : WPOSITION;

            //����任���ֵ
            float4 output.Pos     : TPOSITION;

            float4 input.Color    : COLOR;
            float4 output.Tex     : TEXCOORD;  
            
            //Camera Infomation
            float4 cameraUp       : CAMERAUP ;
            float4 cameraPos      : CAMERAPOSITION ;
            float4 cameraDir      : CAMERADIR ;
            float4 cameraArg      : CAMERAARG ;
	  
      }
      Output 
      {
            //����任���ֵ
            float4 output.Pos      : TPOSITION;
            //����任�������ռ��λ��,�з�����
            float4 output.wPosition: WPOSITION;
            //�������꣬��ɫ��
            float4 output.Color    : COLOR;
            float4 output.Tex      : TEXCOORD;   
      }
///}}
*/


///{{SourceCode 
    return output;
}
///}}