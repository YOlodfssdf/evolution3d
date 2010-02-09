// CxEvol3DViewer.cpp : Implementation of CCxEvol3DViewer
#include "stdafx.h"
#include "OperationSys/xOperationSys.h"
#include "xEvol3D/xEvol3DEnv.h"
#include "CxEvol3DViewer.h"
USING_NS_XEVOL3D;
bool CCxEvol3DViewer::InitEvol3D(HWND hWnd , bool bCreateOwnWindow)
{
	   //��ʼ��3D����
    wchar_t appname[1024] = {0};
    xOperationSys::singleton()->getModuleFileName( (HMODULE)Global_hModuleHandle , appname , 1024);
    //��ʼ������ResourceĿ¼�������ĸ�λ��
    //...���������У� ����������Ŀ¼���λ��   
    /*
          xEvol3D
              +-----trunck
                      +---bin    [����Ŀ¼]
          xGesture
              +-----resource
          ������Ŀ¼Ϊ   ../../xGesture/resource/
    */
    //�������ʹ�ø�binĿ¼һ����Ŀ¼
    m_Evol3D.init();
    if(bCreateOwnWindow)
       m_Evol3D.initRenderer(NULL , hWnd );
    else
        m_Evol3D.initRenderer(hWnd , NULL); 
    //app.loadScene(_XEVOL_ABSPATH_(L".\\WorldComposer\\scene\\demoScene.xml"));
    m_Evol3D.loadScene(_RES_ABSPATH_(L".\\Web3D\\scene.xml"));

	m_bEvol3DInited = true;
    return true;
}
HRESULT CCxEvol3DViewer::OnDraw(ATL_DRAWINFO& di)
{
	return S_OK;
}

// CCxEvol3DViewer

LRESULT CCxEvol3DViewer::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HWND hWnd = NULL;
	m_bEvol3DInited = false;
	ATL::IOleInPlaceActiveObjectImpl<CCxEvol3DViewer>::GetWindow(&hWnd);
	// TODO: Add your message handler code here and/or call default
	::MessageBox( hWnd, L"......." , L"test" , MB_OK);
	::SetTimer(hWnd , NULL , 20 , NULL);
	return 0;
}

LRESULT CCxEvol3DViewer::OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	return 0;
}

LRESULT CCxEvol3DViewer::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	static long lastTick = GetTickCount();
	long thisTickt = GetTickCount();
	long passedTime = thisTickt - lastTick;
	lastTick = thisTickt;
	if(m_bEvol3DInited == false ) 
	{
		HWND hWnd = NULL;
		ATL::IOleInPlaceActiveObjectImpl<CCxEvol3DViewer>::GetWindow(&hWnd);
		InitEvol3D(hWnd , true);
	}
	if(true)
	{
		m_Evol3D.prevUpdateFrame(passedTime);
		m_Evol3D.updateFrame(passedTime);
		m_Evol3D.postUpdateFrame(passedTime);
	}
	return 0;
}
