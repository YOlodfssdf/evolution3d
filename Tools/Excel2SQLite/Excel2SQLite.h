
// Excel2SQLite.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SqliteDB.h"

// CExcel2SQLiteApp:
// �йش����ʵ�֣������ Excel2SQLite.cpp
//

class CExcel2SQLiteApp : public CWinAppEx
{
public:
	CExcel2SQLiteApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};
extern SqliteDB         theDataBase;
extern CExcel2SQLiteApp theApp;