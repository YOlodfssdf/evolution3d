
// xComDocViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CxComDocViewerApp:
// �йش����ʵ�֣������ xComDocViewer.cpp
//

class CxComDocViewerApp : public CWinAppEx
{
public:
	CxComDocViewerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CxComDocViewerApp theApp;