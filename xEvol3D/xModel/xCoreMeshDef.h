#ifndef __XBASE_COREMESH_DEFINE_H__
#define __XBASE_COREMESH_DEFINE_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "../RenderAPI/xTextureManager.h"
#include "../xcomdoc/xcomdoc.h"
#include "../BaseLib/xarray.h"
BEGIN_NAMESPACE_XEVOL3D
struct  xSkeletonID;
struct  xMeshTexture;
struct  xMeshMaterial;
typedef std::vector<xMeshTexture , dllsafe_alloc<xMeshTexture>  > vMeshTextures; 
typedef std::vector<xMeshMaterial, dllsafe_alloc<xMeshMaterial> > vMeshMaterials; 
struct xSkeletonID
{
	int32 m_HiWord;
	int32 m_LoWord;
public:
	bool operator == (const xSkeletonID& rhv) const 
	{
		return (m_HiWord == rhv.m_HiWord) && (m_LoWord == rhv.m_LoWord);
	}
	bool operator != (const xSkeletonID& rhv) const
	{
		return (m_HiWord != rhv.m_HiWord) || (m_LoWord != rhv.m_LoWord);
	}
};

struct xMeshTexture
{
	ds_wstring   m_TexName;
	int32        m_iChannel;
	HBaseTexture m_hTexture;
};

struct xMeshMaterial
{
public:
    xMeshMaterial()
	{
		m_Ambient  = xColor_4f(0.0f , 0.0f , 0.0f , 1.0f);
		m_Diffuse  = xColor_4f(1.0f , 1.0f , 1.0f , 1.0f);
		m_Speculer = xColor_4f(1.0f , 1.0f , 1.0f , 1.0f);
		m_Emissive = xColor_4f(0.0f , 0.0f , 0.0f , 1.0f);
		m_fShiness = 1.0f;
		m_fOpactiy = 1.0f;
	}
	xColor_4f        m_Ambient ;
	xColor_4f        m_Diffuse ;
	xColor_4f        m_Speculer ;
	xColor_4f        m_Emissive;
	std::ds_wstring  m_Script;
	float            m_fShiness;
	float            m_fOpactiy;
	vMeshTextures    m_vTextures;
};

//���������Ķ���
struct xSkinMeshVertex
{
	xMathLib::xvec4 m_Position;
	xMathLib::xvec4 m_Normal;
	xMathLib::xvec4 m_Diffuse;
	xMathLib::xvec4 m_Tangent;
	float           m_weight[4];
	unsigned short  m_windex[4];
	

	//==========================
	xMathLib::xvec2 m_uv[1];
};

//��̬Mesh�Ķ���
struct xStaticMeshVertex
{
	xMathLib::xvec4 m_Position;
	xMathLib::xvec4 m_Normal;
	xMathLib::xvec4 m_Diffuse;
	xMathLib::xvec4 m_Tangent;

	//==========================
	xMathLib::xvec2 m_uv[1];
};

struct xBoneTrans
{
	//Bone����תQuaternian
	xMathLib::xquat    m_Rotate;
	//Bone��ƽ������
	xMathLib::xvec3    m_Trans;
	xMathLib::xvec3    m_Scale;
public:
    void toMatrix(xMathLib::xmat4& mat);
};

struct xBoneData
{
	//��Ϻ������
	xMathLib::xmat4       m_Matrix;
	//Bone��ƽ������
	xBoneTrans            m_BoneTrans;
	//��Ը��ڵ�ģ�
	xMathLib::xmat4       m_LocaleTM;

public:
    void fromBoneTrans(xBoneTrans& trans ,const xMathLib::xmat4& wsMat , const xMathLib::xmat4& InitTMInv)
    {
        m_BoneTrans = trans;
        trans.toMatrix(m_LocaleTM);
        xMathLib::XM_Mul(InitTMInv , wsMat , m_Matrix);
    }
};

struct xSkinBone
{
	//Bone��Name
	wchar_t          m_BoneName[32];
	union{
	int32            m_ParentIndex;//�����������ParentIndex
	int32            m_UC4AddStart;
	};

	int32            m_ParentBoneID; //����ǰ�������ParentID;
	int32            m_BoneID;
	//��ʼ����
	xMathLib::xmat4  m_InitMT;
	xMathLib::xmat4  m_InitMTInv;
};

typedef std::vector<xSkinBone , dllsafe_alloc<xSkinBone> > vSkinBones;

enum eActionType
{
     eActType_None          = 0, 
     eActType_Skeleton      = 1,
	 eActType_Keyframe      = 2,
	 eActType_Frame         = 3,
	 eActType_Blend         = 4,//���������ں�(���������İ������ں�)
	 eActType_Transition    = 5,//������������
	 eActType_Mix           = 6,//��������Ļ��
     eActType_Freeze        = 8,//��һ������������
};

struct xActionInfo
{
	int32             m_lTime;         //��������ʱ�䳤��
	int32             m_iFirstFrame;   //������һ֡
	int32             m_iLastFrame;    //�������һ֡
	int32             m_nFrame;
	eActionType       m_eActType;

};

END_NAMESPACE_XEVOL3D
#endif
