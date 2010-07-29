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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "DockPaneContainer.h"
#include <vector>
#include "BaseLib/xIDManager.h"
#include "../xMedusaEditor.h"
#include "RibbonBar.h"

class CMainFrame;
///Medusa����Ŀ��״̬��
class CStatusBarItem
{
public:
	enum StatusBarItemType
	{
		SBIT_Label,
		SBIT_Button,
		SBIT_Checkbok,
		SBIT_Combox,
	};
private:
	nsMedusaEditor::IMEdUICommandReciver* m_pCallBack;
	CMFCRibbonBaseElement*                m_pElement;
};
typedef ds_map(int , IMEdUIElement*) vID2UIElement;

class CStatusBarInterface : public IMEdUIStatusBar
{
public:
	IMEdUIStatusBarItem* FindItem(const wchar_t* pName);
	bool                 DeleteItem(IMEdUIStatusBarItem* pItem);
	IMEdUIStatusBarItem* InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type);
	bool                 InsertSeperator();
public:
	CMainFrame*           m_pMainFrame;

};

//////////////////////////////////////////////////////////////////////////
class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	IMEdUIStatusBarItem* FindItem(const wchar_t* pName);
	bool                 DeleteItem(IMEdUIStatusBarItem* pItem);
	IMEdUIStatusBarItem* InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type);
	bool                 InsertSeperator();
	IMEdUIStatusBar*     GetStatusBarImpl(){return &m_StatusBarImpl;}
    void                 ResetApplicationLook();
    void                 SetRibbonBar(CMFCRibbonBar* pRibbonBar);
	//CMFCRibbonBar*       GetRibbonBar();
// ����
public:
	bool         AttachToolBar(HWND hToolBar);
	bool         DetachToolBar(HWND hToolBar);
	bool         AttachDockPane(nsMedusaEditor::IMEdDockPane* pPane);
	bool         DetachDockPane(nsMedusaEditor::IMEdDockPane* pPane);

    bool         AddUIElementToShowHideMenu(IMEdUIElement* pUIElement , int id = - 1);
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonStatusBar              m_wndStatusBar;
	////
	std::vector<CDockPaneContainer*> m_DockPanes;
	NS_XEVOL3D::xIDManager           m_DockPaneIDMgr;
	CStatusBarInterface              m_StatusBarImpl;
    CMFCCaptionBar                   m_wndCaptionBar;
    vID2UIElement                    m_mapUIElment;
    CMFCRibbonPanel*                 m_pShowHidePanelView;
// ���ɵ���Ϣӳ�亯��
protected:
    afx_msg void OnViewCustomize();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnViewCaptionBar();
    afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);

    afx_msg void OnEditUndo();
    afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
    afx_msg void OnEditRedo();

	afx_msg void OnDockPaneRange(UINT id);
	afx_msg void OnUpdateDockPaneRange(CCmdUI* pCmdUI);

	afx_msg void OnPluginCmdRange(UINT id);
	afx_msg void OnUpdatePluginCmdRange(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	
};


