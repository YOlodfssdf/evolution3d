/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __MAX_NODE_MESH_H__
#define __MAX_NODE_MESH_H__
#include "MaxEnv.h"
#include "Math.h"
#include "DataDefine.h"
using namespace XEvol3DModel;
#include <vector>
#include <string>
#include <hash_map>
using namespace std;
using namespace stdext;
struct sMaxActionInfo_t
{
	sActionInfo_t      m_BaseInfo;
	bool               m_bTexCoordAni;
	bool               m_bPositionAni;
};



//���һ��Mesh��Ҫ������Щ���ݵĽṹ��
struct sMeshDataFlag
{
	bool        m_isNormal;
	bool        m_isDiffuse;
	bool        m_isCalcTangentSpace;
	int         m_UVChannels;

	//���ŵı���
	float       m_fScale;

	//������������Ҫshell-furЧ���Ļ�����Ҫ��Щ��Ϣ
	MeshFurInfo_t m_FurInfo;
	float       m_fColorScale[4];
	bool        m_bUseBkColor;
	void _default()
	{
		m_isNormal = true;
		m_isDiffuse = true;
		m_isCalcTangentSpace = false;
		m_UVChannels = 1;
		m_FurInfo.m_furShellHeight = 0.0;
		m_FurInfo.m_furLevel  = 0;
		m_FurInfo.m_isFur = true;
		m_FurInfo.m_SkinUTile = 3.0;
		m_FurInfo.m_SkinVTile = 3.0;
		m_fScale  = 1.0f;
		m_fColorScale[0] = m_fColorScale[1] = m_fColorScale[2] = m_fColorScale[3];
	}
};

//��TexLayer��һ����װ��
//�ڵ�����������ҪһЩ��������ݣ�������ͼ�ı任����
struct sMaxTexLayer
{
	sModelTexLayer m_MTL;
	Matrix3        m_UVTransMat;

};

typedef std::vector<sMaxTexLayer> sMaxTexLayers_t;

//��Material��һ����װ��
//������һЩ�������Ϣ
struct sMaxMatrial_t
{
	sMaxTexLayers_t      m_TexLayers;
	int                  m_nTexLayer;
	sMateril_t           m_BaseMat;
	int                  m_matID;
	Mtl*                 m_Mtl;
};

//��Max���Ȱ�Noramlȡ��һ��������
struct NFace
{
	int normal[3];
};

struct MaxNormal
{
	int    smgroup;
	int    iVertex;
	int    iFace;
	bool   bSpecified;
	Point3 vNormal;
};

//��Max��ȡ���ݵ�ʱ���õ��������Ϣ��
//������������е�����Ҫ������
struct sMaxFace_t
{
	int vert[3];
	int uv[3];
	int color[3];
	int normal[3];
	int matID;
	int faceIndex;
};

//һ��VertexIndex����Ҫ������Mesh��ȡ������
//�潨����ɺ�����VertexIndex�б�
struct sVeretIndex_t
{
	int m_iPos;
	int m_iAlpha;
	int m_iDiffuse;
	int m_nUV;
	int m_UVs[12];
	int m_iNormal;

	bool operator == (const sVeretIndex_t& rhv)
	{
		if(m_iPos != rhv.m_iPos)
			return false;
		if(m_iDiffuse != rhv.m_iDiffuse)
			return false;
		if(m_iNormal != rhv.m_iNormal)
			return false;
		for(int i = 0 ; i < m_nUV ; ++i)
		{
			if(m_UVs[i] != rhv.m_UVs[i])
				return false;
		}
		return true;
	}
};

#define COMPARE_COMPENT_LESS(a , b ) if(a < b) { return true; } if(a > b) {return false;} 


inline bool operator < (const sVeretIndex_t& lhv , const sVeretIndex_t& rhv)
{
	COMPARE_COMPENT_LESS(lhv.m_iPos     , rhv.m_iPos);
	COMPARE_COMPENT_LESS(lhv.m_iDiffuse , rhv.m_iDiffuse);
	COMPARE_COMPENT_LESS(lhv.m_iNormal  , rhv.m_iNormal);
	for(int i = 0 ; i < lhv.m_nUV ; ++i)
	{
		COMPARE_COMPENT_LESS(lhv.m_UVs[i] , rhv.m_UVs[i]);
	}
	return false;
}

class CSkeletonExporter;
class MorphR3;

typedef std::vector<sWeightedVertex_t> sWeightedVertexs_t;
typedef std::vector<sVeretIndex_t>     VertexIndexs_t;
typedef std::map<sVeretIndex_t , int > VertexIndexSet_t;
typedef std::vector<sMaxMatrial_t>     Materials_t;
typedef std::vector<Mtl*>              StdMtls_t;
typedef std::vector<int>               UsedMapChannels_t;
typedef sFace_t*                  IndexBuffer_t;
typedef std::vector<IndexBuffer_t>     IndexBuffers_t;
typedef std::vector<NFace>             NormalFaces_t;
typedef std::vector<MaxNormal>         MaxNormals_t;
typedef std::map<MaxNormal , int>      MaxNormalSet_t;
/*�͵���������صļ��ϣ���Щ���϶�����Ҫ�ͷŵ�*/
struct sVertexData_t 
{
	int                    m_nUVChannes;
	sVectors_t             m_Positons;
	sWeightedVertexs_t     m_VertexWeights;//���ع���VertexWeights;
	sColors_t              m_Diffuses;        
	sUVCoords_t            m_UVChannels[12];
	
	sVectors_t             m_Tangents;//��������Tangent space
	sVectors_t             m_Normals;
	sVectors_t             m_VNormals;//��������ǵ�VertexNormal;

	void  reserve(int nVert , int nUV = 12)
	{
		m_Positons.reserve(nVert);
		m_Diffuses.reserve(nVert);
		this->m_nUVChannes = nUV;
		for(int i = 0 ; i < nUV ; ++i)
			m_UVChannels[i].reserve(nVert);

		m_VertexWeights.reserve(nVert);
		m_Tangents.reserve(nVert); //��������������
		m_Normals.reserve(nVert);
		m_VNormals.reserve(nVert);
	}

	void clear()
	{
		m_Positons.clear();
		m_Diffuses.clear();
		m_nUVChannes = 0;
		for(int i = 0 ; i < 12 ; ++i)
			m_UVChannels[i].clear();

		m_VertexWeights.clear();
		m_Tangents.clear(); //��������������
		m_Normals.clear();
		m_VNormals.clear();
	}
};


struct xMeshBone_t
{
    int     m_BoneIndex;//��ͷ��Skeleton�������
	INode*  m_pBone;    //�����ͷ��Ӧ��INode;
};
typedef std::vector<xMeshBone_t> xMeshBones_t;

class xMeshData
{
public:
	sVertexData_t                    m_VertexData;
	IndexBuffers_t                   m_IndexBuffers;
	Materials_t                      m_Materials;

	sMeshDesc_t*                     m_pMeshDesc;
	sBoundBox_t                      m_BoundBox;
	INode*                           m_pNode;
	xMeshBones_t                     m_UsedBones;
	float                            m_fFps;
	wstring                          m_MatName;
	void addUsedBone(xMeshBone_t& bone)
	{
        for(size_t i = 0 ; i < m_UsedBones.size(); ++i)
		{
             if(m_UsedBones[i].m_pBone == bone.m_pBone)
				 return ;
		}
		m_UsedBones.push_back(bone);
	}
};

class CMaxMesh;
struct sMaxMorphTarget_t
{
    std::wstring    m_MorphName;
	CMaxMesh*  m_pMesh;
};

class CMaxMesh
{
public:
	enum eCreateFlag
	{
		eFaceNotCreated = 0,
		eFaceCreated    = 1,
		eFaceCreateFaild = -1,

	};
public:
	xMeshData                      m_MeshData;
	INode*                         m_pNode;
	vector<sMaxMorphTarget_t>      m_MorphTargets;
	
public:

	CMaxMesh();
	CMaxMesh(INode* pNode);
	~CMaxMesh();
public:

	xMeshData& GetOutputData()
	{
		return m_MeshData;
	}


	bool    init(INode* pNode);
	bool    create(unsigned int iMaxTime = 0);
	bool    free();
	bool    is_exportable();
	void    set_scale(float scale);
	void    set_color_scale(float r_s, float g_s , float b_s , float a_s = 1.0);

	bool    build_face_info(bool forTest);
	bool    build_data_info(bool bSkin = false);
	bool    build_skinmesh_data(CSkeletonExporter* pSkeleton);
	bool    build_morph_target();
	void    find_all_used_bone(CSkeletonExporter* pSkeleton);
	static Matrix3 Transpose(Matrix3& matrix);


private:

	//�������ת��Max�е���͵�������Ĺ�ϵ��
	sFace_t  __add_face(sMaxFace_t& face);
	int      __insert_vertex_index(sVeretIndex_t& vi, VertexIndexSet_t& viset);
	bool     __is_mat_tex_layer_map_same(sMaxMatrial_t& lhv,sMaxMatrial_t& rhv);
	int      __count_used_channel(sMaxMatrial_t& mat);
	bool     __build_mesh_desc();
	void     __remove_unused_mat();
	void     __build_used_map();
	int      __find_all_used_map_channel();
	int      __find_used_map_channel_index(int ch);
	bool     __test_used_mapchannel_active();
	void     __clear_index_buf();
	int      __find_mat_by_matid(int matID);
	int      __find_face_mat(Face* face);
	bool     __is_mat_used(int matID);
	bool     __cacl_tbn(sFace_t& face,bool isSkin);
	void     __build_normals(bool isSkin = false);
	void     __build_vertex_normals(bool isSkin = false);
	void     __build_face_normals();
	void     __build_skin_face_normals();
	bool     __find_channel_tm(int ch_index,Matrix3& chTM);


	bool     __find_all_bone_phy(IPhyContextExport *mcExport,CSkeletonExporter* pSkeleton);
	bool     __find_all_bone_skin(ISkin* pSkin,ISkinContextData* pSkinContext ,CSkeletonExporter* pSkeleton);



	void     __build_smgrp_normal();
	int      __add_smgrp_normal(const MaxNormal& max_normal , MaxNormalSet_t& nSet);
	Point3   __get_smgrp_normal(const MaxNormal& max_normal);
	Point3   __get_smgrp_normal(int iNormal);

	bool     __calc_skinmesh_vertex_weights(CSkeletonExporter* pSkeleton);
	bool     __extract_vertex_weight_fromphysique(IPhyContextExport *mcExport,CSkeletonExporter* pSkeleton);
	bool     __extract_vertex_weight_fromskin(ISkin* pSkin, ISkinContextData* pSkinContext ,CSkeletonExporter* pSkeleton);
	bool     __extract_vertex_weight_fromlinkednode(INode* pFakeBone, CSkeletonExporter* pSkeleton);

protected:
	void    _getFaceNormal(int iFace, Point3& n0, Point3& n1, Point3& n2);
	Point3  _getVertexNormal(RVertex * prvertex,DWORD smGroupFace);
	void    _freeMeshData();
	bool    _BuildMaterials(Mtl *pMtl);
	bool    _buildIMesh(unsigned int iMaxTime);
	void    _init_data();
	bool    _buildMaterialInfo(sMaxMatrial_t& mat, StdMat* pMat);
	bool    _build_face_info();
	void    _free_face_info();

private:
	/*��Mesh������صĶ���*/
	sMeshDataFlag m_MeshDataFlag;

	Mesh*         m_pMesh;
	TriObject*    m_triObjNeedDelete;
	eCreateFlag   m_State;
	int           m_iMaxTime;
	int           m_iFrame;
	bool          m_bMirrored;



	//Ϊ�˵���Morpher�á�
	MorphR3*      m_pMaxMophModifier;
	IGameObject*  m_pGameObject;
	IGameMorpher* m_pMorpModifer;

private:
	sVectors_t                       m_VertexNormals;
	sVectors_t                       m_FaceNormals;	
protected:

	void __reserve_normal(int nNormal)
	{
		m_FaceNormals.reserve(nNormal);
		m_VertexNormals.reserve(nNormal);

		m_NormalFaces.reserve(nNormal);
		m_MaxNormals.reserve(nNormal);

		//m_MaxNormalIndex.reserve(nNormal);
	}



	//������ص�������Ϣ

	NormalFaces_t                    m_NormalFaces;
	MaxNormals_t                     m_MaxNormals;
	VertexIndexs_t                   m_VertexIndexs;
	VertexIndexSet_t                 m_VertexIndexSet;
	StdMtls_t                        m_vStdMtls;
	UsedMapChannels_t                m_UsedMapChannels;

	typedef map<MaxNormal,int>       MaxNormalIndexs;
	MaxNormalIndexs                  m_MaxNormalIndex;


	//����������ص�����
	sWeightedVertexs_t               m_VertexWeights;
};

typedef std::vector<CMaxMesh> MaxMeshs;

void    BuildBonesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in);

void    BuildMeshNodesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in);
void    BuildMeshNodesFromScene( sMaxNodes_t& out );
void    BuildNoesFromScene( sMaxNodes_t& out );
void    BuildNoesFromSelectedNode( sMaxNodes_t& out );

void    BuildSkinMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in,CSkeletonExporter* pSkeleton);
void    BuildMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in);
void    ReleaseMaxMeshs(MaxMeshs& meshs);

#endif

