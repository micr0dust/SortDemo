// CQuert.cpp: 實作檔案
//

#include "pch.h"
#include "SortDemo.h"
#include "afxdialogex.h"
#include "CQuert.h"


// CQuert 對話方塊

IMPLEMENT_DYNAMIC(CQuert, CDialogEx)

CQuert::CQuert(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUERY, pParent)
	, InputStr(_T(""))
{

}

CQuert::~CQuert()
{
}

void CQuert::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_QUERY, InputStr);
}


BEGIN_MESSAGE_MAP(CQuert, CDialogEx)
END_MESSAGE_MAP()


// CQuert 訊息處理常式
