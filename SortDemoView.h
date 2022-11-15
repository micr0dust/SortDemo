
// SortDemoView.h: CSortDemoView 類別的介面
//

#pragma once

#include<vector>
#include<string>

using std::vector;
using std::string;

struct SortFmt
{
	long long val;
	string str;
};


class CSortDemoView : public CView
{
protected: // 僅從序列化建立
	CSortDemoView() noexcept;
	DECLARE_DYNCREATE(CSortDemoView)

// 屬性
public:
	CSortDemoDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CSortDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDemoBubblesort();
	afx_msg void OnDemoSelectionsort();
	afx_msg void OnDemoShellsort();
	afx_msg void OnDemoInsertionsort();
	afx_msg void CSortDemoView::OnDemoQuicksort();
	afx_msg void OnDemoUnknownsort();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnQuery();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWrite();
	afx_msg void OnDemoCountingsort();
	afx_msg void OnDemoMergesort();
	afx_msg void OnDemoStraightradixsort();
	afx_msg void OnDemoRadixexangesort();

	vector<string> multiSplit(string str, char* delim);
	vector<SortFmt> initialize(CString input, string delimStr, int mode = 0);
	void printData(vector<SortFmt> sortData);
	void printShellData(vector<SortFmt> sortData, int h, bool data = true);
	void printBitsData(vector<SortFmt> sortData, int bitsize = 8);
	vector<SortFmt> CSortDemoView::quickSortFn(vector<SortFmt> data, int left, int right);
	void mergeSort(vector<SortFmt>& data, int front, int end);
	void merge(vector<SortFmt>& data, int front, int mid, int end);
	vector<SortFmt> RadixExchangeFn(vector<SortFmt> sortData, int left, int right, int b);
	int bits(int x, int shift, int bit);

	CString original;
	string query;
	vector<string> splited;
	vector<string> finalResult;
	bool inputChange;
	RECT screen;
	string sortDelim;
	CPoint BeginPos;
	CPoint originPos;
	CPoint currentPos;
	bool writeToFile;
	bool dragging;
};

#ifndef _DEBUG  // 對 SortDemoView.cpp 中的版本進行偵錯
inline CSortDemoDoc* CSortDemoView::GetDocument() const
   { return reinterpret_cast<CSortDemoDoc*>(m_pDocument); }
#endif

