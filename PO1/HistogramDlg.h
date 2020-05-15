#pragma once
#include "BMP.h"


// HistogramDlg dialog

class HistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(HistogramDlg)

public:
	HistogramDlg(CWnd* pParent = nullptr);   // standard constructor
    HistogramDlg(BMP& accumulator, CWnd* pParent = NULL);
	virtual ~HistogramDlg();

// Dialog Data
	enum { IDD = IDD_HistogramDlg };

private:
    CRect cRect;
    int bPP;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    virtual BOOL OnInitDialog();
    BMP BMPImage;
    bool displayBitmap = false;
    BMP accumulator;
	int prog = 0;
};
