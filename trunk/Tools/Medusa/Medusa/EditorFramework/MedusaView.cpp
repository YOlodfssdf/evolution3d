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

// MedusaView.cpp : CMedusaView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Medusa.h"
#endif

#include "MedusaDoc.h"
#include "MedusaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaView

IMPLEMENT_DYNCREATE(CMedusaView, CView)

BEGIN_MESSAGE_MAP(CMedusaView, CView)
	ON_WM_CREATE()
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMedusaView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMedusaView ����/����

CMedusaView::CMedusaView()
{
	// TODO: �ڴ˴���ӹ������

}

CMedusaView::~CMedusaView()
{
}

BOOL CMedusaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}
int CMedusaView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	theApp.OnRenderWindowCreate(this);

}
// CMedusaView ����

void CMedusaView::OnDraw(CDC* /*pDC*/)
{
	
	// TODO: �ڴ˴�Ϊ������������ƴ���
}


// CMedusaView ��ӡ


void CMedusaView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMedusaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMedusaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMedusaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMedusaView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMedusaView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMedusaView ���

#ifdef _DEBUG
void CMedusaView::AssertValid() const
{
	CView::AssertValid();
}

void CMedusaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMedusaDoc* CMedusaView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMedusaDoc)));
	return (CMedusaDoc*)m_pDocument;
}
#endif //_DEBUG


// CMedusaView ��Ϣ�������
BEGIN_NAMESPACE_XEVOL3D;
bool ConvertToXEvolMsg(xWindowMsg& WinMsg , HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
END_NAMESPACE_XEVOL3D;

BOOL CMedusaView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	NS_XEVOL3D::xWindowMsg msg;
	ConvertToXEvolMsg(msg , GetSafeHwnd() , message , wParam , lParam);
	if( (msg.MsgID >= WIN_KEYBOARD_MESSAGE_BEGIN && msg.MsgID < WIN_KEYBOARD_MESSAGE_END) || (msg.MsgID >= WIN_MOUSE_MESSAGE_BEGIN && msg.MsgID < WIN_MOUSE_MESSAGE_END)             )
	{

		CMedusaMainUI* pUI = dynamic_cast<CMedusaMainUI*>( GetMedusaEditor()->GetUI() );
		if( pUI->FireWindowMsg(msg) == false)
		{
			GetEditorEnv()->onMessage(msg);
		}
	}
	else 
	{
		GetEditorEnv()->onMessage(msg);		
	}
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}