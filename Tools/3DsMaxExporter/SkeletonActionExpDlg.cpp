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

// SkeletonActionExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "SkeletonActionExpDlg.h"
#include ".\skeletonactionexpdlg.h"
#include "MaxEnv.h"
#include "fs/xFileSystem.h"
#include "BaseLib/xI18N.h"
USING_NS_XEVOL3D;
// CSkeletonActionExpDlg dialog

IMPLEMENT_DYNAMIC(CSkeletonActionExpDlg, CDialog)
CSkeletonActionExpDlg::CSkeletonActionExpDlg(sActionInfos_t& ActionList,CWnd* pParent /*=NULL*/)
	: CDialog(CSkeletonActionExpDlg::IDD, pParent)
    , m_iStartFrame(0)
    , m_EndFrame(0)
    , m_iDurationTime(0)
    , m_ActionList(ActionList)
	, m_bExpToSingleFile(TRUE)
    , m_bHasUI(false)
    , m_bKeyFrame(FALSE)
{
}

CSkeletonActionExpDlg::~CSkeletonActionExpDlg()
{
}

void CSkeletonActionExpDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDL_ACTION_LIST, m_ctrlActionList);
    DDX_Text(pDX, IDC_FIRST_FRAME, m_iStartFrame);
    DDX_Text(pDX, IDC_LAST_FRAME, m_EndFrame);
    DDX_Text(pDX, IDC_DUARING_TIME, m_iDurationTime);
    DDX_Check(pDX, IDC_EXPORT_TO_SIGNLE , m_bExpToSingleFile);
    DDX_Control(pDX, IDT_ACTION_NAME, m_ctrlActionName);
    DDX_Check(pDX, IDC_EXPORT_KEYFRAME, m_bKeyFrame);
}


BEGIN_MESSAGE_MAP(CSkeletonActionExpDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDL_ACTION_LIST, OnLvnItemchangedActionList)
    ON_BN_CLICKED(IDB_ACTION_ADD, OnBnClickedActionAdd)
    ON_BN_CLICKED(IDB_ACTION_REMOVE, OnBnClickedActionRemove)
    ON_BN_CLICKED(IDB_FINISH_SETUP, OnBnClickedFinishSetup)
	ON_BN_CLICKED(IDC_EXPORT_TO_SIGNLE, &CSkeletonActionExpDlg::OnBnClickedExportToSignle)
    ON_BN_CLICKED(IDB_ACTION_LOAD, &CSkeletonActionExpDlg::OnBnClickedActionLoad)
    ON_BN_CLICKED(IDB_ACTION_SAVE, &CSkeletonActionExpDlg::OnBnClickedActionSave)
END_MESSAGE_MAP()


// CSkeletonActionExpDlg message handlers

void CSkeletonActionExpDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
	m_ctrlActionList.DeleteAllItems();
	m_ActionList.clear();
	m_ActionXmlList.clear();
    OnOK();
}

void CSkeletonActionExpDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
	m_ctrlActionList.DeleteAllItems();
	m_ActionList.clear();
	m_ActionXmlList.clear();
    OnCancel();
}
int CSkeletonActionExpDlg::findAction(const wchar_t* actionName)
{
    for(size_t i = 0 ; i < m_ActionList.size() ; ++i)
    {
        sActionInfo_t& action = m_ActionList[i];
        if(std::wstring(actionName) == action.m_Name)
        {
            return (int)i;
        }
    }
    return -1;
}

void CSkeletonActionExpDlg::RemoveAction(int index)
{
    sActionInfos_t::iterator i = m_ActionList.begin();
    for(int ii = 0  ; i != m_ActionList.end() ; ++ii , ++i)
    {
        if(ii == index )
        {
            m_ActionList.erase(i);
            m_ctrlActionList.DeleteItem(index);
            return ;
        }
    }
}
BOOL CSkeletonActionExpDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_ctrlActionList.InsertColumn(0,"序号",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(1,"名字",LVCFMT_LEFT,130);
    m_ctrlActionList.InsertColumn(2,"时间",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(3,"第一帧",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(4,"最后帧",LVCFMT_LEFT,30);

	m_bHasUI = true;
	for(size_t i = 0 ; i < m_ActionList.size() ; ++i)
	{
		InsertActionToUI(m_ActionList[i]);
	}
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

bool CSkeletonActionExpDlg::InsertActionToUI(sActionInfo_t& action)
{
	int index = findAction(action.m_Name);

	if(index != -1 && !m_bHasUI )
	{
		return false;
	}
	char buf[23];
	sprintf(buf,"%d",index);
	m_ctrlActionList.InsertItem(index,buf);
	m_ctrlActionList.SetItemText(index,1,ToAnsi(action.m_Name).c_str() );

	sprintf(buf,"%d",action.m_lTime);
	m_ctrlActionList.SetItemText(index,2,buf);

	sprintf(buf,"%d",action.m_iFirstFrame);
	m_ctrlActionList.SetItemText(index,3,buf);

	sprintf(buf,"%d",action.m_iLastFrame);
	m_ctrlActionList.SetItemText(index,4,buf);
	return true;
}


void CSkeletonActionExpDlg::GetOperatedAction(sActionInfos_t& ActionList)
{
	for(size_t i = 0 ; i < m_ActionList.size() ; i ++)
	{
		std::wstring _actName = m_ActionList[i].m_Name;
		if( m_OperatedActions.find(_actName) != m_OperatedActions.end() )
		{
			ActionList.push_back( m_ActionList[i] );
		}
	}
}


bool CSkeletonActionExpDlg::InsertAction(sActionInfo_t& action,bool isInsertToUI)
{
    int index = findAction(action.m_Name);

    isInsertToUI = m_bHasUI && isInsertToUI;
    if(index != -1 && isInsertToUI == false)
    {
        //::MessageBoxA(::GetActiveWindow(),"相同名字的动作存在","错误",MB_OK);
        XEVOL_LOG(eXL_ERROR_FALT , L"----------相同名字=%s 动作存在--------------\n" , action.m_Name);
        return false;
    }
    if(index == -1)
    {
       index = (int)m_ActionList.size();
       m_ActionList.push_back(action);
    }

	if(isInsertToUI)
	{
		InsertActionToUI(action);
	}

	m_OperatedActions.insert( action.m_Name );
    return true;
}
void CSkeletonActionExpDlg::OnLvnItemchangedActionList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
int  CSkeletonActionExpDlg::GetActionType()
{
    if(m_bKeyFrame) 
        return eActType_Keyframe;
    return eActType_Skeleton;
}
void CSkeletonActionExpDlg::OnBnClickedActionAdd()
{
    // TODO: Add your control notification handler code here
    sActionInfo_t action;
    UpdateData();
    action.m_ActionType  = GetActionType();
    action.m_iFirstFrame = m_iStartFrame;
    action.m_iLastFrame  = m_EndFrame;
    action.m_lTime       = m_iDurationTime;
    if(m_iDurationTime == 0)
    {
        int tickt_per_frame = GetTicksPerFrame();
        int iMaxTime = (m_EndFrame - m_iStartFrame + 1 ) * tickt_per_frame;
        float vTime = CMaxEnv::singleton().TicksToSeconds(iMaxTime);
        action.m_lTime       =  vTime * 1000;
        m_iDurationTime = action.m_lTime;
    }
	WGetWndText(&m_ctrlActionName , action.m_Name,32);

    if(wcslen(action.m_Name) == 0)
    {
        ::MessageBoxA(::GetActiveWindow(),"请填写动作名字","错误",MB_OK);
        return ;
    }
    if(m_iDurationTime <= 0)
    {
        ::MessageBoxA(::GetActiveWindow(),"动作时间不能为0","错误",MB_OK);
        return ;
    }
    InsertAction(action, true); 
    
}
void CSkeletonActionExpDlg::LoadActionList(const wchar_t* fileName , bool FilterByFile)
{
    xXmlDocument doc;
    if(doc.load( fileName , true ) == false)
        return ;

    m_ActionXmlList.clear();
    xXmlNode* pNode = doc.root();
    if(pNode == NULL)
        return ;

    CStr MaxFile = CMaxEnv::singleton().m_pInterace->GetCurFileName();
    wchar_t wcsMaxFileName [1024] = {0};
    XEvol_LocaleToUnicode(MaxFile.data() , wcsMaxFileName , 1024);
    std::wstring maxFileName = xFileSystem::singleton()->getFileName(wcsMaxFileName).c_str();


    xXmlNode::XmlNodes nodes;
    pNode->findNode(L"动作" , nodes);
    size_t nAction = nodes.size();
    for(size_t i = 0 ; i < nAction ; i ++)
    {
        xXmlNode* pActNode = nodes[i];
        if(pActNode->value(L"MaxFile") == NULL || pActNode->value(L"MaxFile") == maxFileName || FilterByFile == false )
        {
            sActionInfo_t action;
            action.m_ActionType  = GetActionType();
            action.m_iFirstFrame = pActNode->int_value(L"StartFrame");
            action.m_iLastFrame  = pActNode->int_value(L"EndFrame");
            if(action.m_iFirstFrame > action.m_iLastFrame )
            {
                int _v = action.m_iFirstFrame;
                action.m_iFirstFrame = action.m_iLastFrame;
                action.m_iLastFrame = _v;
            }
            action.m_lTime       = pActNode->int_value(L"DurTime");
            wcsncpy(action.m_Name  , pActNode->value(L"ActionName") , 32);
            action.m_nFrame      = action.m_iLastFrame - action.m_iFirstFrame + 1;
            if(action.m_lTime == 0)
            {
                int tickt_per_frame = GetTicksPerFrame();
                int iMaxTime = (action.m_iLastFrame - action.m_iFirstFrame + 1 ) * tickt_per_frame;
                float vTime = CMaxEnv::singleton().TicksToSeconds(iMaxTime);
                action.m_lTime       =  vTime * 1000;
            }
            InsertAction(action , true); 

            xActionXMLInfo _info;
            _info.m_MaxFile = pActNode->value(L"MaxFile") ? pActNode->value(L"MaxFile") : maxFileName.c_str();
            _info.m_Name    = action.m_Name;
            _info.m_nFirst  = pActNode->int_value(L"StartFrame");
            _info.m_nLast   = pActNode->int_value(L"EndFrame");
            _info.m_DurTime = 0;
            if(pActNode->value(L"DurTime") )
            {
                _info.m_DurTime = pActNode->int_value(L"DurTime");
            }
            m_ActionXmlList.push_back(_info);
        }
    }
    return ;
}
void CSkeletonActionExpDlg::LoadActionList(bool FilterByFile)
{
    m_XmlFile[0] = 0;
    OPENFILENAMEW ofn; 
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = ::GetActiveWindow();
    ofn.lpstrFile = m_XmlFile;
    ofn.nMaxFile = 1024;
    ofn.lpstrFilter = L"动作列表(*.xml)\0*.xml\0所有文件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrDefExt = L"xml";
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileNameW(&ofn) == FALSE)
    {
        return;
    }
    return LoadActionList(m_XmlFile , FilterByFile);
}
void CSkeletonActionExpDlg::OnBnClickedActionLoad()
{
     return LoadActionList(true);
}

void CSkeletonActionExpDlg::OnBnClickedActionRemove()
{
    POSITION pos = m_ctrlActionList.GetFirstSelectedItemPosition();
    int index = m_ctrlActionList.GetNextSelectedItem(pos);
    if(index >= 0 && index < (int)m_ActionList.size())
    {
        RemoveAction(index);
    }
}

void CSkeletonActionExpDlg::OnBnClickedFinishSetup()
{
	UpdateData();
    // TODO: Add your control notification handler code here
    OnOK();
}

void CSkeletonActionExpDlg::OnBnClickedExportToSignle()
{
	// TODO: Add your control notification handler code here
}

void CSkeletonActionExpDlg::OnBnClickedActionSave()
{
    // TODO: Add your control notification handler code here
    CFileDialog dlg(FALSE , 0 , 0 ,4|2 , TEXT("Action List(*.xml)|*.xml|AllFile(*.*)|*.*||") , this );
    if(dlg.DoModal() != IDOK)
        return ;
    wchar_t file_name[1024] = {0};
    XEvol_LocaleToUnicode(dlg.m_ofn.lpstrFile, file_name , 1024);


    CStr MaxFile = CMaxEnv::singleton().m_pInterace->GetCurFileName();
    wchar_t wcsMaxFileName [1024] = {0};
    XEvol_LocaleToUnicode(MaxFile.data() , wcsMaxFileName , 1024);
    std::wstring maxFileName = xFileSystem::singleton()->getFileName(wcsMaxFileName).c_str();

    xXmlDocument doc;
    xXmlNode* pRoot = doc.insertNode(L"动作列表");
    for(int i = 0 ; i < (int)m_ActionList.size() ; i ++)
    {
        sActionInfo_t& ActionInfo = m_ActionList[i];
        xXmlNode* pActNode = pRoot->insertNode(L"动作");
        pActNode->setValue(L"StartFrame" , ActionInfo.m_iFirstFrame);
        pActNode->setValue(L"EndFrame"   , ActionInfo.m_iLastFrame);
        pActNode->setValue(L"DurTime"    , ActionInfo.m_lTime);
        pActNode->setValue(L"MaxFile"    , maxFileName.c_str() );
    }
    doc.save(file_name , true);
}
