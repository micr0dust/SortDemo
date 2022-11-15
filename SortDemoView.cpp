
// SortDemoView.cpp: CSortDemoView 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "SortDemo.h"
#endif

#include "SortDemoDoc.h"
#include "SortDemoView.h"
#include "CInput.h"
#include "CQuert.h"
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<bitset>

using std::vector;
using std::string;
using std::stringstream;
using std::bitset;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSortDemoView

IMPLEMENT_DYNCREATE(CSortDemoView, CView)

BEGIN_MESSAGE_MAP(CSortDemoView, CView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DEMO_BUBBLESORT, &CSortDemoView::OnDemoBubblesort)
	ON_COMMAND(ID_DEMO_SELECTIONSORT, &CSortDemoView::OnDemoSelectionsort)
	ON_COMMAND(ID_DEMO_SHELLSORT, &CSortDemoView::OnDemoShellsort)
	ON_COMMAND(ID_DEMO_INSERTIONSORT, &CSortDemoView::OnDemoInsertionsort)
	ON_COMMAND(ID_DEMO_QUICKSORT, &CSortDemoView::OnDemoQuicksort)
	ON_COMMAND(ID_DEMO_32776, &CSortDemoView::OnDemoUnknownsort)
	ON_COMMAND(ID_DEMO_COUNTINGSORT, &CSortDemoView::OnDemoCountingsort)
	ON_COMMAND(ID_DEMO_MERGESORT, &CSortDemoView::OnDemoMergesort)
	ON_COMMAND(ID_DEMO_STRAIGHTRADIXSORT, &CSortDemoView::OnDemoStraightradixsort)
	ON_COMMAND(ID_DEMO_RADIXEXCHANGESORT, &CSortDemoView::OnDemoRadixexangesort)
	ON_WM_MOUSEWHEEL()
//	ON_WM_ICONERASEBKGND()
ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(QUERY, &CSortDemoView::OnQuery)
ON_WM_KEYDOWN()
ON_COMMAND(ID_WRITE, &CSortDemoView::OnWrite)
END_MESSAGE_MAP()

// CSortDemoView 建構/解構

CSortDemoView::CSortDemoView() noexcept
{
	// TODO: 在此加入建構程式碼

	original = _T("");
	originPos.x = 0;
	originPos.y = 0;
	currentPos.x = 0;
	currentPos.y = 0;
	dragging = false;
	inputChange = false;
	writeToFile = false;
}

CSortDemoView::~CSortDemoView()
{
}

BOOL CSortDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CSortDemoView 繪圖

void CSortDemoView::OnDraw(CDC* /*pDC*/)
{
	CSortDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 雙緩衝繪圖前置
	CRect rc;
	GetClientRect(rc);
	int nWidth = rc.Width();
	int nHeight = rc.Height();

	CDC* pDC = GetDC();
	CDC MemDC, MemBG;
	CBitmap MemBitmap;

	MemDC.CreateCompatibleDC(pDC);
	MemBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
	CBitmap* pOldbitmap = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));
	CWnd* pwndParent = this->GetParent();
	pwndParent->GetClientRect(&screen);


	// 繪製 finalResult 到記憶體
	CT2CA toString(original);
	string inputStr(toString);
	std::ofstream out;
	if (writeToFile) {
		out.open("out.txt");
		out << inputStr << '\n';
	}
	
	if (query.length()) {
		if (inputStr[inputStr.length() - 1] == ';')
			inputStr[inputStr.length() - 1] = ' ';
		finalResult.insert(finalResult.begin(), inputStr);
		currentPos.y -= 20;
		originPos.y -= 20;
	}
	for (int i = 0; i < finalResult.size(); i++) {
		// 關鍵字查找及渲染
		if (query.length()) {
			string toFind = query;
			string tmpResult = finalResult[i];

			struct highLightStr
			{
				string str;
				bool highLight;
			};
			vector<highLightStr> strContent;
			for (size_t j = 0; true; j = 0)
			{
				j = tmpResult.find(toFind, j);
				if (j == string::npos) {
					strContent.push_back({ tmpResult , false });
					break;
				}
				strContent.push_back({ tmpResult.substr(0, j) , false });
				strContent.push_back({ tmpResult.substr(j, toFind.size()) , true });
				tmpResult = tmpResult.substr(j + toFind.size());
			}

			int deltx = 0;
			int delty = 0;

			for (size_t j = 0; j < strContent.size(); j++)
			{
				RECT rectTxt = {};
				DrawTextA(MemDC, strContent[j].str.c_str(), -1, &rectTxt, DT_CALCRECT);
				if (strContent[j].highLight) {
					MemDC.SetBkColor(RGB(0, 0, 128));
					MemDC.SetTextColor(RGB(128, 255, 255));
				}
				else {
					MemDC.SetBkColor(RGB(255, 255, 255));
					MemDC.SetTextColor(RGB(0, 0, 0));
				}
				TextOutA(MemDC, currentPos.x+deltx, currentPos.y + (i + 1) * 20 + delty, strContent[j].str.c_str(), strContent[j].str.size());
				deltx += rectTxt.right;
			}
		}
		else {
			MemDC.TextOut(currentPos.x, currentPos.y, original);
			CString cs(finalResult[i].c_str());
			MemDC.TextOut(currentPos.x, currentPos.y+(i + 1) * 20, cs);
		}
		if (writeToFile)
			out << finalResult[i] << '\n';
	}

	// 繪圖
	pDC->BitBlt(0,0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldbitmap);
	MemBitmap.DeleteObject();
	ReleaseDC(pDC);

	if (query.length()) {
		finalResult.erase(finalResult.begin());
		currentPos.y += 20;
		originPos.y += 20;
	}
	else query = "";
	if (writeToFile) {
		out.close();
		writeToFile = false;
	}
}


// CSortDemoView 列印

BOOL CSortDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CSortDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CSortDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}


// CSortDemoView 診斷

#ifdef _DEBUG
void CSortDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CSortDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSortDemoDoc* CSortDemoView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSortDemoDoc)));
	return (CSortDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CSortDemoView 訊息處理常式


void CSortDemoView::OnDemoBubblesort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ";");

	bool flag;
	do
	{
		flag = false;
		for (int i = 1; i < sortData.size(); i++)
			if (sortData[i - 1].val > sortData[i].val)
			{
				SortFmt tmp = sortData[i];
				sortData[i] = sortData[i - 1];
				sortData[i - 1] = tmp;
				flag = true;
				printData(sortData);
			}
	} while (flag);
	Invalidate();
}


void CSortDemoView::OnDemoSelectionsort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",");

	for (int i = 0; i < sortData.size() - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < sortData.size(); j++)
			if (sortData[j].val < sortData[min].val)
				min = j;
		if (sortData[min].val != sortData[i].val) {
			SortFmt tmp = sortData[i];
			sortData[i] = sortData[min];
			sortData[min] = tmp;
			printData(sortData);
		}
	}
	Invalidate();
}


void CSortDemoView::OnDemoShellsort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",");
	original = "";

	size_t LEN = sortData.size();
	int h = 1;
	do
	{
		h = h * 3 + 1;
	} while (h <= LEN);

	do
	{
		h /= 3;
		printShellData(sortData, h, false);
		for (int i = h; i < LEN; i++)
		{
			SortFmt tmp = sortData[i];
			int pos = i;
			if (tmp.val < sortData[pos - h].val) {
				for (; (pos-h) >= 0 && sortData[pos-h].val > tmp.val; pos -= h)
					sortData[pos] = sortData[pos-h];
				sortData[pos] = tmp;
			}
		}
		printShellData(sortData, h);
	} while (h > 1);
	Invalidate();
}


void CSortDemoView::OnDemoInsertionsort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 1);

	for (int i = 1; i < sortData.size(); i++)
	{
		SortFmt tmp = sortData[i];
		int pos = i - 1;
		for (; pos >= 0 && sortData[pos].val > tmp.val; pos--)
			sortData[pos + 1] = sortData[pos];
		sortData[pos + 1] = tmp;
		printData(sortData);
	}
	Invalidate();
}


void CSortDemoView::OnDemoQuicksort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",", 2);
	if(inputChange)
		printData(sortData);
	quickSortFn(sortData, 0, sortData.size() - 1);

	Invalidate();
}

vector<SortFmt> CSortDemoView::quickSortFn(vector<SortFmt> data, int left, int right)
{
	if (left >= right)
		return data;
	SortFmt tmp;
	int i = left - 1;
	int j = right;
	SortFmt key = data[right];
	do {
		do i++;
		while (data[i].val < key.val && i < j);
		do j--;
		while (data[j].val >= key.val && i < j);
		tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
		printData(data);
	} while (i < j);
	data[j] = data[i];
	data[i] = data[right];
	data[right] = tmp;
	printData(data);

	data = quickSortFn(data, left, i - 1);
	data = quickSortFn(data, i + 1, right);
	return data;
}


void CSortDemoView::OnDemoCountingsort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",");

	const int len = sortData.size();
	const int RANGE = 26;
	vector<SortFmt> result = sortData;
	int count[RANGE] = { 0 };
	for (int i = 0; i < len; i++)
		count[sortData[i].val]++;
	for (int i = 1; i < RANGE; i++)
		count[i] = count[i - 1] + count[i];
	for (int i = len - 1; i >= 0; i--)
		result[(count[sortData[i].val]--) - 1] = sortData[i];
	printData(result);
	Invalidate();
}


void CSortDemoView::OnDemoMergesort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",");

	mergeSort(sortData, 0, sortData.size() - 1);

	Invalidate();
}

void CSortDemoView::merge(vector<SortFmt>& data, int front, int mid, int end)
{
	vector<SortFmt> LeftSub(data.begin() + front, data.begin() + mid + 1);
	vector<SortFmt> RightSub(data.begin() + mid + 1, data.begin() + end + 1);

	LeftSub.insert(LeftSub.end(), { INT64_MAX });
	RightSub.insert(RightSub.end(), { INT64_MAX });

	int left = 0, right = 0;

	for (int i = front; i <= end; i++)
		if (LeftSub[left].val <= RightSub[right].val)
		{
			data[i] = LeftSub[left];
			left++;
		}
		else
		{
			data[i] = RightSub[right];
			right++;
		}
	printData(data);
}

void CSortDemoView::mergeSort(vector<SortFmt>& data, int front, int end)
{
	if (front < end)
	{
		int mid = (front + end) / 2;
		mergeSort(data, front, mid);
		mergeSort(data, mid + 1, end);
		merge(data, front, mid, end);
	}
}


void CSortDemoView::OnDemoStraightradixsort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",");

	int b = 8; //每筆資料有 b 位數
	const int LEN = sortData.size();
	const int RANGE = 4;
	int m = 2; // 每次處理的 bits 數
	int key;
	for (int pass = 0; pass < (b / m); pass++)
	{
		int count[RANGE] = { 0 };
		vector<SortFmt> tmp = sortData;
		for (int i = 0; i < LEN; i++)
			count[key = bits((int)sortData[i].val, pass * m, m)]++;
		for (int i = 1; i < RANGE; i++)
			count[i] += count[i - 1];
		for (int i = LEN - 1; i >= 0; i--)
			tmp[(count[key = bits((int)sortData[i].val, pass * m, m)]--)-1] = sortData[i];
		sortData = tmp;
		printData(sortData);
		printBitsData(sortData);
	}
	Invalidate();
}


void CSortDemoView::OnDemoRadixexangesort()
{
	CInput inputBox;
	if (inputBox.DoModal() != IDOK) return;

	vector<SortFmt> sortData;
	sortData = initialize(inputBox.InputStr, ",");

	RadixExchangeFn(sortData, 0, sortData.size()-1, 4);

	Invalidate();
}

vector<SortFmt> CSortDemoView::RadixExchangeFn(vector<SortFmt> sortData, int left, int right, int b)
{
	if (right > left && b >= 0)
	{
		int i = left;
		int j = right;
		do
		{
			while (bits((int)sortData[i].val, b, 1) == 0 && i < j)
				i++;
			while (bits((int)sortData[j].val, b, 1) == 1 && i < j)
				j--;
			SortFmt tmp = sortData[i];
			sortData[i] = sortData[j];
			sortData[j] = tmp;
			printBitsData(sortData);
			printData(sortData);
		} while (i != j);
		if (bits((int)sortData[right].val, b, 1) == 0)
			j++;
		sortData = RadixExchangeFn(sortData, left, j - 1, b - 1);
		sortData = RadixExchangeFn(sortData, j, right, b - 1);
	}
	return sortData;
}


void CSortDemoView::OnDemoUnknownsort()
{
	// TODO: 在此加入您的命令處理常式程式碼
}


vector<string> CSortDemoView::multiSplit(string str, char* delim)
{
	vector<string> result;
	result.push_back(str);
	for (int i = 0; i < sizeof(delim); i++)
	{
		vector<string> tmp;
		for (int j = 0; j < result.size(); j++)
		{
			stringstream ss(result[j]);
			for (string token; getline(ss, token, delim[i]);)
				if (token.length())
					tmp.push_back(token);
		}
		result = tmp;
	}
	return result;
}


void CSortDemoView::printData(vector<SortFmt> sortData)
{
	string display;
	size_t LEN = sortData.size();
	for (int i = 0;i < LEN; i++)
		display += sortData[i].str + (i == LEN -1 ? "" : sortDelim);
	finalResult.push_back(display);
}

void CSortDemoView::printShellData(vector<SortFmt> sortData, int h, bool data)
{
	if (data) {
		size_t LEN = sortData.size();
		for (int i = 0; i < LEN / h+1; i++) {
			string display="";
			for (int j = i * h; j < i * h + h && j < LEN; j++)
				display += sortData[j].str + (j == LEN - 1 ? "" : sortDelim);
			if(display.length())
				finalResult.push_back(display);
		}
		finalResult.push_back("");
	}
	else {
		finalResult.push_back("h=" + std::to_string(h) + "");
		finalResult.push_back("");
		finalResult.push_back("排序前的數列：");
		finalResult.push_back("");
		printShellData(sortData, h, true);
		finalResult.push_back("排序後的數列：");
		finalResult.push_back("");
	}
}

void CSortDemoView::printBitsData(vector<SortFmt> sortData, int bitsize)
{
	string display;
	size_t LEN = sortData.size();
	for (int i = 0; i < LEN; i++)
		if(bitsize==8)
			display += bitset<8>((int)sortData[i].val).to_string() + (i == LEN - 1 ? "" : sortDelim);
		else if(bitsize == 16)
			display += bitset<16>((int)sortData[i].val).to_string() + (i == LEN - 1 ? "" : sortDelim);
		else if (bitsize == 32)
			display += bitset<32>((int)sortData[i].val).to_string() + (i == LEN - 1 ? "" : sortDelim);
		else if (bitsize == 64)
			display += bitset<64>((int)sortData[i].val).to_string() + (i == LEN - 1 ? "" : sortDelim);

	finalResult.push_back(display);
}

vector<SortFmt> CSortDemoView::initialize(CString input, string delimStr, int mode)
{
	finalResult.clear();
	query = "";
	currentPos.x = 0;
	currentPos.y = 0;
	originPos = currentPos;
	inputChange = false;
	CT2CA toString(original=input);
	string inputStr(toString);
	char delim[] = " ,;";
	sortDelim = delimStr;
	splited = multiSplit(inputStr, delim);

	vector<SortFmt> sortData;
	for (string i : splited)
		if(!mode)
			sortData.push_back({ std::stoll(i) ,i });
		else if (mode == 1)
			sortData.push_back({ (i[0] - 'A') * 1000 + (200-std::stoll(i.substr(1, 3))) ,i });
		else if (mode == 2) {
			char tmp[] = { i[0] ,i[1] };
			i[0] = i[0] >= 'a' ? i[0] + 'A' - 'a' : i[0];
			i[1] = i[1] >= 'a' ? i[1] + 'A' - 'a' : i[1];
			if (tmp[0]!= i[0]|| tmp[1]!= i[1])
				inputChange = true;
			int suit=0;
			if (i[0] == 'H')
				suit = 300;
			else if (i[0] == 'S')
				suit = 200;
			else if (i[0] == 'D')
				suit = 100;
			if (i.length()==3)
				sortData.push_back({ suit + 10, i });
			else if(i[1]<'A' )
				sortData.push_back({ suit + i[1]-'0', i});
			else if (i[1] == 'J')
				sortData.push_back({ suit + 11, i });
			else if (i[1] == 'Q')
				sortData.push_back({ suit + 12, i });
			else if (i[1] == 'K')
				sortData.push_back({ suit + 13, i });
			else if (i[1] == 'A')
				sortData.push_back({ suit + 14, i });
		}

	return sortData;
}


BOOL CSortDemoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if (nFlags == MK_CONTROL && zDelta > 0)
	{
		if (currentPos.x < INT32_MAX)
			currentPos.x += 70;
		originPos = currentPos;
		Invalidate();
	}
	else if (nFlags == MK_CONTROL && zDelta < 0)
	{
		if(currentPos.x > INT32_MIN)
			currentPos.x -= 70;
		originPos = currentPos;
		Invalidate();
	}
	else if (zDelta > 0)
	{
		if (currentPos.y < INT32_MAX)
			currentPos.y += 70;
		originPos = currentPos;
		Invalidate();
	}
	else if (zDelta < 0)
	{
		if (currentPos.y > INT32_MIN)
			currentPos.y -= 70;
		originPos = currentPos;
		Invalidate();
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CSortDemoView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	return FALSE;
}


void CSortDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	BeginPos = point;
	dragging = true;
	CView::OnLButtonDown(nFlags, point);
}


void CSortDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	dragging = false;
	originPos = currentPos;
	CView::OnLButtonUp(nFlags, point);
}


void CSortDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (dragging) {
		currentPos.x = originPos.x + point.x - BeginPos.x;
		currentPos.y = originPos.y + point.y - BeginPos.y;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


void CSortDemoView::OnQuery()
{
	CQuert inputQuery;
	if (inputQuery.DoModal() != IDOK) return;
	CT2CA toString(inputQuery.InputStr);
	string inputStr(toString);
	query = inputStr;
	Invalidate();
}


void CSortDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == 'F' || nChar == 'f') && VK_CONTROL)
		OnQuery();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSortDemoView::OnWrite()
{
	writeToFile = true;
	Invalidate();
}


int CSortDemoView::bits(int x, int shift, int bit)
{
	//把 x 的值從第 bit+1 的 bit 開始提 shift 個所得的值
	int mask = 0xffffffff >> (32 - bit) << shift;
	bitset<32> in(x);
	bitset<32> result((x & mask) >> shift);
	return result.to_ulong();
}
