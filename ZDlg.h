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
	UINT m_SendTime;
	uint32_t m_lastWdt;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDLG_H__BCD34606_B9B6_11DA_8051_0050BA012D9B__INCLUDED_)
