// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"
#include <cstdint>


// HistogramDlg dialog

IMPLEMENT_DYNAMIC(HistogramDlg, CDialogEx)

HistogramDlg::HistogramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HistogramDlg, pParent)
{

}

HistogramDlg::~HistogramDlg()
{
}

void HistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

HistogramDlg::HistogramDlg(BMP& accumulator, CWnd* pParent)
    : CDialogEx(HistogramDlg::IDD, pParent), accumulator(accumulator)
{
    displayBitmap = true;
}

BEGIN_MESSAGE_MAP(HistogramDlg, CDialogEx)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// HistogramDlg message handlers


void HistogramDlg::OnPaint()
{
    CPaintDC dc(this);
    CRect imageRect(0, 0, BMPImage.GetWidth(), BMPImage.GetHeight());
    if(!displayBitmap){
    if (bPP == 1)
    {
        int histogramCols[1500] = { 0 };
        int histogramRows[1500] = { 0 };
        int histogramColsMax = 0;
        int histogramRowsMax = 0;

        //iteracja (x,0->height) zliczanie z pionie i rysowanie z poziomie(kreski w pionie)
        for (int x = 0; x < imageRect.Width(); x++)
        {
            for (int y = 0; y < imageRect.Height(); y++)
            {
                if (!BMPImage.GetPixel1(x, y)) {
                    histogramCols[x]++;
                }
                if (histogramCols[x] > histogramColsMax)
                {
                    histogramColsMax = histogramCols[x];
                }
            }
        }

        //iteracja (0->width, y) zliczanie z poziomie i rysowanie w pionie(kreski w poziomie)
        for (int y = 0; y < imageRect.Height(); y++)
        {
            for (int x = 0; x < imageRect.Width(); x++)
            {
                if (!BMPImage.GetPixel1(x, y))
                {
                    histogramRows[y]++;
                }
                if (histogramRows[y] > histogramRowsMax)
                {
                    histogramRowsMax = histogramRows[y];
                }
            }
        }

        //px w pionie
        for (int i = 0; i < imageRect.Width(); i++) {
            dc.MoveTo(imageRect.left + imageRect.Width() / 20 + i, imageRect.bottom);
            dc.LineTo(imageRect.left + imageRect.Width() / 20 + i, imageRect.bottom - histogramCols[i] * imageRect.Height() / histogramColsMax);
        }

        //zakres px w pionie
        dc.MoveTo(imageRect.left + imageRect.Width() / 20, imageRect.bottom + imageRect.Width() / 40);
        dc.LineTo(imageRect.left + imageRect.Width() / 20 + imageRect.Width(), imageRect.bottom + imageRect.Width() / 40);

        //px w poziomie
        for (int i = 0; i < imageRect.Height(); i++) {
            dc.MoveTo(imageRect.left + imageRect.Width() / 20, imageRect.bottom + imageRect.Width() / 20 + i);
            dc.LineTo(imageRect.left + imageRect.Width() / 20 + histogramRows[i] * imageRect.Width() / histogramRowsMax, imageRect.bottom + imageRect.Width() / 20 + i);
        }

        //zakres px w poziomie
        dc.MoveTo(imageRect.left + imageRect.Width() / 30, imageRect.bottom + imageRect.Width() / 20);
        dc.LineTo(imageRect.left + imageRect.Width() / 30, imageRect.bottom + imageRect.Width() / 20 + imageRect.Height());
    }
    else if (bPP == 8)
    {
        int histogram[256] = { 0 };
        int histogramColsMax = 0;

        for (int x = 0; x < imageRect.Width(); x++)
        {
            for (int y = 0; y < imageRect.Height(); y++)
            {
                int color = BMPImage.GetPixel8(x, y);
                histogram[color]++;
                if (histogram[color] > histogramColsMax)
                {
                    histogramColsMax = histogram[color];
                }
            }
        }

        for (int i = 0; i < 256; i++) {
            dc.MoveTo(cRect.left + i, cRect.bottom);
            dc.LineTo(cRect.left + i, cRect.bottom - histogram[i] * 255 / histogramColsMax);
        }

        dc.MoveTo(cRect.left + prog, cRect.bottom);
        dc.LineTo(cRect.left + prog, cRect.bottom - histogramColsMax);
    }
    else if (bPP == 24)
    {
        int histogramR[256] = { 0 };
        int histogramG[256] = { 0 };
        int histogramB[256] = { 0 };
        int histogramColsMax = 0;

        for (int x = 0; x < imageRect.Width(); x++)
        {
            for (int y = 0; y < imageRect.Height(); y++)
            {
                RGBTRIPLE color = BMPImage.GetPixel24(x, y);
                histogramR[color.rgbtRed]++;
                histogramG[color.rgbtGreen]++;
                histogramB[color.rgbtBlue]++;
                if (histogramR[color.rgbtRed] > histogramColsMax)
                {
                    histogramColsMax = histogramR[color.rgbtRed];
                }
                if (histogramG[color.rgbtGreen] > histogramColsMax)
                {
                    histogramColsMax = histogramG[color.rgbtGreen];
                }
                if (histogramB[color.rgbtBlue] > histogramColsMax)
                {
                    histogramColsMax = histogramB[color.rgbtBlue];
                }
            }
        }

        for (int i = 0; i < 256; i++) {
            dc.MoveTo(cRect.left + i, cRect.bottom);
            dc.LineTo(cRect.left + i, cRect.bottom - histogramR[i] * 255 / histogramColsMax);

            dc.MoveTo(cRect.right + cRect.left + i, cRect.bottom);
            dc.LineTo(cRect.right + cRect.left + i, cRect.bottom - histogramG[i] * 255 / histogramColsMax);

            dc.MoveTo(cRect.right * 2 + cRect.left + i, cRect.bottom);
            dc.LineTo(cRect.right * 2 + cRect.left + i, cRect.bottom - histogramB[i] * 255 / histogramColsMax);
        }
    }
    return;
    }

    float maxVal = 0;

    for (uint32_t x = 0; x < accumulator.GetWidth(); ++x)
    {
        for (uint32_t y = 0; y < accumulator.GetHeight(); ++y)
        {
            if (accumulator.GetPixel8(x, y) > maxVal)
            {
                maxVal = accumulator.GetPixel8(x, y);
            }
        }
    }

    for (uint32_t x = 0; x < accumulator.GetWidth(); ++x)
    {
        for (uint32_t y = 0; y < accumulator.GetHeight(); ++y)
        {
            float color = accumulator.GetPixel8(x, y);
            color = 255- (color / maxVal) * 255;
            accumulator.SetPixel8(x, y, color);
        }
    }

    CRect clientRect, imgRect(0, 0, accumulator.GetWidth(), accumulator.GetHeight());
    GetClientRect(clientRect);
    accumulator.PaintDIB(dc, clientRect, imgRect);
}


BOOL HistogramDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    bPP = BMPImage.biBitCount;
    cRect = CRect(100, 100, 356, 356);

    int histogramDialogWidth = 0;

    if(bPP == 1)
    {
        int histChannelWindowSize = BMPImage.GetHeight();
        if (BMPImage.GetWidth() > BMPImage.GetHeight()) 
        {
            histChannelWindowSize = BMPImage.GetWidth();
        }
        cRect.right = cRect.left + histChannelWindowSize;
        cRect.bottom = cRect.top + histChannelWindowSize;
        histogramDialogWidth = cRect.right * 2 + cRect.left;                
    }
    else if (bPP == 8)
    {
        histogramDialogWidth = cRect.right + cRect.left;
    }
    else if (bPP == 24)
    {
        histogramDialogWidth = cRect.right * 3 + cRect.left;
    }

    SetWindowPos(NULL, 0, 0, histogramDialogWidth, cRect.bottom + cRect.top, SWP_NOMOVE | SWP_NOZORDER);

    return TRUE;
}
