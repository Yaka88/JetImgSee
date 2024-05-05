// JetImgSee.h : main header file for the JETIMGSEE DLL
//

#if !defined(AFX_JETIMGSEE_H__AB082188_3079_11D5_AEAC_00D0F8096324__INCLUDED_)
#define AFX_JETIMGSEE_H__AB082188_3079_11D5_AEAC_00D0F8096324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJetImgSeeApp
// See JetImgSee.cpp for the implementation of this class
//

class CJetImgSeeApp : public CWinApp
{
public:
	CJetImgSeeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJetImgSeeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CJetImgSeeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JETIMGSEE_H__AB082188_3079_11D5_AEAC_00D0F8096324__INCLUDED_)
