// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BCD34608_B9B6_11DA_8051_0050BA012D9B__INCLUDED_)
#define AFX_STDAFX_H__BCD34608_B9B6_11DA_8051_0050BA012D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <cstdint>

#include "Lib\Barcode.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class _stdafx
{
public:
};

#endif // !defined(AFX_STDAFX_H__BCD34608_B9B6_11DA_8051_0050BA012D9B__INCLUDED_)
