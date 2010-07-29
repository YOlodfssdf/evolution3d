
// Excel2SQLiteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ExcelWriter.h"
#include "BaseLib/xXmlDocument.h"
USING_NS_XEVOL3D
// CExcel2SQLiteDlg �Ի���
class CExcel2SQLiteDlg : public CDialog
{
// ����
public:
	CExcel2SQLiteDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CExcel2SQLiteDlg();
// �Ի�������
	enum { IDD = IDD_EXCEL2SQLITE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    virtual void SelectExcelSheet(int iSheet);
	void         GenerateSQL();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CComboBox      m_cbExcelSheets;
    CComboBox      m_cbExcelKey;
    CListCtrl      m_ListExcelFields;
    CComboBox      m_cbSqliteTable;
    CListCtrl      m_listExcelData;
    CEdit          m_textUpdateSql;
    CEdit          m_textInsertExcel;
	xXmlNode       m_DataField;
	xXmlNode       m_ExcelData;
	IExcelWriter*  m_pExcelWriter;
	std::wstring   m_strUpdateSQL;
	std::wstring   m_strQuerySQL;
	std::wstring   m_strInsertSQL;
    afx_msg void   OnBnClickedBtnOpenSqlite();

	void           UpdateSQliteTable() ;
	afx_msg void   OnBnClickedBtnOpenExcel();
	afx_msg void OnCbnSelchangeListExcelSheet();
	afx_msg void OnCbnSelchangeExcelKey();
	afx_msg void OnCbnSelchangeSqliteTable();
	afx_msg void OnBnClickedBtnExcute();
    afx_msg void OnBnClickedBtnGenSql();
};
