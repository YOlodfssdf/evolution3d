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
#include "fs/xFileSystem.h"
#include "BaseLib/xI18N.h"
#include <Userenv.h>
#pragma comment(lib,"Userenv.lib")
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
bool On_ExportActionBatch(HWND hDlg,WPARAM wParam,LPARAM lParam);
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

     case IDC_EXPORT_ACTION_BACTH:
         {
             CMaxEnv::singleton().BeginLog();
             On_ExportActionBatch(hDlg,wParam,lParam);
             CMaxEnv::singleton().FinishLog();
             return true;
         }

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
    dlg.m_bSelected = TRUE;
    dlg.BuildSelNodeTree();
    dlg.DoModal();
    dlg.ReleaseTree();
    return true;
}

bool On_ViewSceneNode(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CNodeViewDlg dlg;
    dlg.m_bSelected = TRUE;
    dlg.BuildRootNodeTree();
    dlg.DoModal();
    dlg.ReleaseTree();
    return true;
}


bool LoadSkeleton(CSkeletonExporter& skeletonExporter)
{
	wchar_t skeleton_file[128];
	skeleton_file[0] = 0;

	if(CMaxEnv::singleton().get_saved_filename(skeleton_file,L"XEvol�����ļ�(*.skeleton)\0*.skeleton\0XEvolģ���ļ�(*.xrm)\0*.xrm\0�����ļ�(*.*)\0*.*\0",L"skeleton",L"��ѡ��һ���Ǽ��ļ�" , false) == false)
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
	if(CMaxEnv::singleton().get_saved_filename(skeleton_file, L"XEvolģ���ļ�(*.xrm)\0*.xrm\0XEvol�����ļ�(*.skeleton)\0*.skeleton\0�����ļ�(*.*)\0*.*\0", L"skeleton",	 L"��ѡ��һ���Ǽ��ļ�" , false) == false)
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
	sActionInfos_t NewActions ;
	CActionExporter action_exporter;
	action_exporter.load_action_list(actions,skeleton_doc,L"skeleton/actions.xml");
	CSkeletonActionExpDlg sAExpdlg(actions);
	sAExpdlg.DoModal();
	sAExpdlg.GetOperatedAction(NewActions);
	if(sAExpdlg.m_bExpToSingleFile == FALSE)
	{
        action_exporter.export(&sexporter,NewActions,skeleton_doc,L"skeleton/");
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
		action_exporter.export(&sexporter,NewActions,dir,L"");
		dir.close();

	}

	
	skeleton_doc.close();
	return true;
}


int On_ExportActionBatchFile(std::wstring action_list , const char* fileName , xcomdocstream* pSkeletonStream ,const wchar_t* export_dir , xcomdoc&  skeleton_doc , vActionXMLInfos&     vAllActionList)
{
    XEVOL_LOG(eXL_DEBUG_HIGH , L"...................................\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , "���ļ�:%s\r\n" , fileName );
    CMaxEnv::singleton().m_pInterace->FileReset(TRUE);
    CMaxEnv::singleton().m_pInterace->LoadFromFile( fileName  );
    CMaxEnv::singleton().Init_scene_bone_id();

    int nExported = 0;
    CSkeletonExporter sexporter;
    pSkeletonStream->stream_seekr(xcdsd_beg , 0 );
    if(false == sexporter.load_base_info(*pSkeletonStream))
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"�ǼܷǷ�����ѡ����һ���Ǽ�\r\n");
        return 0;
    }

    //���´�ActionList;
    sActionInfos_t       vActionList;
    CSkeletonActionExpDlg dlg(vActionList,NULL);
    dlg.LoadActionList(action_list.c_str() , TRUE);   
    if(vActionList.size() == 0 )
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"�����б���û�и�Max�ļ��Ķ���\r\n");
    }
    else
    {
        CActionExporter action_exporter;
        action_exporter.load_action_list(vActionList,skeleton_doc,L"skeleton/actions.xml");
        xcomdoc dir;
        if(false == dir.open_dir(export_dir) )
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪵���Ŀ¼ʧ��\r\n");
            return 0;
        }
        action_exporter.export(&sexporter,vActionList,dir,L"");
        dir.close();
        nExported += vActionList.size();
        Remove(vAllActionList , dlg.m_ActionXmlList );
    }

    XEVOL_LOG(eXL_DEBUG_HIGH , "�ļ��������: %d������ %s\r\n" ,vActionList.size(), fileName );
    XEVOL_LOG(eXL_DEBUG_HIGH , L"...................................\r\n");
    return nExported;
}
void FileMatchFile(const wchar_t* FileName , std::vector<string>& vFileList , std::vector<string>& vFileMatch);
bool On_ExportActionBatch(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    wchar_t skeleton_file[1024] = {0};
    std::wstring action_list = L"";
    wchar_t export_dir[1024] = {0};
    xcomdocstream* pSkeletonStream = NULL;
    xcomdoc skeleton_doc;
    std::vector<string> vFileList;

    vActionXMLInfos     vAllActionList;

    //ѡ��Ǽ��ļ�
    {
        if(CMaxEnv::singleton().get_saved_filename(skeleton_file, L"XEvolģ���ļ�(*.xrm)\0*.xrm\0XEvol�����ļ�(*.skeleton)\0*.skeleton\0�����ļ�(*.*)\0*.*\0",L"skeleton", L"��ѡ��һ���Ǽ��ļ�" , false) == false)
        {
            return false;
        }
        skeleton_doc.open(skeleton_file,xcdm_rw);
        if(skeleton_doc.failed())
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪹Ǽ��ļ�ʧ��\r\n");
            skeleton_doc.close();
            return false;
        }
        pSkeletonStream = skeleton_doc.open_stream(L"skeleton/skeleton");
        if(pSkeletonStream == false)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"�򿪹Ǽ��ļ�ʧ��[skeleton/skeleton��������]\r\n");
            skeleton_doc.close();
            return false;
        }
    }
    
    //ѡ��xml�ļ��������б�
    {
        sActionInfos_t       vActionList;
        CSkeletonActionExpDlg dlg(vActionList,NULL);
        dlg.LoadActionList(FALSE);
        action_list = dlg.ActionListFile();
        if(vActionList.size() == 0)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"{WARNING: �����б���û�ж���\r\n");
            return false;
        }
        vAllActionList = dlg.m_ActionXmlList;
    }
    

    //ѡ�񵼳���Ŀ¼.
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
        XEvol_LocaleToUnicode( dlg.GetPath().GetBuffer() , export_dir , 1024);
        XEVOL_LOG(eXL_DEBUG_HIGH , L"ѡ�񵼳���Ŀ¼: %s\r\n" , export_dir);
    }

    XEVOL_LOG(eXL_DEBUG_HIGH , L"\r\nѡ���ļ�\r\n" , export_dir);
    //ѡ�����ļ�
    {
        while(1)
        {
            OPENFILENAMEA ofn;
            char* szOpenFileNames = new char[1024 * 1024];
            char* p;
            int nLen = 0;
            ZeroMemory( &ofn, sizeof(ofn) );
            ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
            ofn.lStructSize = sizeof(ofn);
            ofn.lpstrFile = szOpenFileNames;
            ofn.nMaxFile = 1024 * 1024;
            ofn.lpstrFile[0] = '\0';
            ofn.lpstrFilter = "3ds Maxģ���ļ�(*.max)\0*.max\0All Files(*.*)\0*.*\0";
            if( GetOpenFileNameA( &ofn ) )
            {  
                char szPath[1024] = {0};
                //�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
                //���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'\'
                //���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
                lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
                //��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
                //���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
                szPath[ ofn.nFileOffset ] = '\0';
                nLen = lstrlen(szPath);

                if( szPath[nLen-1] != '\\' )   //���ѡ�˶���ļ�,��������'\\'
                {
                    lstrcat(szPath, "\\");
                }
                p = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�
                while( *p )
                {   
                    char szFileName[1024] = {0};
                    lstrcat(szFileName, szPath);  //���ļ�������·��  
                    lstrcat(szFileName, p);    //�����ļ���  
                    p += lstrlen(p) +1;     //������һ���ļ�
                    vFileList.push_back(szFileName);
                    XEVOL_LOG(eXL_DEBUG_HIGH , "ѡ���ļ�: %s\r\n" , szFileName);
                }
            }
            else
            {
                break;
            }
            delete [] szOpenFileNames;
        }

        if(vFileList.size() == 0)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"{WARNING: û��ѡ��3ds Max�ļ�\r\n");
            return false;
        }
    }

    int nExported = 0;
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʼ��������\r\n");
    for(int i = 0 ; i < vFileList.size() ; i ++)
    {
          nExported += On_ExportActionBatchFile(action_list , vFileList[i].c_str() , pSkeletonStream , export_dir , skeleton_doc , vAllActionList);
          XEVOL_LOG(eXL_DEBUG_HIGH , L"��%d���ļ�������ϣ���%d------\r\n" , i + 1, vFileList.size() );
    }

    CMaxEnv::singleton().m_pInterace->FileReset(TRUE);
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , "����%d������, ʣ�� %d������δ����\r\n" ,nExported , vAllActionList.size());
    for(size_t i = 0 ; i < vAllActionList.size() ; i ++)
    {
        xActionXMLInfo& _info = vAllActionList[i];
        XEVOL_LOG(eXL_DEBUG_HIGH , L" -->Max�ļ�=%s , ����=%s\r\n" , _info.m_MaxFile.c_str() , _info.m_Name.c_str() );
    }
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");


    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʣ�µ��ļ����б�ͨ����-------------\r\n");
    std::map<std::ds_wstring , std::string> _fileNameSet;
    std::vector<string>    _fileNewList;
    for(size_t i = 0 ; i < vAllActionList.size() ; i ++)
    {
        xActionXMLInfo& _info = vAllActionList[i];
        std::vector<string> vFileMatch;
        if(_fileNameSet.find(_info.m_MaxFile) == _fileNameSet.end( )) 
        {
             FileMatchFile(_info.m_MaxFile.c_str() , vFileList, vFileMatch);
             if(vFileMatch.size() != 0 )
             {
                 XEVOL_LOG(eXL_DEBUG_HIGH ,_info.m_MaxFile.c_str());
                 XEVOL_LOG(eXL_DEBUG_HIGH ,L"ƥ����ļ���: ");
                 for(size_t k = 0 ; k < vFileMatch.size() ; k ++)
                 {
                     XEVOL_LOG(eXL_DEBUG_HIGH ,"%s" , vFileMatch[k].c_str() );
                 }
                 XEVOL_LOG(eXL_DEBUG_HIGH ,L"\n");
                 _fileNameSet[_info.m_MaxFile] = vFileMatch[0];
                 _fileNewList.push_back(vFileMatch[0]);
             }
        }

        if(_fileNameSet.find(_info.m_MaxFile) != _fileNameSet.end( )) 
        {

            wchar_t FileNameW[1024] = {0};
            std::string _FN =  _fileNameSet[_info.m_MaxFile];
            MultiByteToWideChar(CP_ACP , 0 , _FN.c_str() , _FN.length() , FileNameW , 1024 );            
            _info.m_MaxFile = xFileSystem::singleton()->getFileName(FileNameW);;
        }        
    }


    HANDLE hToken = 0;
    OpenProcessToken(GetCurrentProcess() , TOKEN_ALL_ACCESS , &hToken);
    wchar_t _usrDir [1024]={0};
    DWORD _size = 1024;
    GetUserProfileDirectoryW(hToken,_usrDir,&_size);
    wstring userDir = std::wstring(_usrDir) + L"\\" ;
    wstring newALFile = userDir + L"NewAction.xml";
    xXmlDocument _ActionXml;
    xXmlNode* pRootNode = _ActionXml.insertNode(L"���ж���");
    for(size_t i = 0 ; i < vAllActionList.size() ; i ++)
    {
        xActionXMLInfo& _info = vAllActionList[i];
        xXmlNode* pActionNode = pRootNode->insertNode(L"����");
        pActionNode->setValue(L"StartFrame" , _info.m_nFirst);
        pActionNode->setValue(L"EndFrame"   , _info.m_nLast  );
        pActionNode->setValue(L"ActionName" , _info.m_Name.c_str() );
        pActionNode->setValue(L"MaxFile"    , _info.m_MaxFile.c_str() );
        pActionNode->setValue(L"DurTime"    , _info.m_DurTime);
    }
    _ActionXml.save( newALFile.c_str() );

    std::wstring newDir = std::wstring(export_dir) + L"\\��ͨ����";
    SHCreateDirectory(NULL,newDir.c_str());
    for(int i = 0 ; i < _fileNewList.size() ; i ++)
    {
        nExported += On_ExportActionBatchFile(newALFile.c_str() , _fileNewList[i].c_str() , pSkeletonStream , newDir.c_str() , skeleton_doc , vAllActionList);
    }

    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");

    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ͨ�����--------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , "����%d������, ʣ�� %d������δ����\r\n" ,nExported , vAllActionList.size());
    for(size_t i = 0 ; i < vAllActionList.size() ; i ++)
    {
        xActionXMLInfo& _info = vAllActionList[i];
        XEVOL_LOG(eXL_DEBUG_HIGH , L" -->Max�ļ�=%s , ����=%s\r\n" , _info.m_MaxFile.c_str() , _info.m_Name.c_str() );
    }
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");

    skeleton_doc.close();
    return true;
}

bool __Find(const char* FileName , std::vector<string>& vFileList)
{
    for(size_t i = 0 ; i < vFileList.size() ; i ++)
    {
        if(vFileList[i] == std::string(FileName))
            return true;
    }
    return false;
}
template <typename T>void   fileNameWithoutExt(const char* name , T& out)
{
    for(size_t i = 0 ; i < strlen(name) ; i ++)
    {
        if(name[i] == '.') break;
        out.push_back( name[i] );
    }
}
void FileMatchFile(const wchar_t* FileNameW , std::vector<string>& vFileList , std::vector<string>& vFileMatch)
{
    char FileName[1024] = {0};
    WideCharToMultiByte(CP_ACP , 0 , FileNameW , wcslen(FileNameW) , FileName , 1024 , NULL , NULL);
    {

        std::string _NewFileNoExt = "";
        fileNameWithoutExt(FileName , _NewFileNoExt);

        std::string NewFileNoExt = _NewFileNoExt;
        //��ܵı任
        for(size_t i = 0 ; i < NewFileNoExt.length() ; i ++)
        {
            if(NewFileNoExt[i] == '-') NewFileNoExt[i] = '_';
        }
        
        for(int i = 0 ; i < vFileList.size() ; i ++)
        {
            if(vFileList[i].find(NewFileNoExt) != std::string::npos)
            {
                std::string _Fn = NewFileNoExt + ".max";
                vFileMatch.push_back( vFileList[i] );
            }
        }

        NewFileNoExt = _NewFileNoExt;
        for(size_t i = 0 ; i < NewFileNoExt.length() ; i ++)
        {
            if(NewFileNoExt[i] == '_') NewFileNoExt[i] = '-';
        }

        for(int i = 0 ; i < vFileList.size() ; i ++)
        {
            if(vFileList[i].find(NewFileNoExt) != std::string::npos)
            {
                std::string _Fn = NewFileNoExt + ".max";
                vFileMatch.push_back( vFileList[i] );
            }
        }

    }
}

bool On_ExportStaticModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CMaxEnv::singleton().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"��ʼ������̬ģ��\r\n");
    CNodeViewDlg dlg;
    dlg.m_bSelected = TRUE;
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
    dlg.m_bSelected = TRUE;
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
    dlg.m_bSelected = TRUE;
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
