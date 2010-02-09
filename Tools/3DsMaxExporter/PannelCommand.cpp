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
#include <Windows.h>
#include "XEvolMaxExporter.h"
#include "Resource.h"
#include "XEvolMaxExporter.h"
#include "NodeViewDlg.h"
#include "LogTextDlg.h"
#include "SkeletonSetDlg.h"
#include "./xFolderDialog/XFolderDialog.h"
#include "BaseLib/xI18N.h"
//


//�������͵�exporter.
#include "StaticModelExport.h"
#include "FrameAniModelExpoter.h"
#include "SkeletonModelExporter.h"
#include "MeshExporterDlg.h"

#include "SkeletonActionExpDlg.h"
//���еĶԻ�����Ϣ�������ﴦ��

bool On_ViewNode(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ViewSceneNode(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportStaticModel(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportFrameAniModel(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportSkeletonModel(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetBoneID(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ConfirmScene(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetBonesGroup(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetSkinGroup(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetSkeleton(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportAction(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportSkin(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportWaterMesh(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool PannelCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{    

     switch(wParam)
	 {

	 case IDC_SET_BONES_GROUP:
		 return On_SetBonesGroup(hDlg,wParam,lParam);
		 break;
	 case IDC_SET_SKIN_GROUP:
		 return On_SetSkinGroup(hDlg,wParam,lParam);
		 break;
	 case IDC_SET_SKELETON:
		 return On_SetSkeleton(hDlg,wParam,lParam);
		 break;

	 case IDB_VIEWNODE:
          return On_ViewNode(hDlg,wParam,lParam);
	 case IDC_VIEWROOTSCENE_NODE:
          return On_ViewSceneNode(hDlg,wParam,lParam);
     case IDC_EXPORT_STATICMODEL:
          return On_ExportStaticModel(hDlg,wParam,lParam);
     case IDC_EXPORT_FRAMEANI_MODEL:
         return On_ExportFrameAniModel(hDlg,wParam,lParam);
     case IDC_EXPORT_SKELETON_MODEL:
         return On_ExportSkeletonModel(hDlg,wParam,lParam);
         break;
	 case IDC_EXPORT_ACTION:
		 {
			 CMaxEnv::singleton().BeginLog();
			 bool ret =  On_ExportAction(hDlg,wParam,lParam);
			 if(ret == false)
			 {
				XEVOL_LOG(eXL_DEBUG_HIGH , L"��������ʧ��\n");
			 }
			 else
			 {
                 XEVOL_LOG(eXL_DEBUG_HIGH , L"���������ɹ�\n");
			 }
			 CMaxEnv::singleton().FinishLog();
			 return ret;
		 }
		 break;

	 case IDC_EXPORT_WATER_MESH:
		 {
			 CMaxEnv::singleton().BeginLog();
			 bool ret =  On_ExportWaterMesh(hDlg,wParam,lParam);
			 if(ret == false)
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"����ˮ��meshʧ��\r\n");
			 }
			 else
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"����ˮ��mesh�ɹ�\r\n");
			 }
			 CMaxEnv::singleton().FinishLog();
			 return ret;
		 }

	 case IDC_EXPORT_MESH:
		 {
			 CMaxEnv::singleton().BeginLog();
			 CMeshExporterDlg dlg;
			 if(dlg.m_bInited == false)
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"��������ʧ��\r\n");
			 }
			 else
			 {
				 dlg.DoModal();
			 }
			 CMaxEnv::singleton().FinishLog();
			 return true;
		 }
		 break;
	 case IDC_EXPORT_SKIN:
		 {
			 CMaxEnv::singleton().BeginLog();
			 bool ret =  On_ExportSkin(hDlg,wParam,lParam);
			 if(ret == false)
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"����Ƥ��ʧ��\r\n");
			 }
			 else
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"����Ƥ���ɹ�\r\n");
			 }
			 CMaxEnv::singleton().FinishLog();
			 return ret;
		 }
		 break;
     case IDC_SETBONEID:
         return On_SetBoneID(hDlg,wParam,lParam);
         break;
     case IDC_CONFIRMSCENE:
         return On_ConfirmScene(hDlg,wParam,lParam);
         break;
     case IDC_BUSE_BEFORESKELETON_BIND:
         {
             BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_BUSE_BEFORESKELETON_BIND);
             CMaxEnv::singleton().m_bUseBeforeSkeletonPose = (bCheck!=FALSE);
         }
         break;
     case IDC_CALCINITPOS_MYSEF:
         {
             BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_CALCINITPOS_MYSEF);
             CMaxEnv::singleton().m_bCalcInitPosMySelf = (bCheck!=FALSE);
         }
         break;
     case IDC_EXPORT_VERTEX_SHELL:
         {
             BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_EXPORT_VERTEX_SHELL);
             CMaxEnv::singleton().m_bExpVertexNormal = (bCheck!=FALSE);
         }
	 case IDC_INVERT_V_COORD:
		 {
			 BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_INVERT_V_COORD);
			 CMaxEnv::singleton().m_bInvertTexCoordV = (bCheck!=FALSE);
		 }
	 case IDC_INVERT_YZ_COORD:
		 {
			 BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_INVERT_YZ_COORD);
			 CMaxEnv::singleton().m_bInvertYZCoord = (bCheck!=FALSE);
		 }
         break;
     }
     return true;
}

bool On_SetBoneID(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    if(true == CMaxEnv::singleton().Init_scene_bone_id() )
    {
        MessageBox(GetActiveWindow(),"������BoneID�Ѿ����䣡","��ȷ",MB_OK);
    }
    else
    {
        MessageBox(GetActiveWindow(),"������BoneIDû����ȷ���䣡\n ������ĳ����е������������\
                                      \n������Ҫ�Ѳ�ͬ��Node���ó���ͬ����","����",MB_OK);
    }
    return true;
}
bool On_ConfirmScene(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    if(true == CMaxEnv::singleton().Confirm_Scene_BoneID() )
    {
        MessageBox(GetActiveWindow(),"������BoneID���Ѿ����䣡","��ȷ",MB_OK);
    }
    return true;
}

bool On_ViewNode(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    dlg.DoModal();
    dlg.ReleaseTree();
    return true;
}

bool On_ViewSceneNode(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CNodeViewDlg dlg;
    dlg.BuildRootNodeTree();
    dlg.DoModal();
    dlg.ReleaseTree();
    return true;
}


bool LoadSkeleton(CSkeletonExporter& skeletonExporter)
{
	wchar_t skeleton_file[128];
	skeleton_file[0] = 0;

	if(CMaxEnv::singleton().get_saved_filename(skeleton_file,
		L"XEvol�����ļ�(*.skeleton)\0*.skeleton\0XEvolģ���ļ�(*.xrm)\0*.xrm\0�����ļ�(*.*)\0*.*\0",
		L"skeleton",
		L"��ѡ��һ���Ǽ��ļ�") == false)
	{
		return false;
	}

	xcomdoc skeleton_doc;
	skeleton_doc.open(skeleton_file,xcdm_read);
	if(skeleton_doc.failed())
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪹Ǽ��ļ�ʧ��\r\n");
		skeleton_doc.close();
		return false;
	}
	xcomdocstream* pstream = skeleton_doc.open_stream(L"skeleton/skeleton");
	if(pstream == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪹Ǽ��ļ�ʧ��[skeleton/skeleton��������]\r\n");
		skeleton_doc.close();
		return false;
	}

	if(false == skeletonExporter.load_base_info(*pstream))
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"�ǼܷǷ�����ѡ����һ���Ǽ�\r\n");
		skeleton_doc.close();
		return false;
	}
	CMaxEnv::singleton().SaveLogToXComdoc(skeleton_doc);
    skeleton_doc.close();
	return true;
}

bool On_ExportAction(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	wchar_t skeleton_file[128];
	skeleton_file[0] = 0;
	
	XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
	XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʼ��������\r\n");
	if(CMaxEnv::singleton().get_saved_filename(skeleton_file,
		 L"XEvolģ���ļ�(*.xrm)\0*.xrm\0XEvol�����ļ�(*.skeleton)\0*.skeleton\0�����ļ�(*.*)\0*.*\0",
		 L"skeleton",
		 L"��ѡ��һ���Ǽ��ļ�") == false)
	{
          return false;
	}
    CSkeletonExporter sexporter;
	xcomdoc skeleton_doc;
	skeleton_doc.open(skeleton_file,xcdm_rw);
	if(skeleton_doc.failed())
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪹Ǽ��ļ�ʧ��\r\n");
		skeleton_doc.close();
		return false;
	}
    xcomdocstream* pstream = skeleton_doc.open_stream(L"skeleton/skeleton");
    if(pstream == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪹Ǽ��ļ�ʧ��[skeleton/skeleton��������]\r\n");
		skeleton_doc.close();
		return false;
	}
	
	if(false == sexporter.load_base_info(*pstream))
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"�ǼܷǷ�����ѡ����һ���Ǽ�\r\n");
		skeleton_doc.close();
		return false;
	}
	
	sActionInfos_t actions;
	CActionExporter action_exporter;
	action_exporter.load_action_list(actions,skeleton_doc,L"skeleton/actions.xml");
	CSkeletonActionExpDlg sAExpdlg(actions);
	sAExpdlg.DoModal();
	if(sAExpdlg.m_bExpToSingleFile == FALSE)
	{
        action_exporter.export(&sexporter,actions,skeleton_doc,L"skeleton/");
	    CMaxEnv::singleton().SaveLogToXComdoc(skeleton_doc);
	}
	else
	{
		char CurDir[1024] = {0};
		GetCurrentDirectory(1024, CurDir);
		CXFolderDialog dlg(CurDir);
		CXFolderDialog::XFILEDIALOG_OS_VERSION eVer = dlg.GetOsVersion();
		dlg.SetTitle("ѡ�񱣴涯����Ŀ¼");
		if(dlg.DoModal() == IDCANCEL)
		{
			return false;
		}

		xcomdoc dir;
		wchar_t wcsFolderName[1024] = {0};
		XEvol_LocaleToUnicode( dlg.GetPath().GetBuffer() , wcsFolderName , 1024);
		dir.open_dir(wcsFolderName);
		action_exporter.export(&sexporter,actions,dir,L"");
		dir.close();

	}

	
	skeleton_doc.close();
	return true;
}

bool On_ExportStaticModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CMaxEnv::singleton().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʼ������̬ģ��\r\n");
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    if(IDOK == dlg.DoModal() )
    {
        CStaticModelExporter exportor(dlg.getBuiledNodeTree());
        exportor.export();
        dlg.ReleaseTree();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"������̬ģ�����\r\n");
        XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
        CMaxEnv::singleton().FinishLog();
        return true;
    }
	CMaxEnv::singleton().FinishLog();
    return false;

}

bool On_ExportFrameAniModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CMaxEnv::singleton().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʼ����֡����ģ��\r\n");
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    if(IDOK == dlg.DoModal() )
    {
        CFrameAniModelExporter exportor(dlg.getBuiledNodeTree());
        exportor.export();

        dlg.ReleaseTree();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"����֡����ģ�����\r\n");
        XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
        CMaxEnv::singleton().FinishLog();
        return true;
    }
	CMaxEnv::singleton().FinishLog();
    return false;

}

bool On_ExportSkeletonModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CMaxEnv::singleton().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʼ������������ģ��\r\n");
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    if(IDOK == dlg.DoModal() )
    {
        CSkeletonModelExporter exportor(dlg.getBuiledNodeTree());
        exportor.export();

        dlg.ReleaseTree();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"������������ģ�����\r\n");
        XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
        CMaxEnv::singleton().FinishLog();
        return true;
    }
	CMaxEnv::singleton().FinishLog();
    return false;
}

bool On_SetSkeleton(HWND hDlg,WPARAM wParam,LPARAM lParam)
{  
    CMaxEnv::m_BoneGroupDlg.DestroyContex();
	CMaxEnv::m_BoneGroupDlg.BuildContex();

	sMaxNodes_t bones;
	CMaxEnv::m_BoneGroupDlg.GetBones(bones);

	CSkeletonSetDlg SSDlg(1.0f);
	SSDlg.BuildNodeTree(bones);
	SSDlg.DoModal();
	return true;
}

bool On_SetBonesGroup(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	if( ! IsWindow(CMaxEnv::m_BoneGroupDlg.GetSafeHwnd()) )
	{
		CMaxEnv::m_BoneGroupDlg.Create(CSetBoneGroupDlg::IDD);
		CMaxEnv::m_BoneGroupDlg.hide();
	}

	
	if(IsWindowVisible(CMaxEnv::m_BoneGroupDlg.GetSafeHwnd()))
	{
        CMaxEnv::m_BoneGroupDlg.hide();
	}
	else
	{
        CMaxEnv::m_BoneGroupDlg.show();
	}
	return true;
}

bool On_SetSkinGroup(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	if( ! IsWindow(CMaxEnv::m_SkinGroupDlg.GetSafeHwnd()) )
	{
		CMaxEnv::m_SkinGroupDlg.Create(CSetSkinGroupDlg::IDD);
		CMaxEnv::m_SkinGroupDlg.hide();
	}


	if(IsWindowVisible(CMaxEnv::m_SkinGroupDlg.GetSafeHwnd()))
	{
		CMaxEnv::m_SkinGroupDlg.hide();
	}
	else
	{
		CMaxEnv::m_SkinGroupDlg.show();
	}
	return true;
}
