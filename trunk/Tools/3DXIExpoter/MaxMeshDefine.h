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

#ifndef __MAX_NODE_MESH_DEFINE_H__
#define __MAX_NODE_MESH_DEFINE_H__
#include "MaxEnv.h"
#include "Math.h"
#include "DataDefine.h"
using namespace XEvol3DModel;
#include <vector>
#include <string>
#include <hash_map>
using namespace std;
using namespace stdext;
class CMaxMesh;

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
	wchar_t           m_TexName[64];       
	unsigned short    m_UVChannel;     //ʹ�õ���һ��UVChannel
	Matrix3           m_UVTransMat;

};

typedef std::vector<sMaxTexLayer> sMaxTexLayers_t;

//��Material��һ����װ��
//������һЩ�������Ϣ
struct sMaxMatrial_t
{
	sMaxTexLayers_t      m_TexLayers;
	int                  m_nTexLayer;
	sMateril_t           m_BaseMat;
	IGameMaterial*       m_pGameMaterial;
};




#define COMPARE_COMPENT_LESS(a , b ) if(a < b) { return true; } if(a > b) {return false;} 
#define COMPARE_COMPENT_EQUAL(a , b ) if(a != b) { return false; } 

class CSkeletonExporter;
class MorphR3;

typedef std::vector<sWeightedVertex_t> sWeightedVertexs_t;
typedef std::vector<sMaxMatrial_t>     Materials_t;

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


struct sMaxMorphTarget_t
{
    std::wstring    m_MorphName;
	CMaxMesh*       m_pMesh;
};

class CMaxVertexIndex
{
public:
	enum 
	{
		MAX_CHANEL = 12,
	};
public:
	DWORD vert;
	DWORD norm;
	DWORD color;
	DWORD alpha;
	DWORD smGrp;
	//! The material ID of the face

	int   nMapChannel;
	IGameMaterial*   pGameMtl;

	int   ChVert[MAX_CHANEL];
};

inline bool operator == (const CMaxVertexIndex& lhv , const CMaxVertexIndex& rhv)
{
	COMPARE_COMPENT_EQUAL(lhv.smGrp    , rhv.smGrp);
	COMPARE_COMPENT_EQUAL(lhv.vert     , rhv.vert);
	COMPARE_COMPENT_EQUAL(lhv.norm     , rhv.norm);
	COMPARE_COMPENT_EQUAL(lhv.color    , rhv.color);	
	COMPARE_COMPENT_EQUAL(lhv.alpha    , rhv.alpha);	
	for(int i = 0 ; i < lhv.nMapChannel ; ++i)
	{
		COMPARE_COMPENT_LESS(lhv.ChVert[i] , rhv.ChVert[i]);
	}
	return true;
}

inline bool operator < (const CMaxVertexIndex& lhv , const CMaxVertexIndex& rhv)
{
	COMPARE_COMPENT_LESS(lhv.smGrp    , rhv.smGrp);
	COMPARE_COMPENT_LESS(lhv.vert     , rhv.vert);
	COMPARE_COMPENT_LESS(lhv.norm     , rhv.norm);
	COMPARE_COMPENT_LESS(lhv.color    , rhv.color);	
	COMPARE_COMPENT_LESS(lhv.alpha    , rhv.alpha);	
	for(int i = 0 ; i < lhv.nMapChannel ; ++i)
	{
		COMPARE_COMPENT_LESS(lhv.ChVert[i] , rhv.ChVert[i]);
	}
	return false;
}

#endif

