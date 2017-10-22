// ZDlg.h : header file
//

#if !defined(AFX_ZDLG_H__BCD34606_B9B6_11DA_8051_0050BA012D9B__INCLUDED_)
#define AFX_ZDLG_H__BCD34606_B9B6_11DA_8051_0050BA012D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

#include "EditMove.h"

#define	UDP_DATA_PORT	8090
#define DEFAULT_TARGET_IP_W	(((UINT)192<<24) + ((UINT)168<<16) + ((UINT)0<<8) + 105)
#define DEFAULT_SELF_IP_W	(((UINT)192<<24) + ((UINT)168<<16) + ((UINT)0<<8) + 122)

typedef struct strSimpleBMP16Header{
		uint32_t FileSize;//DataOff+Wid*Hgt*2
		uint16_t unused1;
		uint16_t unused2;
		uint32_t DataOff;//InfoSize + 0x16 always

		uint32_t InfoSize;//always 0x28
		uint32_t Wid;
		uint32_t Hgt;
		uint16_t PaneNum;//always 1
		uint16_t ColorDepth;//always 0x10

		uint32_t CompressMethod;//Always BI_BITFIELDS for simplification
		uint32_t BitSize;// Wid*Hgt*2, Including Padding 4Byte
		uint32_t XDPI;//Always 0x0B13 for simplification
		uint32_t YDPI;//Always 0x0B13 for simplification
		uint32_t ColorInPlatte;//0 for simplification
		uint32_t ImportantColor;//0 for simplification
		
		//uint32_t R_Mask;//0xF800
		//uint32_t G_Mask;//0x07E0
		//uint32_t B_Mask;//0x001F
		uint32_t Bit_Mask;//0
		uint32_t A_Mask;//0x00FFFFFF;		
}SimpleBMP16Header;

//For BMP screen shot output
#define BI_RGB	0
#define BI_BITFIELDS	3
#define CI_12_FORMAT    0x03
#define CI_16_FORMAT    0x05
#define CI_18_FORMAT    0x06

#define BLACK16 0x0000
#define WHITE16 0xffff
#define RED16   0xf800
#define CYNK16   0x4ffc
#define GREEN16 0x07e0
#define BLUE16  0x001f
#define LIGHT_BLUE16  0x067f
#define GREEN_YELLOW16  0xb7e6

#define LCD_WID ((uint16_t)320)
#define LCD_HGT ((uint16_t)240)
#define LCD_W LCD_WID
#define LCD_H LCD_HGT
#define LCD_PTS_TOTAL	((uint32_t)LCD_W*LCD_H)
#define TEST_DPT	1	//16

#define TEST_PAT_WID	160
#define TEST_PAT_HGT	1000

struct PatInfo
{
	uint32_t totalLen;
	uint32_t curOff;
	uint16_t curLen;
	uint16_t wid;
	uint16_t hgt;
};

class ZDlg : public CDialog
{
// Construction
public:
	~ZDlg();
	ZDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(ZDlg)
	enum { IDD = DLG_MAIN };
	CButton	btn_Menu;
	EditMove	ed_Move;
	CEdit	ed_PrintY;
	CEdit	ed_PrintX;
	CEdit	ed_PrintH;
	CEdit	ed_PenW;
	CComboBox	cmb_Style;
	CComboBox	cmb_MapMode;
	CEdit	ed_Code;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void On_IDOK();
	afx_msg void On_BtnHelp();
	afx_msg void OnDestroy();
	afx_msg void On_BtnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void On_BtnOnTop();
	afx_msg void On_CmbStyleSelChange();
	afx_msg void On_CmbMapModeSelChange();
	afx_msg void On_BtnPrint();
	afx_msg void On_EdCodeChange();
	afx_msg void On_EdPenWidthChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	uint32_t P_DrawBarcode(CDC*pDC,int iX,int iY0,int iY10,int iY11,COLORREF clrBar,COLORREF clrSpace,int iPenW,BarcodeBase*pBc);
	void P_SetOnTop();
	int P_AddOrGet(CComboBox&box,BOOL bAdd);
	uint32_t P_DrawBarCode(CDC*pDC,const BOOL bPrint);

public:
	afx_msg void OnBnClickedsend();
	afx_msg void OnEnUpdateRepeat();
	afx_msg void OnEnChangeRepeat();
	void CaptureScreen(const char* filename);
	void CaptureScreenMono(const char* filename);

	DWORD m_BoardIP;
	DWORD m_SelfIp;
	UINT m_SendTime;
	uint32_t m_lastWdt;
	int m_Pat;

	PatInfo m_PatInfo;
	uint16_t m_PatBuf[TEST_PAT_WID * TEST_PAT_HGT / (8 * sizeof(uint16_t))];

	afx_msg void OnBnClickedPat0();
	afx_msg void OnBnClickedPat1();
	afx_msg void OnBnClickedPat2();
	afx_msg void OnBnClickedPat3();
	afx_msg void OnBnClickedPat4();
	afx_msg void OnBnClickedPat5();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDLG_H__BCD34606_B9B6_11DA_8051_0050BA012D9B__INCLUDED_)
