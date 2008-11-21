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
	bool operator == (const xSkeletonID& rhv)
	{
		return (m_HiWord == rhv.m_HiWord) && (m_LoWord == rhv.m_LoWord);
	}
	bool operator != (const xSkeletonID& rhv)
	{
		return (m_HiWord != rhv.m_HiWord) || (m_LoWord != rhv.m_LoWord);
	}
};

struct xMeshTexture
{
	ds_wstring   m_TexName;
	int          m_iChannel;
	HBaseTexture m_hTexture;
};

struct xMeshMaterial
{
public:
	vMeshTextures m_vTextures;
};

//���������Ķ���
struct xSkinMeshVertex
{
	xvec4           m_Position;
	xvec4           m_Normal;
	xvec4           m_Diffuse;
	xvec4           m_Tangent;
	float           m_weight[4];
	unsigned int32  m_windex[4];
	

	//==========================
	xvec2           m_uv[1];
};

//��̬Mesh�Ķ���
struct xStaticMeshVertex
{
	xvec4           m_Position;
	xvec4           m_Normal;
	xvec4           m_Diffuse;
	xvec4           m_Tangent;

	//==========================
	xvec2           m_uv[1];
};

struct xBoneTrans
{
	//Bone����תQuaternian
	xquat    m_Rotate;
	//Bone��ƽ������
	xvec3    m_Trans;
	xvec3    m_Scale;
};

struct xBoneData
{
	//��Ϻ������
	xmat4       m_Matrix;
	//Bone��ƽ������
	xBoneTrans  m_BoneTrans;
	//��Ը��ڵ�ģ�
	xmat4       m_LocaleTM;
};

struct xSkinBone
{
	//Bone��Name
	wchar_t      m_BoneName[32];
	union{
	int          m_ParentIndex;//�����������ParentIndex
	int          m_UC4AddStart;
	};

	int          m_ParentBoneID; //����ǰ�������ParentID;
	int          m_BoneID;
	//��ʼ����
	xmat4        m_InitMT;
	xmat4        m_InitMTInv;
};

typedef std::vector<xSkinBone , dllsafe_alloc<xSkinBone> > vSkinBones;

enum eActionType
{
	 eActType_Skel_Keyframe = 0,
     eActType_Skeleton      = 1,

	 eActType_Keyframe      = 2,
	 eActType_Frame         = 3,
	
};

struct xActionInfo
{
	long              m_lTime;         //��������ʱ�䳤��
	int               m_iFirstFrame;   //������һ֡
	int               m_iLastFrame;    //�������һ֡
	int               m_nFrame;
	eActionType       m_eActType;

};

END_NAMESPACE_XEVOL3D
#endif
