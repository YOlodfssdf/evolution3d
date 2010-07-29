#include "stdafx.h"
#include "RibbonBar.h"


CRibbonPanel::CRibbonPanel(CRibbonCategory* pCategory , const wchar_t* _name)
{
	if(pCategory)
	{
		m_pCategory = pCategory;
		CMFCRibbonCategory* pMFCCategory = m_pCategory->RibbonCategoryPtr();
		m_pPanel = pMFCCategory->AddPanel(_name);
	}
	m_Name = _name;
}

CMFCRibbonPanel* CRibbonPanel::RibbonPtr()
{
	return m_pPanel ; 
}


//////////////////////////////////////////////////////////////////////////
CRibbonCategory::CRibbonCategory(CMFCRibbonBar* pRibbonBar , const wchar_t* _name)
{
	m_pRibbonBars = pRibbonBar;
	m_Name = _name;
	m_RibbonCategory = m_pRibbonBars->AddCategory(_name , 0 , 0);
}

CMFCRibbonCategory*  CRibbonCategory::RibbonCategoryPtr()
{
	return m_RibbonCategory;
}
CRibbonPanel* CRibbonCategory::findPanel(const wchar_t* _name)
{
	if(_name == NULL)
		return NULL;
	for(size_t i = 0 ; i < m_vPanels.size() ; i ++)
	{
		if(m_vPanels[i]->m_Name == _name)
			return m_vPanels[i];
	}
	return NULL;
}

CRibbonPanel* CRibbonCategory::createPanel(const wchar_t* _name)
{
     CRibbonPanel* pPanel = findPanel(_name);

	 if( pPanel != NULL ) 
		  return pPanel;

	 pPanel = new CRibbonPanel(this , _name);
     m_vPanels.push_back( pPanel );
	 return pPanel;
}



//////////////////////////////////////////////////////////////////////////
CRibbonCategory* CRibbonBarWrapper::findCategory(const wchar_t* _name)
{
	if(_name == NULL)
		return NULL;
	for(size_t i = 0 ; i < m_Categorys.size() ; i ++)
	{
		if(m_Categorys[i]->m_Name == _name)
			return m_Categorys[i];
	}
	return NULL;
}

CRibbonCategory* CRibbonBarWrapper::createCategory(const wchar_t* _name)
{
	CRibbonCategory* pCategory = findCategory(_name);

	if( pCategory != NULL ) 
		return pCategory;

	pCategory = new CRibbonCategory(&m_wndRibbonBar , _name );
	m_Categorys.push_back( pCategory );
	return pCategory;
}

void CRibbonBarWrapper::InitializeRibbon(CWnd* pParent)
{
	m_wndRibbonBar.Create(pParent);

	// Init main button:
	//m_MainButton.SetImage(IDB_MAIN);
	//m_MainButton.SetText(_T("\nf"));
	//m_MainButton.SetToolTipText(strTemp);

	//m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(L"Main", IDB_FILESMALL, IDB_FILELARGE);
    m_pMainPanel = new CRibbonPanel(NULL , L"Main");
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, L"�½�", 0, 0));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, L"��", 1, 1));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, L"����", 2, 2));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, L"���Ϊ", 3, 3));

	CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, L"��ӡ", 6, 6);
	pBtnPrint->SetKeys(_T("p"), _T("w"));
	pBtnPrint->AddSubItem(new CMFCRibbonLabel(L"��ӡ"));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, L"ֱ�Ӵ�ӡ", 7, 7, TRUE));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, L"Ԥ��", 8, 8, TRUE));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, L"����", 11, 11, TRUE));
	pMainPanel->Add(pBtnPrint);
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, L"�ر�", 9, 9));
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, L"�˳�", 15));


	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(L"Home",IDB_WRITESMALL, IDB_WRITELARGE);
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(L"Clippboard", m_PanelImages.ExtractIcon(27));

	CMFCRibbonButton* pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, L"ճ��", 0, 0);
	pPanelClipboard->Add(pBtnPaste);

	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, L"����", 1));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, L"����", 2));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, L"ȫѡ", -1));

	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(L"��ͼ", m_PanelImages.ExtractIcon (7));

	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, L"״̬��");
	pPanelView->Add(pBtnStatusBar);
    
	CMFCRibbonButton* pBtnCaptionBar = new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, L"������");
	pPanelView->Add(pBtnCaptionBar);


	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, L"��ʽ", -1, -1);

	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* No default command */, TRUE /* Right align */);
	pVisualStyleButton->SetToolTipText(L"��ʽѡ��");

	pVisualStyleButton->SetDescription(L"��ʽ");
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// Add quick access toolbar commands:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	lstQATCmds.AddTail(ID_FILE_PRINT_DIRECT);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}