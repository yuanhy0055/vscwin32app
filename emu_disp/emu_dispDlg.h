// emu_dispDlg.h : header file
//

#if !defined(AFX_EMU_DISPDLG_H__AFAF3167_4F15_4B48_9F7F_028C2BBB4EAF__INCLUDED_)
#define AFX_EMU_DISPDLG_H__AFAF3167_4F15_4B48_9F7F_028C2BBB4EAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEmu_dispDlg dialog

class CEmu_dispDlg : public CDialog
{
// Construction
public:
	CEmu_dispDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEmu_dispDlg)
	enum { IDD = IDD_EMU_DISP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmu_dispDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEmu_dispDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMU_DISPDLG_H__AFAF3167_4F15_4B48_9F7F_028C2BBB4EAF__INCLUDED_)
