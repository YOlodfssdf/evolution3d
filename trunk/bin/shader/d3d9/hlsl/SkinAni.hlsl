///{{Slot = VM_DoSkin
Input 
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Weights  : BLENDWEIGHT;
      int4   BoneIdxs : BLENDINDICES;
}

Output class=VM_SkinRet
{
      float4 Pos: TPOSITION;
      float4 Nor: TNORMAL;
      float4 Tan: TTANGENT;

      float4 wPos : WPOSITION;
      float4 wNor : WNORMAL;
      float4 wTan : WTANGENT;
}
///}}


///>>Import = libMatrix

///{{Declaration
#define _CBUFFER_ANI_
struct SKINMESHVS_INPUT
{
      float4 Pos      : POSITION;
      float4 Nor      : NORMAL;
      float4 Color    : COLOR;
      float4 Tan      : TANGENT;
      float4 Weights  : BLENDWEIGHT;
      int4   BoneIdxs : BLENDINDICES;
      float2 Tex      : TEXCOORD;  
};

struct VM_SkinRet
{
      float4 Pos;
      float4 Tan;
      float4 Nor;
      float4 wPos;
      float4 wNor;
      float4 wTan;	
};


struct cBoneFrame
{       
      float4 matBone[72 * 3];
};

cBoneFrame BoneFrame;
VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs);
#undef VS_INPUT
#define VS_INPUT SKINMESHVS_INPUT
///}}


///{{SourceCode


VM_SkinRet VM_DoSkin(float4 Pos , float4 Nor , float4 Tan , float4 Weights , int4   BoneIdxs)
{
      VM_SkinRet ret;
      //Ŀǰ��δ��뻹û��
      float vWeight[4];
      vWeight[0] = Weights.x;
      vWeight[1] = Weights.y;
      vWeight[2] = Weights.z;
      vWeight[3] = Weights.w;

      int vBoneIdxs[4];
      vBoneIdxs[0] = BoneIdxs.x;
      vBoneIdxs[1] = BoneIdxs.y;
      vBoneIdxs[2] = BoneIdxs.z;
      vBoneIdxs[3] = BoneIdxs.w;


      float4 vPos = float4(0.0f,0.0f,0.0f,0.0f);
      float4 vNor = float4(0.0f,0.0f,0.0f,0.0f);
      float4 vTan = float4(0.0f,0.0f,0.0f,0.0f);

      for(int i = 0 ; i < 4 ; i ++ )
      {
            int boneIdx = vBoneIdxs[i];

            float4x4  matPosT;
            matPosT[0] = BoneFrame.matBone[ boneIdx * 3 + 0  ];
            matPosT[1] = BoneFrame.matBone[ boneIdx * 3 + 1  ];
            matPosT[2] = BoneFrame.matBone[ boneIdx * 3 + 2  ];

            matPosT[3] = float4(0.0 , 0.0 , 0.0 , 1.0);

            float4x4  matPos   = transpose(matPosT);

            float4x4 matNor = normalMatrix(matPos);
            vPos += vWeight[i] * mul(  Pos  ,  matPos  );
            vNor += vWeight[i] * mul(  Nor  ,  matNor  );
            vTan += vWeight[i] * mul(  Tan  ,  matNor  );
      }
      
      //��δ�����ʱ�����˹��������Ĺ���
      //vPos = Pos;
      //vNor = Nor;
      //vTan = Tan;

      vPos.w = 1.0f;
      //�ȱ�������ռ�ķ���ֵ
      ret.wPos = vPos;
      ret.wNor = vNor;
      ret.wTan = vTan;

      //����任���λ��
      vPos    = mul(vPos  , TransformBuffer.matWorld  );
      vPos    = mul(vPos  , TransformBuffer.matView   );
      ret.Pos = mul(vPos  , TransformBuffer.matProject);


      //��������ռ��λ�úͷ�/������
      float4x4 matWordNormal = normalMatrix(TransformBuffer.matWorld);
      ret.wPos = mul( ret.wPos , TransformBuffer.matWorld );
      ret.wNor = mul( ret.wNor , matWordNormal            );
      ret.wTan = mul( ret.wTan , matWordNormal            );

      //����۲�ռ��λ�úͷ�/������
      float4x4 matNor = normalMatrix(TransformBuffer.matView);
      ret.Nor = mul( ret.wNor , matNor ); 
      ret.Tan = mul( ret.wTan , matNor );

      return ret;
}
///}}

