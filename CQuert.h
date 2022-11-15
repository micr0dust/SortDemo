#pragma once
#include "afxdialogex.h"


// CQuert 對話方塊

class CQuert : public CDialogEx
{
	DECLARE_DYNAMIC(CQuert)

public:
	CQuert(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~CQuert();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	CString InputStr;
};
