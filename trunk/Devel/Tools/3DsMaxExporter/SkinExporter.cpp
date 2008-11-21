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
#include "SkinExporter.h"
#include "xCfgParser.h"
#include "xExportFunction.h"
bool CSkinExporter::__remove_exist_mesh(wstring skin_group_dir,MaxMeshs& mesh_in ,MaxMeshs& mesh_out)
{
	wstring skin_name = skin_group_dir;
	for(size_t i = 0 ; i  < mesh_in.size() ; i++)
	{
		CMaxMesh& mesh = mesh_in[i];
		wstring mesh_name = skin_name + INodeName(mesh.m_pNode);
		if(NULL == m_doc.open_stream(mesh_name.c_str(),true) )
		{
			mesh_out.push_back(mesh);
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {����} Ƥ����[%s] Ҳ�Ѿ����ڣ����Mesh=[%s]��ȡ������\r\n",skin_name.c_str(),mesh_name.c_str() );
		}
	}
	return true;
}

bool CSkinExporter::export_skin(const wstring prefix, bool bTexMode ,  MaxMeshs& maxMeshs_in,  const wstring& SkinName,sBoundBox_t& BoundBox)
{
    wstring skin_name = prefix + SkinName;
	bool bNeedDelete = true;
    //������������Skin�ڰ���·�����ǲ��Ǵ���
    if(  (m_doc.create_stream(skin_name.c_str(),xcddt_dir_flag) == NULL) && SkinName != L"")
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {����} Ƥ����[%s]�Ѿ����ڣ���ȡ���ģʽ\n" , skin_name.c_str() );
		bNeedDelete = false;
        //return false;
	}


	MaxMeshs maxMeshs;

	//ȥ����Щ�Ѿ����ڵ�Mesh
    __remove_exist_mesh(skin_name,maxMeshs_in,maxMeshs);

    //Export all skin meshs
    wstring errostring;
    vector<wstring> mesh_names;
    for(size_t i = 0 ; i < maxMeshs.size(); ++i)
    {
        CMaxMesh& mesh = maxMeshs[i];
        xMeshData& meshData = mesh.GetOutputData();
        //�������mesh
        mesh.create(0);
        mesh.set_scale(m_pSkeleton->m_fScale);

        //�������Mesh�������Ϣ
        if(false == mesh.build_face_info(false) )
        {
            errostring = wstring(L"����[")+INodeName(mesh.m_pNode)+L"]���������Ϣ����";
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
            mesh.free();
            continue;
        }

        //�������mesh��������Ϣ���һض����������Ϣ
        if(false == mesh.build_data_info(true) )
        {
            errostring = wstring(L"����[")+INodeName(mesh.m_pNode)+L"]��������������Ϣ����";
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
            mesh.free();
            continue;
        }


        if(false == mesh.build_skinmesh_data(m_pSkeleton) )
        {
            errostring = wstring(L"����[")+ INodeName(mesh.m_pNode) + L"]����SkinData����Ϣ����";
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
            mesh.free();
            continue;
        }
        //Ϊ�˷�ֹ�������õ�һ�����õ�����
        wstring mesh_name = __get_avialabale_mesh_name(skin_name,INodeName(mesh.m_pNode));
        //������ֱ��������ˣ��ʹ�ӡ������Ϣ
        if(mesh_name != INodeName(mesh.m_pNode))
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Warning}[%s] ����Ϊ [%s] \n " , INodeName(mesh.m_pNode) , mesh_name.c_str() );
        }

        bool exp_mesh_ret = false;

        //�������maxmesh
        if(bTexMode)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   >>--Exporting<TextMode>: Mesh=[%s]\n" , mesh_name.c_str() );
        }
        else
        {
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   >>--Exporting<BinMode>: Mesh=[%s]\n" , mesh_name.c_str() );
        }
		XEVOL_LOG(eXL_DEBUG_HIGH , L"       nFace=%d nSubMesh=%d \n" , meshData.m_pMeshDesc->m_nFace , meshData.m_pMeshDesc->m_nSubMesh );
		XEVOL_LOG(eXL_DEBUG_HIGH , L"       �õ�Bone�ĸ���: %d \n" , mesh.GetOutputData().m_UsedBones.size()  );
		exp_mesh_ret = export_skin_mesh(skin_name,mesh_name,mesh , bTexMode);

        if(exp_mesh_ret)
        {
            //������һ������ӵ������б���
            mesh_names.push_back(mesh_name);
            BoundBox.combine(meshData.m_BoundBox);
        }
        mesh.free();
    }
	if(mesh_names.size() == 0 && bNeedDelete)
	{
		m_doc.delete_stream(skin_name.c_str());
		return false;
	}

    export_skin_header(skin_name,mesh_names);


    XEVOL_LOG(eXL_DEBUG_HIGH , L"   >>Exporting Finshing: %d sub meshs exporting to the skinmesh\r\n" , (int)mesh_names.size() );
    int nMeshExport = (int)mesh_names.size();
	mesh_names.clear();
    return  nMeshExport > 0;
}

bool CSkinExporter::export_skin_header(wstring skin_group_dir,vector<wstring>& mesh_names)
{
    SkinGroupDesc_t skin_desc;
    skin_desc.n_SkinMesh = (int)mesh_names.size();
    //char mesh_name[64];
    xcomdocstream* pstream =  NULL;
	pstream = m_doc.open_stream((skin_group_dir + L"skins.xml").c_str(),true);

	if(pstream == NULL)//û�д���ԭ����Skin_desc�ļ������´���һ��
	{
		pstream = m_doc.create_stream( (skin_group_dir + L"skins.xml").c_str());
		if(pstream == NULL)
			return false;
		
		xCfgDocument xml;
		xCfgNode* rootNode = xml.insertNode(L"skins");
		for(size_t i = 0 ; i<mesh_names.size();++i)
		{
			xCfgNode* meshNode = rootNode->insertNode(L"mesh");
			meshNode->insertValue(L"name")->setValue(mesh_names[i].c_str());
		}

		pstream->stream_seekw(xcdsd_beg,0);
		xcdstream out_stream(pstream);
		xml.setXMLStyle(true);
		xml.save(out_stream);
		out_stream.close();
		m_doc.close_stream(pstream);

	}
	else //�Ѿ����ڣ�����Ҫ������Ķ�����������Ȼ�����¼���
	{
		vector<wstring> old_mesh_names;
		//���ȶ�����
		pstream->stream_seekr(xcdsd_beg,0);
		char* buf = new char[pstream->data_len() + 1];
		pstream->read(buf,pstream->data_len());
		buf[pstream->data_len() ] = 0;
		xCfgDocument xml;
		xml.load( buf,(int)pstream->data_len() ,true);
		xCfgNode* rootNode = xml.root();
		for(size_t i = 0 ; i<mesh_names.size();++i)
		{
			xCfgNode* meshNode = rootNode->insertNode(L"mesh");
			meshNode->insertValue(L"name")->setValue(mesh_names[i].c_str());
		}

		pstream->stream_seekw(xcdsd_beg,0);
		xcdstream out_stream(pstream);
		xml.setXMLStyle(true);
		xml.save(out_stream);
		out_stream.close();
		m_doc.close_stream(pstream);

	}
    return true;
}

bool CSkinExporter::export_skin_mesh(wstring skin_group_dir,wstring mesh_name,CMaxMesh& mesh , bool bTexMode)
{
	xMeshData& meshData = mesh.GetOutputData();
  
    wstring mesh_dir  = skin_group_dir + mesh_name;
    if(NULL == m_doc.create_stream(mesh_dir.c_str(),(xcd_data_type)cddt_static_mesh_dir) )
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"{����}: Skin �е�Mesh�Ѿ����� [%s]\n",mesh_name.c_str() );
		return false;
	}

	//д��Mesh��Description
	sSkeletonID_t sid = m_pSkeleton->getID();

	CModelBaseExportor::export_mesh_data(meshData,mesh_dir,m_doc,sid,true , bTexMode);
    return true;  
}

bool CSkinExporter::__export_action_data()
{
    return true;
}

wstring CSkinExporter::__get_avialabale_mesh_name(wstring skin_group_dir, wstring name)
{
    wchar_t posfix[32];
    if(m_doc.open_stream( (skin_group_dir + name).c_str(),true) == NULL)
        return name;
    for( int i = 1 ; ; i++)
    {
        swprintf(posfix,L"_%d",i);
        if(m_doc.open_stream( (skin_group_dir + name + posfix).c_str(),true) == NULL)
            return name + posfix;
    }
}