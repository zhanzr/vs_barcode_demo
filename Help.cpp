// Help.cpp: implementation of the Help class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarCode.h"
#include "Help.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

void Help::DrawSignUP(CDC *pDC, int iX0, int iY0, int iX1, int iY1)
{
	pDC->MoveTo(iX0-1,	iY0);
	pDC->LineTo(iX0-20,	iY0);

	pDC->MoveTo(iX1+1,	iY0);
	pDC->LineTo(iX1+20,	iY0);

	pDC->MoveTo(iX0-1,	iY1);
	pDC->LineTo(iX0-20,	iY1);

	pDC->MoveTo(iX1+1,	iY1);
	pDC->LineTo(iX1+20,	iY1);

	//-----------------------------
	pDC->MoveTo(iX0,	iY0+1);
	pDC->LineTo(iX0,	iY0+20);

	pDC->MoveTo(iX1,	iY0+1);
	pDC->LineTo(iX1,	iY0+20);

	pDC->MoveTo(iX0,	iY1-1);
	pDC->LineTo(iX0,	iY1-20);

	pDC->MoveTo(iX1,	iY1-1);
	pDC->LineTo(iX1,	iY1-20);
}

int Help::GetValue(CEdit&ed)
{
	CString cs;
	ed.GetWindowText(cs);
	return _ttoi(cs);
}
