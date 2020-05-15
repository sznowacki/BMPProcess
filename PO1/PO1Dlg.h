
// PO1Dlg.h : header file
//

#pragma once
#include "ImgWnd.h"
#include "afxwin.h"
#include "HistogramDlg.h"


// CPODlg dialog
class CPODlg : public CDialogEx
{
// Construction
public:
	CPODlg(CWnd* pParent = NULL);	// standard constructor
    int houghLines = 0;

// Dialog Data
	enum { IDD = IDD_PO1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
    HistogramDlg histogramDlg;

// Implementation
protected:
	HICON m_hIcon;

	CImgWnd m_imgIN;
	CImgWnd m_imgOUT;

	CComboBox m_combo1;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonParams();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl CSlider;
    afx_msg void OnCbnSelchangeCombo1();
    void CPODlg::DrawLine(float kat, float ro, int szerokosc, int wysokosc, int color);
    void CPODlg::RysujProsta(float kat, float ro, int szerokosc, int wysokosc);
};
