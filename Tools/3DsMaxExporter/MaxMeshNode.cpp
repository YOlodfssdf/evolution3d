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

#include "stdafx.h"
#include "MaxMeshNode.h"
#include "SkeletonExp.h"
//#include "wm3.h"
void    BuildBonesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in)
{
	size_t nBone = in.size(); 
	out.clear();
	out.reserve(nBone);
	for(size_t i = 0;  i < nBone ; ++i)
	{
		INode* pNode = in[i];
		if(  CMaxEnv::singleton().IsBone(pNode)  )
		{
			out.push_back(pNode);
		}

	}
}
void    BuildMeshNodesFromNodes( sMaxNodes_t& out, sMaxNodes_t& in)
{

	size_t nMesh = in.size(); 
	out.clear();
	out.reserve(nMesh);
	for(size_t i = 0;  i < nMesh ; ++i)
	{
		INode* pNode = in[i];
		CMaxMesh mesh;
		if(mesh.init(pNode) == false)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��Mesh��ȡ������\n" , INodeName(pNode).c_str() );
			continue;
		}

		if(mesh.is_exportable() == false)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��Mesh��ȡ������", INodeName(pNode).c_str() );
			continue;
		}
		mesh.free();
		out.push_back(pNode);         
	}
}

void    ExtracAllNodes(INode* pNode, sMaxNodes_t& out)
{

	out.push_back(pNode);
    for(int i = 0 ; i  < pNode->NumberOfChildren() ; i++)
	{		
		ExtracAllNodes(pNode->GetChildNode(i) , out);
	}
}


void    BuildNoesFromSelectedNode( sMaxNodes_t& out )
{
	for(int i = 0 ; i < CMaxEnv::singleton().m_pInterace->GetSelNodeCount() ; ++i)
	   out.push_back(CMaxEnv::singleton().m_pInterace->GetSelNode(i) );
}

void    BuildNoesFromScene( sMaxNodes_t& out )
{
	 ExtracAllNodes(CMaxEnv::singleton().m_pInterace->GetRootNode(),out);
}

void    BuildMeshNodesFromScene( sMaxNodes_t& out )
{
	sMaxNodes_t nodes;
	BuildNoesFromScene(nodes);
	BuildMeshNodesFromNodes(out,nodes);
	nodes.clear();
}

void    BuildSkinMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in,CSkeletonExporter* pSkeleton)
{
    size_t nMesh = in.size(); 
    out.clear();
    out.reserve(nMesh);
    for(size_t i = 0;  i < nMesh ; ++i)
    {
        INode* pNode = in[i];
        CMaxMesh mesh;
        if(mesh.init(pNode) == false)
        {
           XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��Mesh��ȡ������\n" , INodeName(pNode).c_str() );
            continue;
        }

        //if(CMaxEnv::FindPhysiqueModifier(pNode) == NULL &&)
        //{
        //    XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��SkinMesh��ȡ������\n" , INodeName(pNode).c_str() );;
        //    continue;
        //}

        if(mesh.is_exportable() == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��Mesh��ȡ������\n" , INodeName(pNode).c_str() );
            continue;
        }
		if(pSkeleton)
		    mesh.find_all_used_bone(pSkeleton);
        mesh.free();
        out.push_back(mesh);         
    }
}
void    BuildMeshsFromNodes(MaxMeshs& out, sMaxNodes_t& in)
{
    size_t nMesh = in.size(); 
    out.clear();
    out.reserve(nMesh);
    for(size_t i = 0;  i < nMesh ; ++i)
    {
        INode* pNode = in[i];
        CMaxMesh mesh;
        if(mesh.init(pNode) == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��Mesh��ȡ������\n" , INodeName(pNode).c_str() );
            continue;
        }
        if(mesh.is_exportable() == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ]����һ��Mesh��ȡ������\n" , INodeName(pNode).c_str() );
            continue;
        }
        mesh.free();
        out.push_back(mesh);         
    }
}

void    ReleaseMaxMeshs(MaxMeshs& meshs)
{
    size_t nMesh = meshs.size();
    for(size_t i  = 0 ; i < nMesh ; ++i)
    {
        meshs[i].free();
    }
    meshs.clear();
}
CMaxMesh::~CMaxMesh()
{
    _freeMeshData();

}
CMaxMesh::CMaxMesh()
{
    _init_data();
	m_State = eFaceNotCreated;
}

CMaxMesh::CMaxMesh(INode* pNode)
{
    m_State = eFaceNotCreated;
    _init_data();
    init(pNode);
}
void  CMaxMesh::set_scale(float scale)
{
    m_MeshDataFlag.m_fScale = scale;
}

void  CMaxMesh::set_color_scale(float r_s, float g_s , float b_s , float a_s)
{
    m_MeshDataFlag.m_fColorScale[0] = r_s;
    m_MeshDataFlag.m_fColorScale[1] = g_s;
    m_MeshDataFlag.m_fColorScale[2] = b_s;
    m_MeshDataFlag.m_fColorScale[3] = a_s;
}

bool CMaxMesh::init(INode* pNode)
{
    _freeMeshData();
    assert(pNode);
    if( false == CMaxEnv::singleton().IsMesh(pNode) )
    {
        //sprintf(buf,"����Meshʧ��!, Node����һ����Ч��Mesh [Node = %s]",INodeName(pNode));
        //MessageBox(GetActiveWindow(),buf,"����",MB_OK);
        return false;
    }
    m_pNode = pNode;
    m_MeshData.m_pNode = pNode;
    return true;
}

bool CMaxMesh::create(unsigned int iMaxTime)
{

    assert(m_pNode);
    int tickt_per_frame = GetTicksPerFrame();
    

    m_iMaxTime = (int)iMaxTime;
    m_iFrame   = m_iMaxTime/tickt_per_frame;
	CMaxEnv::singleton().m_pIGameScene->SetStaticFrame(m_iFrame);


    if(_buildIMesh(m_iMaxTime) == false)
    {
        return false;
    }

    m_pMesh->buildNormals();

    Matrix3					ObjMatrix;
    Point3					r1, r2, r3, r12;
    float					Mirror;
    ObjMatrix = m_pNode->GetObjTMAfterWSM(m_iMaxTime);
    r1 = ObjMatrix.GetRow(0);
    r2 = ObjMatrix.GetRow(1);
    r3 = ObjMatrix.GetRow(2);
    r12 = CrossProd(r1, r2);//����������
    Mirror = DotProd(r12, r3);//�Ƕȼ���
    m_bMirrored = Mirror < 0;

	IGameNode* pIGameNode = CMaxEnv::singleton().m_pIGameScene->GetIGameNode(m_pNode);
	m_pGameObject = pIGameNode->GetIGameObject();
	if(m_pGameObject == NULL)
	{
		MessageBox(GetActiveWindow(),"�������ϵ۰���IGameObject��Ȼ����ʧ���ˣ�����","Faint!",MB_OK);
		return false;
	}
	int nIModifier = m_pGameObject->GetNumModifiers();
	m_pMorpModifer = NULL;
	for(int i = 0 ; i < nIModifier ; i ++)
	{
		IGameModifier* pModifier = m_pGameObject->GetIGameModifier(i);
		if(pModifier->GetModifierType() == IGameModifier::IGAME_MORPHER)
		{
			m_pMorpModifer = (IGameMorpher*)pModifier;
		}
	}
    return true;
}
bool CMaxMesh::free()
{
    _freeMeshData();
	m_pMorpModifer = NULL;
	m_pGameObject  = NULL;
    return true;
}
bool CMaxMesh::is_exportable()
{
    //�ܴ���
    if( false == create(0) )
        return false;
    if( false == build_face_info(true) )
    {
        _free_face_info();
        return false;
    }
    return true;
}

bool  CMaxMesh::build_face_info(bool forTest)
{
    _free_face_info();
    Mtl* pMat =  m_pNode->GetMtl();
    if(pMat == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} Node[%s] û�в���" , INodeName(m_pNode).c_str() );
        return false;
    }

    m_UsedMapChannels.clear();
    m_vStdMtls.clear();
    m_MeshData.m_Materials.clear();

	__find_all_used_map_channel();
    //�������е�Material�����Ϣ
    //1.  ʹ�õ���MapChannel
    //2.  ���е�(����û��ʹ�ù���)Mtl
    //3.  ��ȡ��� (ʹ�õ��ģ���������һЩ��Ϣ) Materials�б�
    if(_BuildMaterials(pMat) == false)
    {
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} Node[%s] ����Materialʧ��\n" , INodeName(m_pNode).c_str() );
        return false;
    }

    //û���õ�һ�������ǲ��е�
    int nMat = (int)m_MeshData.m_Materials.size();
    if(nMat <= 0)
    {
		wstring errorString = std::wstring(L"Ӧ�õ�[") + std::wstring(INodeName(m_pNode)) + L"]�ϵĲ��ʸ���Ϊ0"; 
 		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errorString.c_str() );
        MessageBoxW(GetActiveWindow(),errorString.c_str(),L"����",MB_OK);
        return false;
    }

    //ȷ��Ӧ�õ���������ϵĶ����Ӳ��ʵ�Actived Channel Map����ͬ
    for(int i = 0 ; i  < nMat - 1 ; ++i)
    {
        if(__is_mat_tex_layer_map_same(m_MeshData.m_Materials[i],m_MeshData.m_Materials[i+1]) == false)
        {
            std::wstring errorString = std::wstring(L"Ӧ�õ�[") + std::wstring(INodeName(m_pNode)) + L"]�Ķ����Ӳ��ʵ�UV Map����ͬ"; 
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errorString.c_str() );
            MessageBoxW(GetActiveWindow(),errorString.c_str(),L"����",MB_OK);
            return false;
        }
    }


    //һ�������õ���ChannelMap��ʹ�������������Mesh��ChannelMap��ʹ�����Ҫ��ͬ
    //Ҳ����˵:
    //<!!!!!!!!!!!!!��������>
    //     Ӧ�õ�һ��Mesh�ϵ����в��ʵ�UVMapChannel��ʹ�����Ҫ��ȫ��ͬ
    //</!!!!!!!!!!!!>
    int n_acmapCh = (int)m_UsedMapChannels.size();
    int n_used_channel = __count_used_channel(m_MeshData.m_Materials[0]);
    if(n_acmapCh != n_used_channel)
    {
        std::wstring errorString = std::wstring(L"Ӧ�õ�[") + std::wstring(INodeName(m_pNode)) + L"]�ϵĲ��ʵ�Actived Channel Map��Ŀ����ʵ���õ���ChannelMap"; 
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errorString.c_str() );
        //MessageBoxA(GetActiveWindow(),errorString.c_str(),"����",MB_OK);
        //return false;
    }

    //�鿴һ�£��õ���ChannelMap�ǲ��Ƕ���Eanable��
    if(__test_used_mapchannel_active() == false)
    {
        std::wstring errorString = std::wstring(L"Ӧ�õ�[") + std::wstring(INodeName(m_pNode)) + L"]�ϵĲ��ʵ�Channel Mapû��Enable"; 
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errorString.c_str() );
        MessageBoxW(GetActiveWindow(),errorString.c_str(),L"����",MB_OK);
        return false;
    }
    //�ƿ���Щû���õ��Ĳ���
    __remove_unused_mat();

    //<!!!!!!!!!!!!!��������>
    //     һ��Mesh������һ������
    //</!!!!!!!!!!!!>
    if(m_MeshData.m_Materials.size() <= 0 )
    {
        std::wstring errorString = std::wstring(L"Ӧ�õ�[") + std::wstring(INodeName(m_pNode)) + L"]�ϵĲ��ʵ���Ч���ʸ���Ϊ0"; 
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errorString.c_str() );
        MessageBoxW(GetActiveWindow(),errorString.c_str(),L"����",MB_OK);
        return false;
    }

    //����Mesh��������Ϣ
    if( false == __build_mesh_desc() )
        return false;


	if(forTest == false)
	{
		m_pMesh->buildNormals();
		m_pMesh->checkNormals(TRUE);

		//Reserver Normal;
		__reserve_normal(m_pMesh->numFaces * 3);

		__build_smgrp_normal();
		//�������ʵ��������Ϣ
		if( false == _build_face_info() )
			return false;
		return true;
	}
	else
	{
		if(m_pMesh->faces == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error}[ %s ] facesָ��Ϊ�� \n" , INodeName(m_pNode).c_str() );
			return false;
		}
		if(m_pMesh->tvFace == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error}[ %s ] tvfacesָ��Ϊ�� \n" , INodeName(m_pNode).c_str() );
			return false;
		}
		return true;
	}
}

Matrix3 CMaxMesh::Transpose(Matrix3& matrix)
{
    float transpose[4][3];

    int row;
    for(row = 0; row < 3; row++)
    {
        int column;
        for(column = 0; column < 3; column++)
        {
            transpose[column][row] = matrix.GetAddr()[row][column];
        }
    }

    int column;
    for(column = 0; column < 3; column++)
    {
        transpose[3][column] = 0;
    }

    Matrix3 transposeMatrix(transpose);
    transposeMatrix.NoTrans();

    return transposeMatrix;
}

bool  CMaxMesh::build_data_info(bool bSkin)
{
    m_MeshData.m_VertexData.clear();
    m_MeshData.m_BoundBox.clear(100000000.0f);

    sMaxMatrial_t& mat = m_MeshData.m_Materials[0];

    m_pMesh->buildNormals();
    m_pMesh->checkNormals(TRUE);

    int nVert  = (int)m_VertexIndexs.size();
	int nUV    = (int)m_UsedMapChannels.size();

	m_MeshData.m_VertexData.reserve(nVert,nUV);
    m_MeshData.m_VertexData.m_nUVChannes = (int)m_UsedMapChannels.size();

    Matrix3 PosMat = m_pNode->GetObjTMAfterWSM(m_iMaxTime);

    Matrix3 NrmMat = Inverse(Transpose(PosMat));
    NrmMat.NoTrans();
    for(int i = 0 ; i < nVert ; ++i)
    {
        sVeretIndex_t& Vi = m_VertexIndexs[i];
        Point3    max_vertex;
        sVector_t v;

        //����Vi������m_nUVӦ����ͬ
        for(int iMapCh = 0 ; iMapCh < Vi.m_nUV ; ++iMapCh)
        {
            int ch = m_UsedMapChannels[iMapCh];

            _XASSERT(m_pMesh->getNumMapVerts(ch) >= Vi.m_UVs[iMapCh]);

            UVVert* pUVert =  m_pMesh->mapVerts(ch);
            max_vertex.x  = pUVert[Vi.m_UVs[iMapCh]].x;
            max_vertex.y  = pUVert[Vi.m_UVs[iMapCh]].y;
            max_vertex.z  = pUVert[Vi.m_UVs[iMapCh]].z;

            //<!!!!!!!!!!!!!��������>
            //�õ����Channel��TM��Ҫ������ʹ�����Channel��SubMaterial�е�TexMap��������ͬ��TM
            //��Ȼ�������Ķ����᲻����
            //</!!!!!!!!!!!!>
            Matrix3 chMat;
            chMat.IdentityMatrix();
            __find_channel_tm(iMapCh,chMat);
            max_vertex = max_vertex * chMat;            
            sUVCoord_t tv;
            tv.u = max_vertex.x; 
			tv.v = max_vertex.y ;

			//D3D���
			if( CMaxEnv::singleton().m_bInvertTexCoordV )
			{
				tv.v = 1.0f - max_vertex.y ;
			}
            m_MeshData.m_VertexData.m_UVChannels[iMapCh].push_back(tv);
        }
        //Position
        _XASSERT(m_pMesh->getNumVerts() >= Vi.m_iPos);

        max_vertex = m_pMesh->getVert(Vi.m_iPos)* PosMat;
        //ģ����С�Ŵ�
        v.x = max_vertex.x * m_MeshDataFlag.m_fScale; 
        v.y = max_vertex.y * m_MeshDataFlag.m_fScale; 
        v.z = max_vertex.z * m_MeshDataFlag.m_fScale;

        m_MeshData.m_BoundBox.add_vertex(v);
        m_MeshData.m_VertexData.m_Positons.push_back(v);

        //Diffuse
        sColor_t color ;
        color.a = 255;
        if(m_pMesh->getNumVertCol() == 0)
        {
            color.r = 255 * m_MeshDataFlag.m_fColorScale[0];
            color.g = 255 * m_MeshDataFlag.m_fColorScale[1];
            color.b = 255 * m_MeshDataFlag.m_fColorScale[2];    
        }
        else
        {
            VertColor& vc =  m_pMesh->vertCol[Vi.m_iDiffuse];
            max_vertex.x = vc.x;
            max_vertex.y = vc.y; 
            max_vertex.z = vc.z; 
            color.r = max_vertex.x * 255 * m_MeshDataFlag.m_fColorScale[0];
            color.g = max_vertex.y * 255 * m_MeshDataFlag.m_fColorScale[1];
            color.b = max_vertex.z * 255 * m_MeshDataFlag.m_fColorScale[2];
        }

		if(m_pMesh->mapSupport(-VDATA_ALPHA))
		{
			UVVert* pAlpha = m_pMesh->mapVerts(-VDATA_ALPHA);
			assert(pAlpha);
			float alpha = pAlpha[Vi.m_iAlpha].x;
			color.a = alpha * 255;
		}
        m_MeshData.m_VertexData.m_Diffuses.push_back(color);

        sVector_t zero_v ;
        zero_v.x = 0.0f ;zero_v.y = 0.0f;zero_v.z = 0.0f;
        sVector_t nrl;
        Point3 nrml_pt3 = __get_smgrp_normal(Vi.m_iNormal) * NrmMat;
        nrml_pt3.Normalize();
        nrl.x = nrml_pt3.x;
        nrl.y = nrml_pt3.y;
        nrl.z = nrml_pt3.z;
        m_MeshData.m_VertexData.m_Normals.push_back(nrl);
        m_MeshData.m_VertexData.m_Tangents.push_back(zero_v);


		//��תYZ
		if( CMaxEnv::singleton().m_bInvertYZCoord )
		{
			float t = m_MeshData.m_VertexData.m_Positons[i].y ; 
			m_MeshData.m_VertexData.m_Positons[i].y = m_MeshData.m_VertexData.m_Positons[i].z ; 
			m_MeshData.m_VertexData.m_Positons[i].z = t;

			t = m_MeshData.m_VertexData.m_Normals[i].y ; 
			m_MeshData.m_VertexData.m_Normals[i].y = m_MeshData.m_VertexData.m_Normals[i].z ; 
			m_MeshData.m_VertexData.m_Normals[i].z = t;

			t = m_MeshData.m_VertexData.m_Tangents[i].y ; 
			m_MeshData.m_VertexData.m_Tangents[i].y = m_MeshData.m_VertexData.m_Tangents[i].z ; 
			m_MeshData.m_VertexData.m_Tangents[i].z = t;
		}


    }
    int nVertex = (int)m_MeshData.m_VertexData.m_Positons.size();
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   Node Mesh: [%s]  ��[%d]������ \n" , INodeName(m_pNode).c_str() , nVertex );
    if(bSkin == false)
    {
        __build_normals(); 
        __build_vertex_normals();
    }

    __build_face_normals();
	m_MeshData.m_pMeshDesc->m_nVertex = (int)m_VertexIndexs.size();
    return true;
}

bool CMaxMesh::build_skinmesh_data(CSkeletonExporter* pSkeleton)
{
    if(fabs(pSkeleton->m_fScale - m_MeshDataFlag.m_fScale) > 0.000001)
    {
        MessageBoxA(GetActiveWindow(),"������ͬ־û�е���Mesh::set_scale(),Mesh��Scale��Skeleton��Scale����ͬ","����",MB_OK);
        assert(0);
    }

	m_MeshData.m_BoundBox.clear(100000000.0f);
	m_MeshData.m_VertexData.m_VertexWeights.clear();
	int nVert  = (int)m_VertexIndexs.size();

	m_VertexWeights.reserve(nVert);

	m_MeshData.m_VertexData.m_VertexWeights.reserve(nVert);
    if(__calc_skinmesh_vertex_weights(pSkeleton) == false)
        return false;

    for(int i = 0 ; i < nVert ; ++i)
    {
        int wi = m_VertexIndexs[i].m_iPos;
        sWeightedVertex_t wv = m_VertexWeights[wi];

		//���Ҫ�����Ǽܳ�ʼλ�ã���ôҪʹ��InitPos�����InitPos��__extract_vertex_weight_fromXXXX����Ѿ�������ˡ�
		//���������Physique���ǿ���ֱ��ȡ������vtxBlend->GetOffsetVector(j). ����Skin�Ļ���Ҫ�Լ�����.
		//ͬʱ����������InitPos�Ļ����ǵû�Ҫ��InitNormalҲ�������

		//���ʹ�õ��ǵ�һ֡����ô��˵��ֱ��ʹ�ø�Static�õ������ݾͺ��ˡ�Pos��Normal��ֱ����
		//�������ʱ��InitTM����TM(0)�����CSkeletonExporter::push_bone(INode* pBone)��������
        if(CMaxEnv::singleton().m_bUseBeforeSkeletonPose)
        {
            wv.m_InitPos.x *= pSkeleton->m_fScale;
            wv.m_InitPos.y *= pSkeleton->m_fScale;
            wv.m_InitPos.z *= pSkeleton->m_fScale;

            //===============================================
            //�����Ƥ�ĳ�ʼ������
			//���Լ�ȥ�㶥��ĳ�ʼλ��һ���ġ���
			//ֻ�Ƿ��ߵı任���������任����ʽInvert Transpose�Ĺ�ϵ���������ת�õĹ�ϵ
			//��������__extract_vertex_weight_fromskin��__extract_vertex_weight_fromphysique
            Point3 nrm0;
            nrm0.x = m_MeshData.m_VertexData.m_Normals[i].x;
            nrm0.y = m_MeshData.m_VertexData.m_Normals[i].y;
            nrm0.z = m_MeshData.m_VertexData.m_Normals[i].z;
            Point3 nrmInit;
            nrmInit.Set(0.0f,0.0f,0.0f);
            for(int iBone = 0 ; iBone < wv.m_nEffBone ; iBone++)
            {
                Matrix3 matMeshInit;
                int boneIndex = wv.m_BoneWeights[iBone].m_BoneIndex ;
                float fWeight = wv.m_BoneWeights[iBone].m_fWeight;
                sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
                assert(wv.m_BoneWeights[iBone].m_BoneID == boneNode.m_Bone.m_BoneID);
                 
				//Normal �ĳ�ʼ����
				Matrix3 normlMat_InitTM = boneNode.m_SkinInitMT;
				normlMat_InitTM.Invert();
				normlMat_InitTM = Transpose(normlMat_InitTM);
	
				//Normal��0֡�任�������
				Matrix3 normlMat_TM0 = boneNode.m_InitNodeTM0;
				normlMat_TM0 = Transpose(normlMat_TM0);

				
                //matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;
				//matMeshInit = Inverse( Inverse(normlMat_TM0) ) * normlMat_InitTM;

				matMeshInit = normlMat_TM0 * normlMat_InitTM;

				matMeshInit.NoTrans();
                Point3 pt = nrm0 * matMeshInit;
                nrmInit.x = nrmInit.x + fWeight * pt.x;
                nrmInit.y = nrmInit.y + fWeight * pt.y;
                nrmInit.z = nrmInit.z + fWeight * pt.z;
            }
            nrmInit.Normalize();
            m_MeshData.m_VertexData.m_Normals[i].x = nrmInit.x;
            m_MeshData.m_VertexData.m_Normals[i].y = nrmInit.y;
            m_MeshData.m_VertexData.m_Normals[i].z = nrmInit.z;
            //================================================

        }
        else
        {
            wv.m_InitPos.x = m_MeshData.m_VertexData.m_Positons[i].x;
            wv.m_InitPos.y = m_MeshData.m_VertexData.m_Positons[i].y;
            wv.m_InitPos.z = m_MeshData.m_VertexData.m_Positons[i].z;
        }


		//��תYZ
		if( CMaxEnv::singleton().m_bInvertYZCoord )
		{
            float t = wv.m_InitPos.y ; wv.m_InitPos.y = wv.m_InitPos.z ; wv.m_InitPos.z = t;
			
			t = m_MeshData.m_VertexData.m_Normals[i].y ; 
			m_MeshData.m_VertexData.m_Normals[i].y = m_MeshData.m_VertexData.m_Normals[i].z ; 
			m_MeshData.m_VertexData.m_Normals[i].z = t;

			t = m_MeshData.m_VertexData.m_Tangents[i].y ; 
			m_MeshData.m_VertexData.m_Tangents[i].y = m_MeshData.m_VertexData.m_Tangents[i].z ; 
			m_MeshData.m_VertexData.m_Tangents[i].z = t;
		}

		m_MeshData.m_BoundBox.add_vertex(wv.m_InitPos);
        m_MeshData.m_VertexData.m_VertexWeights.push_back(wv);
    }
    __build_normals(true);
    __build_skin_face_normals();
    __build_vertex_normals(true);
	m_MeshData.m_pMeshDesc->m_nVertex = (int)m_VertexIndexs.size();
    return  true;
}

bool CMaxMesh::build_morph_target()
{
   
	if(m_pMorpModifer==NULL)
		return false;

	int nMorphTarget = m_pMorpModifer->GetNumberOfMorphTargets();
	m_pMaxMophModifier = (MorphR3*)m_pMorpModifer->GetMaxModifier();
	for(int i = 0 ; i < nMorphTarget ; ++i)
	{
          sMaxMorphTarget_t morphTarget;
		  IGameNode* pGameNode =  m_pMorpModifer->GetMorphTarget(i);
		  //morphTarget.m_MorphName = pGameNode;
	}

	//int nMorphChannel = m_pMaxMophModifier->chanNum;
	//for(int i = 0 ; i < nMorphChannel ; i ++)
	//{
	//	morphChannel& morphCh = m_pMaxMophModifier->chanBank[i];
	//	wstring channelName = morphCh.mName;
	//}

	return true;

}


