#pragma once
#include "../xMedusaEditor.h"
#include "../xMedusaEditorHelper.h"
#include "BaseLib/xIDManager.h"
USING_NS_MDED;
USING_NS_XEVOL3D;
class CMainFrame;
#define UNDO_QUEUE_MASK ( (IMEdUICommand*)NULL + 1)
#include <map>
class CMedusaMainUI : public nsMedusaEditor::IMEdMainUI
{
	//��������
public:
	typedef std::vector<nsMedusaEditor::IMEdUIElement*> vMEdUIElements;
	typedef std::map<std::wstring , CMEdUiToolBarInfo*> vMEUIToolbars;
	typedef std::vector<IMEdUIMessageListenner*>        vMsgListeners;
	typedef std::vector<IMEdUICommand*>                 vMEdUICommands;
	enum { MAX_UNDO = 1024 };

public:
	CMedusaMainUI();
	~CMedusaMainUI(); 
//��� SDK�Ľӿ�
	void               AttachUIElement(nsMedusaEditor::IMEdUIElement* pPane);
	void               DetachUIElement(nsMedusaEditor::IMEdUIElement* pPane);
	bool               DetachUIElement();

	void               _DetachUIElementImpl( nsMedusaEditor::IMEdUIElement* pPane ) ;
	void               AttachDockPane(nsMedusaEditor::IMEdDockPane* pPane);
	void               DetachDockPane(nsMedusaEditor::IMEdDockPane* pPane = NULL);
public:

	void               RegisteToolbar(CMEdUiToolBarInfo* _toolbar);
	void               UnregisteToolbar(const wchar_t* _tbName);
	CMEdUiToolBarInfo* FindToolbar(const wchar_t* _tbName);
	bool               CreateToolbar(const wchar_t* _tbName);
	bool               DestroyToolbar(const wchar_t* _tbName);
	bool               DestroyToolbar();
	bool               HideToolbar(const wchar_t* _tbName);
	bool               ShowToolbar(const wchar_t* _tbName);
	CMEdUiToolBarInfo* FindToolbarByCmdID(int cmdID);
	bool               PushMessageListenner(IMEdUIMessageListenner* pListener);
    void               PopMessageListenner();
	IMEdUIMessageListenner* GetMessageListenner();
    bool               fireMEdUIEvent(eMEUIEvent _event , int param, int param2 = 0);
	//�����Ҫ����һ������ID�������
	int                AllocUIID(int nId);
	
	IMEdUIStatusBar *  GetStatusBar();
	bool               Redo();
	bool               Undo() ;
	bool               InsertCommand(IMEdUICommand* pCommand);
	bool               CanUndo();
	bool               CanRedo();
public:
	bool               FireWindowMsg(NS_XEVOL3D::xWindowMsg& msg);
public:
	CMainFrame*              m_pMainFrame;
	vMEdUIElements           m_vElements;
	vMEUIToolbars            m_vToolbars;
	NS_XEVOL3D::xIDManager   m_IDManager;
	vMsgListeners            m_Listeners;
	IMEdUICommand*           m_CommandQueue[MAX_UNDO];
	int                      m_CmdQueueLast; //��ǰ��λ��
};

class CMedusaEditor : public nsMedusaEditor::IMedusaEditor 
{
public:
	friend class CMedusaApp;
public:
	nsMedusaEditor::IMEdMainUI* GetUI() { return &m_MainUI ; }

protected:
	void SetMainFrame(CMainFrame*   pMainFrame);
public:
	CMedusaMainUI m_MainUI;
   
};
