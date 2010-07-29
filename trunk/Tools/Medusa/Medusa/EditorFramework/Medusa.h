// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// Medusa.h : Medusa Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "../Resource.h"       // main symbols
#include "../Editor/xMedusaEditorImpl.h"
#include "../Editor/EditorEnv.h"
#include <vector>
#include <string>

class CMedusaView;
// CMedusaApp:
// �йش����ʵ�֣������ Medusa.cpp
//

class CLocaleToolbas
{
	CMEdUiToolBarInfo m_LocaleToolBar[1];
public:
	CLocaleToolbas()
	{

	}

	void Register()
	{
		m_LocaleToolBar[0].m_funcCallback = NULL;
		m_LocaleToolBar[0].m_startID = -1;
		m_LocaleToolBar[0].m_hDll = AfxGetResourceHandle();
		m_LocaleToolBar[0].m_ResID = IDR_MAINFRAME_256;
		wcscpy(m_LocaleToolBar[0].m_name  , L"MainToolbar");
		wcscpy(m_LocaleToolBar[0].m_title , L"Standard Toolbar");	
		m_LocaleToolBar[0].m_OriginBtnIDs = NULL;
		m_LocaleToolBar[0].m_hToolbar = NULL;
		m_LocaleToolBar[0].m_nButton = 0;
		m_LocaleToolBar[0].SetType(CMEdUiToolBarInfo::eToolBar);
		GetMedusaEditor()->GetUI()->RegisteToolbar(m_LocaleToolBar);
	}
};

class CMedusaApp : public CWinAppEx  , public CEditorEnv
{
public:
	//�ӿ�
	void              SetAppName(const wchar_t* _appName);
	void              SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath);
	void              SetInitSceneFile(const wchar_t* sceneFile);
	void              SetWorkDir(const wchar_t* _workDir);
	std::wstring      GetWorkDir(){ return m_WorkDir; }
	CMedusaEditor&    GetEditor() { return m_MedusaEditor ; }

	BOOL              InitMedusaEditor();
	bool              OnMainWindowCreate(CMainFrame* pMainFrame);
	bool              OnRenderWindowCreate(CMedusaView* pView);
	void              LoadMedusaUI(NS_XEVOL3D::xXmlNode* pNode , nsMedusaEditor::IMEdUIElement* pRootPanel , nsMedusaEditor::IMEdMainUI* pMainUI );
public:
	CMedusaApp();


protected:
	CMedusaEditor                           m_MedusaEditor;
	std::wstring                            m_AppName;
	std::wstring                            m_AppCfgFile ;
	std::wstring                            m_WorkDir;
	std::wstring                            m_UILayoutFile;
	std::wstring                            m_UILayoutName;
	std::wstring                            m_InitSceneFile;
	CLocaleToolbas                          m_LocaleToolbar;
	CMedusaView*                            m_pRenderWindow;


// ��д
public:
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL InitInstance();
	virtual int  ExitInstance();


	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void CreateFileView();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();


	DECLARE_MESSAGE_MAP()

public:
	// MFC�Լ�������
	UINT  m_nAppLook;
    afx_msg void OnEditUndo();
};

extern CMedusaApp theApp;
std::wstring  _MEDUSA_RESPATH(const wchar_t* _path);