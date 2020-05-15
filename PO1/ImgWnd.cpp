// ImgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "ImgWnd.h"

// CImgWnd

IMPLEMENT_DYNAMIC(CImgWnd, CWnd)

CImgWnd::CImgWnd()
{

}

CImgWnd::~CImgWnd()
{
}


BEGIN_MESSAGE_MAP(CImgWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CImgWnd message handlers

BOOL CImgWnd::Create(const RECT& rect, CWnd*  pParentWnd, UINT nID)
{
	BOOL bRes;

	bRes = CWnd::Create(NULL, NULL,
		WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL, rect, pParentWnd, nID, NULL);

	ShowScrollBar(SB_BOTH, FALSE);

	return bRes;
}


void CImgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// Do not call CWnd::OnPaint() for painting messages    
	CRect clientRect;
	GetClientRect(clientRect);
    CRect imageRect = CRect(0,0,BMPImage.GetWidth(), BMPImage.GetHeight());
	BMPImage.PaintDIB(dc, clientRect, imageRect);
}

void CImgWnd::PaintImg()
{
    CRect clientRect;
    CRect imgRect = CRect(0, 0, BMPImage.GetWidth(), BMPImage.GetHeight());
    GetClientRect(clientRect);
    BMPImage.PaintDIB(::GetDC(m_hWnd), clientRect, imgRect);
}