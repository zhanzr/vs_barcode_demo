// EditMove.cpp : implementation file
//

#include "stdafx.h"
#include "BarCode.h"
#include "EditMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditMove

EditMove::EditMove()
{
}

EditMove::~EditMove()
{
}


BEGIN_MESSAGE_MAP(EditMove, CEdit)
	//{{AFX_MSG_MAP(EditMove)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditMove message handlers

void EditMove::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	if((nChar==VK_LEFT)||(nChar==VK_RIGHT))
	{
		CWnd*pw=GetParent();
		if(nChar==VK_LEFT)	pw->PostMessage(WM_USER,-1);	
		if(nChar==VK_RIGHT)	pw->PostMessage(WM_USER,+1);	
	}
	else
	if((nChar==VK_UP)||(nChar==VK_DOWN))
	{
		CWnd*pw=GetParent();
		if(nChar==VK_UP)	pw->PostMessage(WM_USER+1,-1);	
		if(nChar==VK_DOWN)	pw->PostMessage(WM_USER+1,+1);	
	}
}
