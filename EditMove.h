#if !defined(AFX_EDITMOVE_H__ED5FC5C1_BD24_11DA_8051_0050BA012D9B__INCLUDED_)
#define AFX_EDITMOVE_H__ED5FC5C1_BD24_11DA_8051_0050BA012D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMove.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditMove window

class EditMove : public CEdit
{
// Construction
public:
	EditMove();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditMove)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~EditMove();

	// Generated message map functions
protected:
	//{{AFX_MSG(EditMove)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMOVE_H__ED5FC5C1_BD24_11DA_8051_0050BA012D9B__INCLUDED_)
