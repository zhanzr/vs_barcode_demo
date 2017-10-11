// Help.h: interface for the Help class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELP_H__DC824341_BE4B_11DA_8051_0050BA012D9B__INCLUDED_)
#define AFX_HELP_H__DC824341_BE4B_11DA_8051_0050BA012D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Help  
{
public:
	static void DrawSignUP(CDC*pDC,int iX0,int iY0,int iX1,int iY1);
	static int GetValue(CEdit&ed);

};

#endif // !defined(AFX_HELP_H__DC824341_BE4B_11DA_8051_0050BA012D9B__INCLUDED_)
