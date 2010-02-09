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
#include "MaxMeshBuilder.h"
#include "FrameAniModelExpoter.h"
#include "xXmlDocument.h"
#include "xExportFunction.h"
CFrameAniModelExporter::CFrameAniModelExporter(CMaxNodeTree& nodeTree)
:m_nodeTree(nodeTree)
{
    sMaxNodes_t nodes;
    m_nodeTree.buildFinalSelNodes(nodes);
    BuildMeshsFromNodes(m_ExpContex.m_MaxMeshs,nodes);
}

bool CFrameAniModelExporter::export()
{
    wchar_t file_name[256];
    file_name[0] = 0;
    if(false == _get_saved_filename(file_name) )
    {
        MessageBoxA(GetActiveWindow(),"û��ѡ���ļ�","������ȡ��",MB_OK);
        return false;
    }
    if(false == _show_expeort_dlg() )
        return false;

    if(m_ExpContex.m_bExpPackage == false)
        return _export_to_file(file_name);
    else
        return _export_to_package(file_name);
	ReleaseMaxMeshs(m_ExpContex.m_MaxMeshs);
    return true;
}

bool CFrameAniModelExporter::__export_to_xcomdoc()
{
	__build_action_datas();
	wstring errostring ;
	vector<wstring> mesh_names;
	for(size_t i = 0 ; i < m_ExpContex.m_MaxMeshs.size(); ++i)
	{
		CMaxMesh& mesh = m_ExpContex.m_MaxMeshs[i];
        xMeshData& meshData = mesh;
		//�������mesh
		mesh.SetScale(m_ExpContex.m_fModelScale);

		//�������Mesh�������Ϣ
		if(mesh.CreateFaces() == false || mesh.CreateMesh(0 , NULL) == false  )
		{
			errostring = std::wstring(L"����[")+ INodeName(mesh.m_pNode)+ L"]���������Ϣ����";
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   {Error} %s\n",errostring.c_str() );
			mesh.Free();
			continue;
		}

		//Ϊ�˷�ֹ�������õ�һ�����õ�����
		wstring mesh_name = __get_avialabale_mesh_name(L"", INodeName(mesh.m_pNode) );
		//������ֱ��������ˣ��ʹ�ӡ������Ϣ
		if(mesh_name != INodeName(mesh.m_pNode))
		{
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   {Error} %s\n",errostring.c_str() );
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   {Warning}[%s] ����Ϊ [%s]\n",INodeName(mesh.m_pNode),mesh_name.c_str() );
		}

		bool exp_mesh_ret = false;

		//�������maxmesh
		if(m_ExpContex.m_bTextMode)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   >>--Exporting<TextMode>: %s\n", mesh_name.c_str() );
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   >>--Exporting<BinMode>: %s\n" , mesh_name.c_str() );
		}

		XEVOL_LOG(eXL_DEBUG_HIGH  , L"       nFace=%d , nSubMesh=%d\n" , meshData.m_MeshDesc.m_nFace , meshData.m_MeshDesc.m_nSubMesh );
		exp_mesh_ret = __export_mesh(L"",mesh_name,mesh);

		if(exp_mesh_ret)
		{
			//������һ������ӵ������б���
			mesh_names.push_back(mesh_name);
		}
		mesh.Free();
	}

	//����ļ�ͷ
	__export_header(L"",mesh_names);
	XEVOL_LOG(eXL_DEBUG_HIGH  , L"   >>Exporting Finshing: %d meshs exporting\r\n" , (int)mesh_names.size() );

	//���Action����Ϣ
	m_ActionDatas.clear();
	return true;
}

bool CFrameAniModelExporter::_export_to_file(wchar_t* file_name)
{
    //�õ���ǰ��ʱ�䣮����ֻ��Ҫ������ǰ��һ֡
	XEVOL_LOG(eXL_DEBUG_HIGH , L"Begin to export model to %s \n" , file_name );
    DeleteFileW(file_name);
    m_doc.create(file_name);
    bool ret = __export_to_xcomdoc();
	G_MaxEnv().SaveLogToXComdoc(m_doc);
    m_doc.close();
    return ret;
}

bool CFrameAniModelExporter::_export_to_package(wchar_t* file_name)
{
     return true;
}

bool CFrameAniModelExporter::__export_header(std::wstring prefix,vector<wstring>& mesh_names)
{
	m_ExpContex.m_ModelDesc.m_nSubMesh = (int)mesh_names.size();
	//д��ÿһ֡��Ӧ����Ϣ��������ʱֻ�а�Χ��
	int nFrame = (int)m_FrameBoundBoxs.size();
	sBoundBox_t _bb;
	_bb.clear(100000000000.0f);
	for(int i = 0 ; i < nFrame ; ++ i)
	{
		_bb.combine(m_FrameBoundBoxs[i]);
	}


    
    //write the model desc
    //m_doc.create_stream( (prefix + MODEL_TYPE_STRING(XRM_STATIC_MODEL) ).c_str());
    xcomdocstream* pstream =  m_doc.create_stream((prefix + L"desc.xml").c_str(),(xcd_data_type)cddt_frame_ani_model_desc);
	xXmlDocument xml;
	xXmlNode* rootNode = xml.insertNode(L"model");
	rootNode->insertValue(L"type")->setValue(L"frame");
	rootNode->insertValue(L"name")->setValue(m_ExpContex.m_ModelDesc.m_Name);
	xXmlNode* bbNode = rootNode->insertNode(L"BoundBox");
	bbNode->insertValue(L"min_x")->setValue(_bb.m_minx);
	bbNode->insertValue(L"min_y")->setValue(_bb.m_miny);
	bbNode->insertValue(L"min_z")->setValue(_bb.m_minz);
	bbNode->insertValue(L"max_x")->setValue(_bb.m_maxx);
	bbNode->insertValue(L"max_y")->setValue(_bb.m_maxy);
	bbNode->insertValue(L"max_z")->setValue(_bb.m_maxz);
	for(size_t i = 0 ; i<mesh_names.size();++i)
	{
		xXmlNode* meshNode = rootNode->insertNode(L"mesh");
		meshNode->insertValue(L"name")->setValue(mesh_names[i].c_str());
	}

	pstream->stream_seekw(xcdsd_beg,0);
	xcdstream out_stream(pstream);
	xml.setXMLStyle(true);
	xml.save(out_stream);
	out_stream.close();
	m_doc.close_stream(pstream);
	xml.unload();

    //д��ÿһ����������
    xcomdocstream* pActstream =  m_doc.create_stream((prefix + L"actions.xml").c_str());

	xXmlDocument xmlAct;
	xXmlNode* rootActNode = xmlAct.insertNode(L"actions");
	int nAct = (int)m_ExpContex.m_Actions.size();
    nFrame = 0;
    for(int i = 0 ; i < nAct ; ++i)
	{
		xXmlNode* actNode = rootActNode->insertNode(L"action");
		sActionInfo_t& action = m_ActionDatas[i].m_ActionInfoExp.m_BaseInfo ;
		action.m_nFrame = GetActNumFrame(action);
		actNode->insertValue(L"Name")->setValue(action.m_Name );
		actNode->insertValue(L"nFrame")->setValue(action.m_nFrame);
		actNode->insertValue(L"DurTime")->setValue(action.m_lTime);
		actNode->insertValue(L"FirstFrame")->setValue(action.m_iFirstFrame);
		actNode->insertValue(L"LastFrame")->setValue(action.m_iLastFrame);
		actNode->insertValue(L"ActionType")->setValue(action.m_ActionType);
        nFrame += (action.m_ActionType - action.m_iLastFrame);
	}

	xcdstream out_actstream(pActstream);
	pActstream->stream_seekw(xcdsd_beg,0);
	xmlAct.setXMLStyle(true);
	xmlAct.save(out_actstream);
	out_actstream.close();
	m_doc.close_stream(pActstream);
	xmlAct.unload();



    return false;
}

bool CFrameAniModelExporter::__export_mesh(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh)
{
    //��д�������Ϣ


	vMaxTimes exportTimes;
    for(int i = 0 ; i < int(m_ActionDatas.size()) ; ++i)
    {

		sMaxActionInfo_t& action = m_ActionDatas[i].m_ActionInfoExp;
		sMaxActionInfo_t& action_raw = m_ExpContex.m_Actions[i]; 
		xMeshData&   meshData = mesh;
		int frame_s = action_raw.m_BaseInfo.m_iFirstFrame;
		int frame_e = action_raw.m_BaseInfo.m_iLastFrame;
		int frame_n = abs(frame_e - frame_s) + 1;
		int frame_delta = 1;
		unsigned int iMaxTime = frame_s * GetTicksPerFrame();
		if(frame_e < frame_s) frame_delta = -1;

		for(int iframe = frame_s ; iframe != frame_e ; iframe += frame_delta)
		{
			iMaxTime = iframe * GetTicksPerFrame();
			exportTimes.push_back(iMaxTime);
		}
    }
	if( __export_mesh_faceinfo (prefix,mesh_name,mesh) == false)
		return false;

    if(false == __export_mesh_action(prefix,mesh_name,mesh, exportTimes) )
		return false;


	return true;
}

bool CFrameAniModelExporter::__export_mesh_action(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh,vMaxTimes& exportTimes)
{
	xMeshData&   meshData = mesh;
	INode*       pNode = mesh.m_pNode;
    unsigned int iMaxTime = exportTimes[0];
	mesh.Free();
	G_SetCurrentTime(iMaxTime);
	mesh.Init(pNode , NULL);
    if(mesh.CreateMesh(iMaxTime , NULL) == false)
        return false;
    if(mesh.BuildVertexData(NULL) == false)
        return false;

    std::wstring mesh_dir  = prefix + mesh_name;
    int nVert = (int)meshData.m_VertexData.m_Positons.size();
    //д��Postion
    std::wstring mesh_pos_file = mesh_dir + L"/position";
    xcomdocstream* pPosStream = m_doc.create_stream(mesh_pos_file.c_str());
    pPosStream->write(nVert);
    //д��Normal
    std::wstring mesh_norma_file = mesh_dir + L"/normal";
    xcomdocstream* pNormalStream = m_doc.create_stream(mesh_norma_file.c_str());
    pNormalStream->write(nVert);

    //д��Diffuse
    std::wstring mesh_diffuse_file = mesh_dir + L"/diffuse";
    xcomdocstream* pdiffuseStream = m_doc.create_stream(mesh_diffuse_file.c_str());
    pdiffuseStream->write(nVert);
    //д��Tangent
    std::wstring mesh_tangent_file = mesh_dir + L"/tangent";
    xcomdocstream* pTangentStream = m_doc.create_stream(mesh_tangent_file.c_str());
    pTangentStream->write(nVert);

    xcomdocstream* pVNormalStream  = NULL;

    //���㷨����
    if(G_MaxEnv().m_bExpVertexNormal)
    {
        std::wstring vmesh_norma_file = mesh_dir + L"/vnormal";
        pVNormalStream = m_doc.create_stream(vmesh_norma_file.c_str());
        pVNormalStream->write(nVert);
    }

    //д��UVChannel
    vector<xcomdocstream*> UVChannelStreams;
    for(int iUVCh = 0 ; iUVCh < meshData.m_VertexData.m_nUVChannes ; ++iUVCh )
    {
        wchar_t buf[32];
        swprintf(buf,L"/uv_%d",iUVCh);
        std::wstring  file_name = mesh_dir + buf;
        xcomdocstream* pUVStream = m_doc.create_stream(file_name.c_str());
        pUVStream->write(nVert);
        UVChannelStreams.push_back(pUVStream);
    }


	size_t nTimes = exportTimes.size();
	m_FrameBoundBoxs.clear();
    for(size_t i  = 0 ; i < nTimes ; i++ )// iframe = frame_s ; iframe != frame_e ; iframe += frame_delta)
    {
		sBoundBox_t _bb;

		_bb.clear(100000000000000.0f);
        iMaxTime = exportTimes[i];
        if(i != 0)
        {
			mesh.Free();
			G_SetCurrentTime(iMaxTime);
			mesh.Init(pNode , NULL);
            mesh.CreateMesh(iMaxTime , NULL);
            mesh.BuildVertexData(FALSE);
        }
		//����λ��
		for(int i = 0 ; i <  nVert ; ++i )
		{
		    sVector_t& v = meshData.m_VertexData.m_Positons[i];
			_bb.add_vertex(v);
		    pPosStream->write(v);
		}
	    m_FrameBoundBoxs.push_back(_bb);
		//������
		for(int i = 0 ; i <  nVert ; ++i )
		{
		    sVector_t& norm = meshData.m_VertexData.m_Normals[i];
		    pNormalStream->write(norm);
		}

		//������
		if(G_MaxEnv().m_bExpVertexNormal)
		{
		    for(int i = 0 ; i <  nVert ; ++i )
		    {
		        sVector_t& norm = meshData.m_VertexData.m_VNormals[i];
		        pVNormalStream->write(norm);
		    }
		}
		
		//��ɫ
		for(int i = 0 ; i <  nVert ; ++i )
		{
		    sColor_t & cl = meshData.m_VertexData.m_Diffuses[i];
		    pdiffuseStream->write(cl);
		}

		//Tangent space
		for(int i = 0 ; i <  nVert ; ++i )
		{
		    sVector_t & tang = meshData.m_VertexData.m_Tangents[i];
		    pTangentStream->write(tang);
		}

		//UV Channel
		for(int iUVCh = 0 ; iUVCh < meshData.m_VertexData.m_nUVChannes ; ++iUVCh )
		{
		    xcomdocstream* pUVStream = UVChannelStreams[iUVCh];
		    for(int i = 0 ; i <  nVert ; ++i )
		    {
		        sUVCoord_t & uv = meshData.m_VertexData.m_UVChannels[iUVCh][i];
		        pUVStream->write(uv);
		    }
		}
    }

    m_doc.close_stream(pPosStream);
    m_doc.close_stream(pNormalStream);
    m_doc.close_stream(pdiffuseStream);
    m_doc.close_stream(pTangentStream);

    //���㷨����
    if(G_MaxEnv().m_bExpVertexNormal)
    {
        m_doc.close_stream(pVNormalStream);
    }
    //m_doc.close_stream(pFNormalStream);
    for(int iUVCh = 0 ; iUVCh < meshData.m_VertexData.m_nUVChannes ; ++iUVCh )
    {
        xcomdocstream* pUVStream = UVChannelStreams[iUVCh];
        m_doc.close_stream(pUVStream);
    }
    UVChannelStreams.clear();
	mesh.Free();
    return true;
}
bool CFrameAniModelExporter::__export_mesh_faceinfo(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh)
{
	xMeshData& meshData = mesh;
    std::wstring mesh_dir  = prefix + mesh_name;
    m_doc.create_stream(mesh_dir.c_str(),(xcd_data_type)cddt_frame_ani_mesh_dir);

	sSkeletonID_t sid;
	sid.m_HiWord = 0;
	sid.m_LoWord = 0;


    //��������һ��mesh��frame��Ϣ��
    std::wstring frame_desc_file = mesh_dir + L"/frame";
    int nFrame = 0 ;
    for(size_t i = 0 ; i < m_ActionDatas.size() ; i ++)
    {
        FrameAniActionData_t& acdata=m_ActionDatas[i];
        sMaxActionInfo_t& acinfo = acdata.m_ActionInfoExp;
        nFrame += (acinfo.m_BaseInfo.m_iLastFrame - acinfo.m_BaseInfo.m_iFirstFrame + 1); 
    }

    //����������
    xcomdocstream* pFrameStream = m_doc.create_stream(frame_desc_file.c_str(),(xcd_data_type)cddt_frame_ani_mesh_desc);
    int nAction = (int)m_ActionDatas.size();
    //һ���м���frame.
    pFrameStream->write(nFrame);
    pFrameStream->write(nAction);
    for(size_t i = 0 ; i < m_ActionDatas.size() ; i ++)
    {
        FrameAniActionData_t& acdata=m_ActionDatas[i];
        sMaxActionInfo_t& acinfo = acdata.m_ActionInfoExp;
        pFrameStream->write(acinfo);   
    }

    m_doc.close_stream(pFrameStream);
    //д��Mesh��Material
    exportMaterialToStream(mesh_dir , m_doc ,meshData );
    //д��face Index. Index buffer
	exportFaceToStream(mesh_dir , m_doc , meshData);

	//exportMeshDescToStream(mesh_dir , m_doc , meshData,nFrame,sid);
	exportMeshDescToStreamAsXML(mesh_dir , m_doc , meshData,nFrame,sid);

    return true;
}


