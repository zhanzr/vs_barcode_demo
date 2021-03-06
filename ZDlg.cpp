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
	, m_Pat(0)
	, m_SelfIp(0)
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
	DDX_IPAddress(pDX, IDC_IP_BOARD, m_BoardIP);
	DDX_IPAddress(pDX, IDC_IP_SELF, m_SelfIp);
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
	ON_BN_CLICKED(IDC_PAT0, &ZDlg::OnBnClickedPat0)
	ON_BN_CLICKED(IDC_PAT1, &ZDlg::OnBnClickedPat1)
	ON_BN_CLICKED(IDC_PAT2, &ZDlg::OnBnClickedPat2)
	ON_BN_CLICKED(IDC_PAT3, &ZDlg::OnBnClickedPat3)
	ON_BN_CLICKED(IDC_PAT4, &ZDlg::OnBnClickedPat4)
	ON_BN_CLICKED(IDC_PAT5, &ZDlg::OnBnClickedPat5)
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

	m_BoardIP = DEFAULT_TARGET_IP_W;
	m_SelfIp = DEFAULT_SELF_IP_W;

	CheckRadioButton(IDC_PAT0, IDC_PAT5, IDC_PAT0);

	UpdateData(FALSE);

	MoveWindow(400, 400, 1160, 256, TRUE);

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

	//CaptureScreenMono("m_" + csStyle + "_" + csCode + "_" + tmpStr);
}

void ZDlg::OnEnUpdateRepeat()
{
}

void ZDlg::OnEnChangeRepeat()
{

}

void ZDlg::CaptureScreen(const char* filename) 
{ 
//#define	BMP_DEPTH	24
//	// get screen rectangle 
//	RECT windowRect; 
//	GetWindowRect(&windowRect); 
//
//	// bitmap dimensions 
//	auto bitmap_dx = windowRect.right - windowRect.left; 
//	//auto bitmap_dx = m_lastWdt * 2; 
//	auto bitmap_dy = windowRect.bottom - windowRect.top; 
//
//	// create file 
//	ofstream file(filename, ios::binary); 
//	if(!file) 
//		return; 
//
//	// save bitmap file headers 
//	BITMAPFILEHEADER fileHeader; 
//	BITMAPINFOHEADER infoHeader; 
//
//	fileHeader.bfType      = 0x4d42; 
//	fileHeader.bfSize      = 0; 
//	fileHeader.bfReserved1 = 0; 
//	fileHeader.bfReserved2 = 0; 
//	fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
//
//	infoHeader.biSize          = sizeof(infoHeader); 
//	infoHeader.biWidth         = bitmap_dx; 
//	infoHeader.biHeight        = bitmap_dy; 
//	infoHeader.biPlanes        = 1; 
//	infoHeader.biBitCount      = BMP_DEPTH; 
//	infoHeader.biCompression   = BI_RGB; 
//	infoHeader.biSizeImage     = 0; 
//	infoHeader.biXPelsPerMeter = 0; 
//	infoHeader.biYPelsPerMeter = 0; 
//	infoHeader.biClrUsed       = 0; 
//	infoHeader.biClrImportant  = 0; 
//
//	file.write((char*)&fileHeader, sizeof(fileHeader)); 
//	file.write((char*)&infoHeader, sizeof(infoHeader)); 
//
//	// dibsection information 
//	BITMAPINFO info; 
//	info.bmiHeader = infoHeader;  
//
//	// ------------------ 
//	// THE IMPORTANT CODE 
//	// ------------------ 
//	// create a dibsection and blit the window contents to the bitmap 
//	auto winDC = GetWindowDC(); 
//	auto memDC = CreateCompatibleDC(winDC->m_hDC); 
//	BYTE* memory = 0; 
//	HBITMAP bitmap = CreateDIBSection(winDC->m_hDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0); 
//	SelectObject(memDC, bitmap); 
//	BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC->m_hDC, 0, 0, SRCCOPY); 
//	DeleteDC(memDC); 
//	ReleaseDC(winDC); 
//
//	// save dibsection data 
//	int bytes = (((BMP_DEPTH*bitmap_dx + 31) & (~31))/8)*bitmap_dy; 
//	file.write((const char*)memory, bytes); 
//	file.close();
//	DeleteObject(bitmap); 

	//Network Send
	//char testData[100];
	//uint32_t testLen = 100;
	//for(auto i=0; i<testLen; ++i)
	//{
	//	testData[i] = 'a'+i%26;
	//}
	//sprintf(testData, "%s%d", __DATE__, 12345678);

	//Pattern Information
	m_PatInfo.totalLen = TEST_PAT_WID * TEST_PAT_HGT / 8;
	m_PatInfo.curLen = 0;
	m_PatInfo.curOff = 0;
	m_PatInfo.wid = TEST_PAT_WID;
	m_PatInfo.hgt = TEST_PAT_HGT;
#define SINGLE_PKT_LEN	1024

	sockaddr_in dest;
	sockaddr_in local;
	char tmpAddr[16];
	WSAData data;
	WSAStartup( MAKEWORD( 2, 2 ), &data );

	local.sin_family = AF_INET;
	
	sprintf(tmpAddr, "%d.%d.%d.%d",
		0x000000ff & (m_SelfIp>>24),
		0x000000ff & (m_SelfIp>>16), 
		0x000000ff & (m_SelfIp>>8),
		0x000000ff &m_SelfIp);
	local.sin_addr.s_addr = inet_addr( tmpAddr );
	local.sin_port = 0;//htons( UDP_DATA_PORT );//0-> choose any

	UpdateData(TRUE);
	dest.sin_family = AF_INET;
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
	
	for(auto t=0; t<m_SendTime; ++t)
	{
		while(m_PatInfo.curOff < m_PatInfo.totalLen)
		{
			m_PatInfo.curLen = min(SINGLE_PKT_LEN, m_PatInfo.totalLen - m_PatInfo.curOff);

			//Send Info as separate packet
			auto infoRet = sendto( s, 
				(const char*)&m_PatInfo, 
				sizeof(m_PatInfo),
				0, 
				(sockaddr *)&dest,
				sizeof(dest) );

			//Send the data
			auto ret = sendto( s, 
				((const char*)m_PatBuf) + m_PatInfo.curOff, 
				m_PatInfo.curLen,
				0, 
				(sockaddr *)&dest,
				sizeof(dest) );
			m_PatInfo.curOff += ret;
		}
	}
//#undef BMP_DEPTH
}

const uint16_t BMPFileType = 0x4D42;//Always "BM"

void ZDlg::CaptureScreenMono(const char* filename) 
{ 
#define	BMP_DEPTH	16
	// get screen rectangle 
	RECT windowRect; 
	GetWindowRect(&windowRect); 

	// bitmap dimensions 
	int bitmap_dx = windowRect.right - windowRect.left; 
	//auto bitmap_dx = (m_lastWdt * 2)/8; 
	//bitmap_dx *= 8;
	auto bitmap_dy = windowRect.bottom - windowRect.top; 

	// create file 
	ofstream file(filename, ios::binary); 
	if(!file) 
		return; 

	// save bitmap file headers 
	BITMAPFILEHEADER fileHeader; 
	BITMAPINFOHEADER infoHeader; 

	fileHeader.bfType      = 0x4d42; 
	fileHeader.bfReserved1 = 0; 
	fileHeader.bfReserved2 = 0; 
	fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
	fileHeader.bfSize      = fileHeader.bfOffBits + bitmap_dx*bitmap_dy*BMP_DEPTH/8; 

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

	// save dibsection data 
	int bytes = (((BMP_DEPTH*bitmap_dx + 31) & (~31))/8)*bitmap_dy; 
	file.write((const char*)memory, bytes); 
	file.close();

	//Test Monochrome bitmap
	SimpleBMP16Header sbh;
    uint8_t tmpPix;
    uint32_t bw = 0;
	FILE* fOutputBin;
	uint32_t i;
	uint32_t j;
	
	char mono_name[256];
	sprintf(mono_name, "mono_%s", filename);
	fOutputBin = fopen(mono_name, "wb");
	sbh.unused1 = 0;
	sbh.unused2 = 0;
    sbh.InfoSize = 0x28;
    sbh.DataOff = sbh.InfoSize + 0x16;
    sbh.FileSize = (sbh.DataOff + bitmap_dx * bitmap_dy * TEST_DPT)/8;

    sbh.Wid = bitmap_dx;
    sbh.Hgt = bitmap_dy;

    sbh.PaneNum = 1;
    sbh.ColorDepth = TEST_DPT;

    sbh.CompressMethod = BI_RGB ;//BI_RGB
    sbh.BitSize = (bitmap_dx * bitmap_dy * TEST_DPT) / 8;
    sbh.XDPI = 0;//0x0B13;
    sbh.YDPI = 0;//0x0B13;
    sbh.ColorInPlatte = 0;
    sbh.ImportantColor = 0;

    //sbh.R_Mask = 0xF800;
    //sbh.G_Mask = 0x07E0;
    //sbh.B_Mask = 0x001F;
    sbh.Bit_Mask = 0;
    sbh.A_Mask = 0x00ffffff;
	
    fwrite(&BMPFileType, sizeof(BMPFileType), 1, fOutputBin);
	fwrite(&sbh, sizeof(sbh), 1, fOutputBin);

	
	for(auto pVet = 0; pVet<sbh.Hgt; ++pVet)
	{
		tmpPix = 0xa3;
		for(auto j=0; j<(sbh.Wid * TEST_DPT)/8; j++)
        {
			//auto Pix7 = winDC->GetPixel(j*8 + 0, pVet);
			//auto Pix6 = winDC->GetPixel(j*8 + 1, pVet);
			//auto Pix5 = winDC->GetPixel(j*8 + 2, pVet);
			//auto Pix4 = winDC->GetPixel(j*8 + 3, pVet);
			//auto Pix3 = winDC->GetPixel(j*8 + 4, pVet);
			//auto Pix2 = winDC->GetPixel(j*8 + 5, pVet);
			//auto Pix1 = winDC->GetPixel(j*8 + 6, pVet);
			//auto Pix0 = winDC->GetPixel(j*8 + 7, pVet);
            fwrite(&tmpPix, sizeof(tmpPix), 1, fOutputBin);
        }

		//for(auto tmp = 0; tmp < 8; ++tmp)
		//{
		//	uint16_t cWord = *(uint16_t*)(memory + pByte * (BMP_DEPTH/TEST_DPT) + tmp * sizeof(uint16_t));
		//	//auto Red = (cWord>>11)&0x1F;
		//	//auto Green = (cWord>>5)&0x3F;
		//	//auto Blue = (cWord)&0x1F;
		//	//auto Y = 0.299 * Red + 0.587 * Green/2 + 0.114 * Blue;
		//	//if(Y>16)
		//	//{
		//	//	tmpPix |= (1<<(7-tmp));
		//	//}
		//	
		//	if(cWord >= 0x7BDE)
		//	{
		//		//tmpPix |= (1<<(7-tmp));
		//		tmpPix |= (1<<(tmp));
		//	}
		//}

		////tmpPix = 0xcc;

		//fwrite(&tmpPix, sizeof(tmpPix), 1, fOutputBin);
	}

	fseek(fOutputBin, sbh.DataOff + bitmap_dx * bitmap_dy * TEST_DPT/8, SEEK_SET);
    fclose(fOutputBin);

	DeleteDC(memDC); 
	ReleaseDC(winDC); 
	DeleteObject(bitmap); 
#undef BMP_DEPTH
}

void ZDlg::OnBnClickedPat0()
{
	m_Pat = 0;
	//CString tmpStr;
	//tmpStr.Format("%s_%d", __FUNCTION__, m_Pat);
	//AfxMessageBox(tmpStr);
	for(auto line=0; line<TEST_PAT_HGT; ++line)
	{
		for(auto wid=0; wid<TEST_PAT_WID/(2*8); ++wid)
		{
			m_PatBuf[line*TEST_PAT_WID/(2*8) + wid] = 0xFF00;
		}
	}

	TRACE("%d\n", m_Pat);
}


void ZDlg::OnBnClickedPat1()
{
	m_Pat = 1;
	for(auto line=0; line<TEST_PAT_HGT; ++line)
	{
		for(auto wid=0; wid<TEST_PAT_WID/(2*8); ++wid)
		{
			m_PatBuf[line*TEST_PAT_WID/(2*8) + wid] = 0xF0F0;
		}
	}
	TRACE("%d\n", m_Pat);
}

void ZDlg::OnBnClickedPat2()
{
	m_Pat = 2;
	for(auto line=0; line<TEST_PAT_HGT; ++line)
	{
		for(auto wid=0; wid<TEST_PAT_WID/(2*8); ++wid)
		{
			m_PatBuf[line*TEST_PAT_WID/(2*8) + wid] = 0xCCCC;
		}
	}
	TRACE("%d\n", m_Pat);
}

void ZDlg::OnBnClickedPat3()
{
	m_Pat = 3;
	for(auto line=0; line<TEST_PAT_HGT; ++line)
	{
		for(auto wid=0; wid<TEST_PAT_WID/(2*8); ++wid)
		{
			m_PatBuf[line*TEST_PAT_WID/(2*8) + wid] = 0xAAAA;
		}
	}
	TRACE("%d\n", m_Pat);
}

void ZDlg::OnBnClickedPat4()
{
	m_Pat = 4;
	for(auto line=0; line<TEST_PAT_HGT; ++line)
	{
		for(auto wid=0; wid<(TEST_PAT_WID/(2*8))/2; ++wid)
		{	
			if(0 == wid%2)
			{
				m_PatBuf[line*TEST_PAT_WID/(2*8) + wid*2] = 0xFF00;
				m_PatBuf[line*TEST_PAT_WID/(2*8) + wid*2+1] = 0;
			}
			else
			{
				m_PatBuf[line*TEST_PAT_WID/(2*8) + wid*2] = 0;
				m_PatBuf[line*TEST_PAT_WID/(2*8) + wid*2+1] = 0xFF00;
			}
		}
	}
	TRACE("%d\n", m_Pat);
}

//打印字体试一下子
//纵向,倒序, 16x16
//For Every Font, need to rearrange
//^^^^^^^^
//^^^^^^^^  
uint8_t hanzi_8_test[]={
0x10,0x10,0x10,0xFF,0x10,0x90,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,
0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,
0x00,0xFC,0x44,0x44,0x44,0x42,0x42,0x00,0xFC,0x04,0x04,0x04,0x04,0xFC,0x00,0x00,
0x00,0x1F,0x10,0x10,0x08,0x08,0x08,0x00,0xFF,0x00,0x08,0x10,0x08,0x07,0x00,0x00,
0x10,0x0C,0x04,0x24,0x24,0x24,0x25,0x26,0xA4,0x64,0x24,0x04,0x04,0x14,0x0C,0x00,
0x02,0x02,0x02,0x02,0x02,0x42,0x82,0x7F,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,
0x00,0x80,0x60,0xF8,0x07,0x10,0x10,0x10,0xD0,0xFF,0xD0,0x10,0x10,0x10,0x00,0x00,
0x01,0x00,0x00,0xFF,0x10,0x08,0x04,0x0B,0x08,0xFF,0x08,0x0B,0x04,0x08,0x10,0x00,
0x40,0x40,0x42,0xCC,0x00,0x90,0x90,0x90,0x90,0x90,0xFF,0x10,0x11,0x16,0x10,0x00,
0x00,0x00,0x00,0x3F,0x10,0x28,0x60,0x3F,0x10,0x10,0x01,0x0E,0x30,0x40,0xF0,0x00,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x42,0x82,0x02,0x02,0x02,0x02,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x01,0x06,0x00,0x00,0x00,
0x80,0x82,0x82,0x82,0x82,0x82,0x82,0xE2,0xA2,0x92,0x8A,0x86,0x82,0x80,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

void ZDlg::OnBnClickedPat5()
{
	m_Pat = 5;
	//ReRange
	uint16_t tmpLine[16];
	for(auto i=0; i<8; ++i)
	{
		for(auto j=0; j<16; ++j)
		{
			tmpLine[j] = hanzi_8_test[i*32 + j] + (((uint16_t)hanzi_8_test[i*32 + j+16])<<8);
		}
		memcpy((void*)&hanzi_8_test[i*32], (const void*)tmpLine, sizeof(tmpLine));
	}

	//Font Test
	memcpy((void*)&m_PatBuf[TEST_PAT_WID * 0 / (8 * sizeof(uint16_t))],
		hanzi_8_test,
		sizeof(hanzi_8_test));
	//Bliank Test
	memset((void*)&m_PatBuf[TEST_PAT_WID * 1 / (8 * sizeof(uint16_t))],
		0,
		sizeof(hanzi_8_test));
	//Font Test
	memcpy((void*)&m_PatBuf[TEST_PAT_WID * 2 / (8 * sizeof(uint16_t))],
		hanzi_8_test,
		sizeof(hanzi_8_test));
	//Bliank Test
	memset((void*)&m_PatBuf[TEST_PAT_WID * 3 / (8 * sizeof(uint16_t))],
		0,
		sizeof(hanzi_8_test));
	//Font Test
	memcpy((void*)&m_PatBuf[TEST_PAT_WID * 4 / (8 * sizeof(uint16_t))],
		hanzi_8_test,
		sizeof(hanzi_8_test));
	//Bliank Test
	memset((void*)&m_PatBuf[TEST_PAT_WID * 5 / (8 * sizeof(uint16_t))],
		0,
		sizeof(hanzi_8_test));
	//Font Test
	memcpy((void*)&m_PatBuf[TEST_PAT_WID * 6 / (8 * sizeof(uint16_t))],
		hanzi_8_test,
		sizeof(hanzi_8_test));
	//Bliank Test
	memset((void*)&m_PatBuf[TEST_PAT_WID * 7 / (8 * sizeof(uint16_t))],
		0,
		sizeof(hanzi_8_test));
	TRACE("%d\n", m_Pat);
}

