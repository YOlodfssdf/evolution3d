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

// Medusa.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Medusa.h"
#include "MainFrm.h"

#include "MedusaDoc.h"
#include "MedusaView.h"
#include "Splash.h"
#include "SplashScreenEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaApp

BEGIN_MESSAGE_MAP(CMedusaApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMedusaApp::OnAppAbout)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CMedusaApp::CreateFileView)
	ON_COMMAND(ID_FILE_OPEN, &CMedusaApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE   , &CMedusaApp::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMedusaApp::OnFileSaveAs)

    ON_COMMAND(ID_EDIT_UNDO, &CMedusaApp::OnEditUndo)
END_MESSAGE_MAP()


// CMedusaApp ����

CMedusaApp::CMedusaApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Medusa.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	m_AppName       = L"Medusa";
	m_WorkDir       = L"Medusa";
	m_UILayoutFile  = L"Configure/uidef.xml";
	m_UILayoutName  = L"Default";
	m_InitSceneFile = L"scene/demoScene.xml";
	m_pRenderWindow = NULL;

}

void CMedusaApp::SetWorkDir(const wchar_t* _workDir)
{
	m_WorkDir = _workDir;
}

void CMedusaApp::SetInitSceneFile(const wchar_t* sceneFile)
{
	m_InitSceneFile = sceneFile;
}

void CMedusaApp::SetAppName(const wchar_t* _appName)
{
	m_AppName = _appName;
}

void CMedusaApp::SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath)
{
	if(bRelPath == false )
		m_UILayoutFile = layoutFile;
	else
		m_UILayoutFile = _MEDUSA_RESPATH(layoutFile);
	m_UILayoutName = layoutName;
}

CEditorEnv * GetEditorEnv()
{
	return dynamic_cast<CEditorEnv *>(&theApp);
}

CEvolEnviroment* GetEvol3DEnv()
{
	return GetEditorEnv()->evol3DEnv();
}

xSceneSelection* GetSceneSelection()
{
	return GetEvol3DEnv()->GetSelection();
}

void CMedusaApp::OnFileSave()
{

	return ;
}

void CMedusaApp::OnFileSaveAs()
{

}
// CMedusaApp initialization
void CMedusaApp::CreateFileView()
{
	if(this->bInited() == false && m_pRenderWindow == NULL )
	{
		__super::OnFileNew();

		if(m_pRenderWindow)
		{
			SetRenderWindow( m_pRenderWindow->GetSafeHwnd() );
			initRenderer(NULL);
		}
		assert(m_pRenderWindow);
	}
	else
	{
		//���½���һ��Scene���൱�ڰ�ԭ����Scene����ˡ�
	}

}

void CMedusaApp::OnFileOpen()
{

}
// Ψһ��һ�� CMedusaApp ����

CMedusaApp theApp;


// CMedusaApp ��ʼ��

BOOL CMedusaApp::InitInstance()
{
	xXmlDocument uiDoc;
	if( false == uiDoc.load(m_UILayoutFile.c_str() )  )
	{
		MessageBox(GetActiveWindow() , L"Medusa Init failed : Missing UI configuration\n", L"Error" , MB_OK);
		return FALSE;
	}

	xXmlNode* pUISchema = uiDoc.root()==NULL?NULL : uiDoc.root()->findNode( m_UILayoutName.c_str() );
	if(pUISchema == NULL)
	{
		MessageBox(GetActiveWindow() , L"Medusa Init failed : Missing UI Schema\n", L"Error" , MB_OK);
		return FALSE;
	}
	uiDoc.unload();

	xI18N i18N;
	std::ds_wstring langFile  = std::ds_wstring(L"Language/") + i18N.getCurrentLocaleInfor()->_locale + L".dll";
	std::ds_wstring resDllName = _MEDUSA_RESPATH(langFile.c_str()).c_str() ;
	HINSTANCE hResDll  = ::LoadLibrary(resDllName.c_str() );
	if(hResDll == NULL)
	{
		langFile   = std::ds_wstring(L"Medusa/Language/") + i18N.getCurrentLocaleInfor()->_locale + L".dll";
		resDllName = _XEVOL_ABSPATH_(langFile.c_str()) ;
		hResDll  = ::LoadLibrary(resDllName.c_str() );
	}

	if(hResDll)
	{
		//m_hLangResourceDLL = hResDll;
		AfxSetResourceHandle(hResDll);
	}

	//��������
	CEditorEnv::init();
	m_AppCfgFile =  std::wstring(L"Configure/") + m_AppName.c_str() + L".xml";
	m_AppCfgFile = _MEDUSA_RESPATH(m_AppCfgFile.c_str() );
	m_sysConfig.load(m_AppCfgFile.c_str() );

	CSplashScreenEx Splash ;
	bool bSplash = GetEditorEnv()->sysValue(L"splash" , L"visible" , true);
	if( bSplash )
	{
		Splash.Create(NULL,L"CSplashScreenEx dynamic text:",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
		Splash.SetBitmap(_MEDUSA_RESPATH(L"splash.bmp").c_str(),255,0,255);
		Splash.SetTextFont(L"Impact",100,CSS_TEXT_NORMAL);
		Splash.SetTextRect(CRect(125,60,291,104));
		Splash.SetTextColor(RGB(255,255,255));
		Splash.SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		Splash.Show();
		Splash.SetText(TEXT("Medusa 3D Studio ......"));
	}

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey( m_AppName.c_str() );
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMedusaDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMedusaView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


    m_LocaleToolbar.Register();

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	

	//--------------------------
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����


	//��ʼ�����
	if( bSplash )
	{
		Splash.Hide();
	}
	if(m_InitSceneFile.length() > 0 )
	{
		//Now load the scene
		evol3DEnv()->loadScene( _MEDUSA_RESPATH(m_InitSceneFile.c_str()).c_str() );
	}
	m_pMainWnd->SetWindowText(m_AppName.c_str() );
	return TRUE;
}

void  CMedusaApp::LoadMedusaUI(xXmlNode* pNode , IMEdUIElement* pRootPanel , IMEdMainUI* pMainUI )
{
	xXmlNode::XmlNodes elments;
	pNode->findNode(L"Element" , elments);
	for(size_t i = 0 ;i < elments.size() ; i ++)
	{
		xXmlNode* pElNode = elments[i];
		const wchar_t* strTypeName = pElNode->value(L"type");
		IMEdUIElement* pPanel = CMEdUIElFactoryMgr::singleton()->createInstance(strTypeName);
		if(pRootPanel)
		{
			pRootPanel->AttachUIElement(pPanel);
		}
		else
		{
			pMainUI->AttachUIElement(pPanel);
		}
		LoadMedusaUI(pElNode , pPanel, pMainUI);
		pPanel->ReleaseObject();
	}
}

bool  CMedusaApp::OnRenderWindowCreate(CMedusaView* pView)
{
	  assert(m_pRenderWindow == NULL);
	  m_pRenderWindow = pView;
	  return m_pRenderWindow != NULL;
}

bool CMedusaApp::OnMainWindowCreate(CMainFrame* pMainFrame)
{

    //--------------------------
    m_MedusaEditor.SetMainFrame(  pMainFrame );

	//�������в��
	xPluginMgr::singleton()->loadPluginDir( _MEDUSA_RESPATH(L"Plugin/").c_str() );
	xPluginMgr::singleton()->loadPluginDir( _XEVOL_ABSPATH_(L"./Medusa/Plugin/") );

	xXmlDocument uiDoc;
	if(false == uiDoc.load( m_UILayoutFile.c_str()  ) )
	{
		return false;
	}

    //=======================================================
    //LoadState( (CMainFrame*)m_pMainWnd , NULL);
    //this->LoadCustomState();
    //CRect _rect ;
    //int flag;
    //int shcmd;
    //this->LoadWindowPlacement(_rect , flag , shcmd);
    //=======================================================

    xXmlNode::XmlNodes toolbars;
    xXmlNode* pTestUISchema = uiDoc.root()->findNode( m_UILayoutName.c_str() );

    pTestUISchema->findNode(L"Toolbar" , toolbars);
    for(size_t i = 0 ;i < toolbars.size() ; i ++)
    {
        xXmlNode* pTbNode = toolbars[i];
        const wchar_t* strToolbarName = pTbNode->value(L"name");
        GetMedusaEditor()->GetUI()->CreateToolbar( strToolbarName );
    }
    	
	LoadMedusaUI(pTestUISchema , NULL , GetMedusaEditor()->GetUI());

    pMainFrame->SetTitle( m_AppName.c_str() );
	return true;
}

int CMedusaApp::ExitInstance()
{

	m_MedusaEditor.GetUI()->DetachUIElement();
	int _ret = __super::ExitInstance();
	m_sysConfig.setXMLStyle(true);
	m_sysConfig.save( m_AppCfgFile.c_str() );

	// TODO: Add your specialized code here and/or call the base class	
	CEditorEnv::Uninit();


	m_MedusaEditor.GetUI()->DestroyToolbar();
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

BOOL CMedusaApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	//CEditorEnv::OnIdle();
	return __super::OnIdle(lCount);
}
// CMedusaApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMedusaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMedusaApp �Զ������/���淽��

void CMedusaApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMedusaApp::LoadCustomState()
{
}

void CMedusaApp::SaveCustomState()
{
}

// CMedusaApp ��Ϣ�������






//////////////////////////////////////////////////////////////////////////


std::wstring  _MEDUSA_RESPATH(const wchar_t* _path)    
{
	std::wstring _fullPath = std::wstring(L"./" ) + theApp.GetWorkDir() + std::wstring(L"/" ) + _path;
	std::ds_wstring _absPath = xOperationSys::singleton()->getAbsResourcePath( _fullPath.c_str() );
	std::wstring  ret = _absPath.c_str();
	return ret ;
}

extern "C"
{
	MEDUSA_API IMedusaEditor* GetMedusaEditor()
	{
		return &theApp.GetEditor();
	}
}

void CMedusaApp::OnEditUndo()
{
    // TODO: Add your command handler code here
}
