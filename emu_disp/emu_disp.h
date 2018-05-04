// emu_disp.h : main header file for the EMU_DISP application
//

#if !defined(AFX_EMU_DISP_H__78ED677D_C19B_476D_B0E4_6900DBACAA5B__INCLUDED_)
#define AFX_EMU_DISP_H__78ED677D_C19B_476D_B0E4_6900DBACAA5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEmu_dispApp:
// See emu_disp.cpp for the implementation of this class
//

class CEmu_dispApp : public CWinApp
{
public:
	CEmu_dispApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmu_dispApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEmu_dispApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMU_DISP_H__78ED677D_C19B_476D_B0E4_6900DBACAA5B__INCLUDED_)
