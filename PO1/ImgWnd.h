#pragma once

// CImgWnd
#include "BMP.h"

class CImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgWnd)

public:
	CImgWnd();
	virtual ~CImgWnd();

	BOOL Create(const RECT& rect, CWnd*  pParentWnd, UINT nID);
    BMP BMPImage;

protected:
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
    afx_msg void PaintImg();
};


