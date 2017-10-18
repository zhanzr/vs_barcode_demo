// ZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarCode.h"
#include "ZDlg.h"

#include "Help.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

ZDlg::ZDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ZDlg::IDD, pParent)
	, m_SendTime(0)
	, m_BoardIP(0)
{
	//{{AFX_DATA_INIT(ZDlg)
	//}}AFX_DATA_INIT
}

ZDlg::~ZDlg()
{
}

void ZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZDlg)

	DDX_Control(pDX, ED_PENW, ed_PenW);
	DDX_Control(pDX, CMB_STYLE, cmb_Style);
	DDX_Control(pDX, CMB_MAPMODE, cmb_MapMode);
	DDX_Control(pDX, ED_CODE, ed_Code);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_REPEAT, m_SendTime);
	DDV_MinMaxUInt(pDX, m_SendTime, 1, 100000);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_BoardIP);
}

BEGIN_MESSAGE_MAP(ZDlg, CDialog)
	//{{AFX_MSG_MAP(ZDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, On_IDOK)
	ON_BN_CLICKED(BTN_HELP, On_BtnHelp)
	ON_WM_DESTROY()
	ON_BN_CLICKED(BTN_CLOSE, On_BtnClose)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(CMB_STYLE, On_CmbStyleSelChange)
	ON_CBN_SELCHANGE(CMB_MAPMODE, On_CmbMapModeSelChange)
	ON_BN_CLICKED(BTN_PRINT, On_BtnPrint)
	ON_EN_CHANGE(ED_CODE, On_EdCodeChange)
	ON_EN_CHANGE(ED_PENW, On_EdPenWidthChange)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_send, &ZDlg::OnBnClickedsend)
	ON_EN_UPDATE(IDC_REPEAT, &ZDlg::OnEnUpdateRepeat)
	ON_EN_CHANGE(IDC_REPEAT, &ZDlg::OnEnChangeRepeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZDlg message handlers

void ZDlg::On_IDOK() 
{
}

void ZDlg::On_BtnClose() 
{
	OnCancel();	
}

void ZDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);

	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION);
}

void ZDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

int ZDlg::P_AddOrGet(CComboBox &box, BOOL bAdd)
{
	struct IntString
	{
		int iV;
		TCHAR *psz;
	};

	IntString ps[]=
	{
		{MM_TEXT,		_T("MM_TEXT")},
		{MM_LOENGLISH,	_T("MM_LOENGLISH")},
		{MM_HIENGLISH,	_T("MM_HIENGLISH")},
		{MM_LOMETRIC,	_T("MM_LOMETRIC")},
		{MM_TWIPS,		_T("MM_TWIPS")},
	};
	
	int i,iNum=sizeof(ps)/sizeof(ps[0]);

	if(bAdd==0)
	{
		i=box.GetCurSel();
		if((i<0)||(i>=iNum))	return ps[0].iV;
		return ps[i].iV;
	}

	for(i=0;i<iNum;i++)
	{
		box.AddString(ps[i].psz);
	}
	return 0;
}

BOOL ZDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Using barcode library"));

	P_AddOrGet(cmb_MapMode,1);

	TCHAR* pCode[]=
	{
		_T("Code39"),
		_T("Code93"),
		_T("Code128A"),
		_T("Code128B"),
		_T("Code128C"),
		_T("CodeI2of5"),
		_T("CodeEan13"),
	};

	int i,iNum=sizeof(pCode)/sizeof(pCode[0]);
	for(i=0;i<iNum;i++)
	{
		cmb_Style.AddString(pCode[i]);
	}
	
	cmb_Style.SetCurSel(3);
	cmb_MapMode.SetCurSel(1);
	ed_Code.SetWindowText(_T("CodeTest123"));

	ed_PenW.SetWindowText(_T("2"));

	m_SendTime = 1;

	m_BoardIP = (UINT)192<<24;
	m_BoardIP += (UINT)168<<16;
	m_BoardIP += (UINT)1<<8;
	m_BoardIP += 30;

	UpdateData(FALSE);

	MoveWindow(400, 400, 1150, 256, TRUE);

	return 1;
}

void ZDlg::On_BtnHelp() 
{
	TCHAR sz[1024];
	::GetModuleFileName(0,sz,1020);
	TCHAR *p = _tcsrchr(sz,'\\');
	p++;
	*p=0;
	_tcscat(sz,_T("help.htm"));
	::ShellExecute(0,0,sz,0,0,SW_SHOW);
}

void ZDlg::On_CmbStyleSelChange() 
{
	RedrawWindow();	
}

void ZDlg::On_CmbMapModeSelChange() 
{
	RedrawWindow();	
}

void ZDlg::On_EdCodeChange() 
{
	RedrawWindow();	
}

void ZDlg::On_EdPenWidthChange() 
{
	RedrawWindow();	
}

void ZDlg::OnPaint() 
{
	CPaintDC dc(this);

	m_lastWdt = P_DrawBarCode(&dc,0);
}


void ZDlg::On_BtnPrint() 
{
	CPrintDialog cp(0);
	if(cp.DoModal()!=IDOK)	return;
	HDC hDC=cp.GetPrinterDC();
	CDC*pDC=CDC::FromHandle(hDC);
	m_lastWdt = P_DrawBarCode(pDC,1);
}

uint32_t ZDlg::P_DrawBarCode(CDC*pDC,const BOOL bPrint)
{
	uint32_t ret = 0;

	const int iSel=cmb_Style.GetCurSel();
	if(iSel<0)
		return ret;

	//--------------------------------------------
	int iL,iT,iB;
	const int iMapMode=pDC->SetMapMode(P_AddOrGet(cmb_MapMode,0));
	
	{
		CRect rcT;
		if(bPrint)
		{
			rcT.left	=Help::GetValue(ed_PrintX);
			rcT.top		=Help::GetValue(ed_PrintY);
			rcT.bottom	=rcT.top+Help::GetValue(ed_PrintH);

			//temp, useless value
			rcT.right=rcT.left+100;
		}
		else
		{
			GetWindowRect(&rcT);
			::MapWindowPoints(0,m_hWnd,(POINT*)&rcT,2);
			rcT.left+=4;
			rcT.bottom+=30;
		}
		pDC->DPtoLP(&rcT);
		iL=rcT.left;
		iT=rcT.top;
		iB=rcT.bottom;
	}
	//--------------------------------------------

	int iPenW=Help::GetValue(ed_PenW);
	if(iPenW==0)	iPenW=1;

	CString csStyle;
	cmb_Style.GetWindowText(csStyle);
	CString csCode;
	ed_Code.GetWindowText(csCode);

	COLORREF clrBar		=RGB(0,0,0);
	COLORREF clrSpace	=RGB(255,255,255);

	if(bPrint)
	{
		pDC->StartDoc(_T("Hello"));
		pDC->StartPage();
	}

	if(csStyle.CompareNoCase(_T("Code39"))==0)
	{
		Barcode39 code;
		code.Encode39(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	else
	if(csStyle.CompareNoCase(_T("Code93"))==0)
	{
		Barcode93 code;
		code.Encode93(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	else
	if(csStyle.CompareNoCase(_T("CodeI2of5"))==0)
	{
		BarcodeI2of5 code;
		code.EncodeI2of5(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	else
	if(csStyle.CompareNoCase(_T("Code128A"))==0)
	{
		Barcode128 code;
		code.Encode128A(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	else
	if(csStyle.CompareNoCase(_T("Code128B"))==0)
	{
		Barcode128 code;
		code.Encode128B(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	else
	if(csStyle.CompareNoCase(_T("Code128C"))==0)
	{
		Barcode128 code;
		code.Encode128C(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	else
	if(csStyle.CompareNoCase(_T("CodeEan13"))==0)
	{
		BarcodeEan13 code;
		code.EncodeEan13(csCode.GetBuffer(0));
		ret = P_DrawBarcode(pDC,iL,iT,iB-10,iB,clrBar,clrSpace,iPenW,&code);
	}
	
	if(bPrint)
	{
		pDC->EndPage();
		pDC->EndDoc();
	}
	pDC->SetMapMode(iMapMode);

	return ret;
}

uint32_t ZDlg::P_DrawBarcode(CDC*pDC,int iX,int iY0,int iY10,int iY11,COLORREF clrBar,COLORREF clrSpace,int iPenW,BarcodeBase*pBc)
{
	return pBc->DrawBarcode(pDC->m_hDC,iX,iY0,iY10,iY11,clrBar,clrSpace,iPenW);	
}

LRESULT ZDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CRect rc;
	if(message==WM_USER)
	{
		GetWindowRect(&rc);
		rc.left+=wParam;
		rc.right+=wParam;
		MoveWindow(&rc);
	}
	else
	if(message==WM_USER+1)
	{
		GetWindowRect(&rc);
		rc.top+=wParam;
		rc.bottom+=wParam;
		MoveWindow(&rc);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


void ZDlg::OnBnClickedsend()
{
	if(m_SendTime <1)
	{
		m_SendTime = 2;
	}
	else if(m_SendTime > 10000)
	{
		m_SendTime = 9999;
	}

	//CString tmpStr;
	//tmpStr.Format(_T("%08X %d"), m_BoardIP, m_SendTime);
	//AfxMessageBox(tmpStr);

	CString csStyle;
	cmb_Style.GetWindowText(csStyle);
	CString csCode;
	ed_Code.GetWindowText(csCode);
	CString tmpStr;
	 auto ct=CTime::GetCurrentTime();
	tmpStr.Format(_T("%04d%02d%02d_%02d%02d%02d.bmp"), 
		ct.GetYear(),
		ct.GetMonth(),
		ct.GetDay(),
		ct.GetHour(),
		ct.GetMinute(),
		ct.GetSecond() );

	CaptureScreen(csStyle + "_" + csCode + "_" + tmpStr);

	CaptureScreenMono("m_" + csStyle + "_" + csCode + "_" + tmpStr);
}

void ZDlg::OnEnUpdateRepeat()
{
}

void ZDlg::OnEnChangeRepeat()
{

}

void ZDlg::CaptureScreen(const char* filename) 
{ 
#define	BMP_DEPTH	24
	// get screen rectangle 
	RECT windowRect; 
	GetWindowRect(&windowRect); 

	// bitmap dimensions 
	//int bitmap_dx = windowRect.right - windowRect.left; 
	auto bitmap_dx = m_lastWdt * 2; 
	auto bitmap_dy = windowRect.bottom - windowRect.top; 

	// create file 
	ofstream file(filename, ios::binary); 
	if(!file) 
		return; 

	// save bitmap file headers 
	BITMAPFILEHEADER fileHeader; 
	BITMAPINFOHEADER infoHeader; 

	fileHeader.bfType      = 0x4d42; 
	fileHeader.bfSize      = 0; 
	fileHeader.bfReserved1 = 0; 
	fileHeader.bfReserved2 = 0; 
	fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

	infoHeader.biSize          = sizeof(infoHeader); 
	infoHeader.biWidth         = bitmap_dx; 
	infoHeader.biHeight        = bitmap_dy; 
	infoHeader.biPlanes        = 1; 
	infoHeader.biBitCount      = BMP_DEPTH; 
	infoHeader.biCompression   = BI_RGB; 
	infoHeader.biSizeImage     = 0; 
	infoHeader.biXPelsPerMeter = 0; 
	infoHeader.biYPelsPerMeter = 0; 
	infoHeader.biClrUsed       = 0; 
	infoHeader.biClrImportant  = 0; 

	file.write((char*)&fileHeader, sizeof(fileHeader)); 
	file.write((char*)&infoHeader, sizeof(infoHeader)); 

	// dibsection information 
	BITMAPINFO info; 
	info.bmiHeader = infoHeader;  

	// ------------------ 
	// THE IMPORTANT CODE 
	// ------------------ 
	// create a dibsection and blit the window contents to the bitmap 
	auto winDC = GetWindowDC(); 
	auto memDC = CreateCompatibleDC(winDC->m_hDC); 
	BYTE* memory = 0; 
	HBITMAP bitmap = CreateDIBSection(winDC->m_hDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
	SelectObject(memDC, bitmap); 
	BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC->m_hDC, 0, 0, SRCCOPY); 
	DeleteDC(memDC); 
	ReleaseDC(winDC); 

	// save dibsection data 
	int bytes = (((BMP_DEPTH*bitmap_dx + 31) & (~31))/8)*bitmap_dy; 
	file.write((const char*)memory, bytes); 

	DeleteObject(bitmap); 

	//Network Send
	char testData[100];
	uint32_t testLen = 100;
	for(auto i=0; i<testLen; ++i)
	{
		testData[i] = 'a'+i%26;
	}
	//sprintf(testData, "%s%d", __DATE__, 12345678);

	sockaddr_in dest;
	sockaddr_in local;
	WSAData data;
	WSAStartup( MAKEWORD( 2, 2 ), &data );

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr( "192.168.1.100" );
	local.sin_port = htons( UDP_DATA_PORT );//0-> choose any

	dest.sin_family = AF_INET;
	char tmpAddr[16];
	sprintf(tmpAddr, "%d.%d.%d.%d",
		0x000000ff & (m_BoardIP>>24),
		0x000000ff & (m_BoardIP>>16), 
		0x000000ff & (m_BoardIP>>8),
		0x000000ff &m_BoardIP);
	dest.sin_addr.s_addr = inet_addr( tmpAddr );
	dest.sin_port = htons( UDP_DATA_PORT );

	// create the socket
	SOCKET s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	// bind to the local address
	bind( s, (sockaddr *)&local, sizeof(local) );
	// send the pkt
	int ret = sendto( s, testData, testLen, 0, (sockaddr *)&dest, sizeof(dest) );
#undef BMP_DEPTH
}

void ZDlg::CaptureScreenMono(const char* filename) 
{ 
#define	BMP_DEPTH	16
	// get screen rectangle 
	RECT windowRect; 
	GetWindowRect(&windowRect); 

	// bitmap dimensions 
	//int bitmap_dx = windowRect.right - windowRect.left; 
	auto bitmap_dx = m_lastWdt * 2; 
	auto bitmap_dy = windowRect.bottom - windowRect.top; 

	// create file 
	ofstream file(filename, ios::binary); 
	if(!file) 
		return; 

	// save bitmap file headers 
	BITMAPFILEHEADER fileHeader; 
	BITMAPINFOHEADER infoHeader; 

	fileHeader.bfType      = 0x4d42; 
	fileHeader.bfSize      = 0; 
	fileHeader.bfReserved1 = 0; 
	fileHeader.bfReserved2 = 0; 
	fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

	infoHeader.biSize          = sizeof(infoHeader); 
	infoHeader.biWidth         = bitmap_dx; 
	infoHeader.biHeight        = bitmap_dy; 
	infoHeader.biPlanes        = 1; 
	infoHeader.biBitCount      = BMP_DEPTH; 
	infoHeader.biCompression   = BI_RGB; 
	infoHeader.biSizeImage     = 0; 
	infoHeader.biXPelsPerMeter = 0; 
	infoHeader.biYPelsPerMeter = 0; 
	infoHeader.biClrUsed       = 0; 
	infoHeader.biClrImportant  = 0; 

	file.write((char*)&fileHeader, sizeof(fileHeader)); 
	file.write((char*)&infoHeader, sizeof(infoHeader)); 

	// dibsection information 
	BITMAPINFO info; 
	info.bmiHeader = infoHeader;  

	// ------------------ 
	// THE IMPORTANT CODE 
	// ------------------ 
	// create a dibsection and blit the window contents to the bitmap 
	auto winDC = GetWindowDC(); 
	auto memDC = CreateCompatibleDC(winDC->m_hDC); 
	BYTE* memory = 0; 
	HBITMAP bitmap = CreateDIBSection(winDC->m_hDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
	SelectObject(memDC, bitmap); 
	BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC->m_hDC, 0, 0, SRCCOPY); 
	DeleteDC(memDC); 
	ReleaseDC(winDC); 

	// save dibsection data 
	int bytes = (((BMP_DEPTH*bitmap_dx + 31) & (~31))/8)*bitmap_dy; 
	file.write((const char*)memory, bytes); 

	DeleteObject(bitmap); 
#undef BMP_DEPTH
}