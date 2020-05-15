
// PO1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "PO1Dlg.h"
#include "ParamsDlg.h"
#include "afxdialogex.h"
#include "BMP.h"
#include "HistogramDlg.h"
#include <cstdint>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMG_WND_ID_IN	100
#define IMG_WND_ID_OUT	101

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPODlg dialog



CPODlg::CPODlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CPODlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPODlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_combo1);
    DDX_Control(pDX, IDC_SLIDER1, CSlider);
}

BEGIN_MESSAGE_MAP(CPODlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_LOAD, &CPODlg::OnBnClickedButtonLoad)
    ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CPODlg::OnBnClickedButtonProcess)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPODlg::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_PARAMS, &CPODlg::OnBnClickedButtonParams)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CPODlg::OnNMCustomdrawSlider1)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CPODlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CPODlg message handlers

BOOL CPODlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    CRect rDlg(7, 7, 407, 407);
    MapDialogRect(rDlg);

    m_imgIN.Create(rDlg, this, IMG_WND_ID_IN);

    rDlg = CRect(530, 7, 930, 407);
    MapDialogRect(rDlg);

    m_imgOUT.Create(rDlg, this, IMG_WND_ID_OUT);

    m_combo1.AddString(L"1. PixelOpsy: Convert to greyscale");
    m_combo1.AddString(L"1. PixelOpsy: Zmiana jasnosci");
    m_combo1.AddString(L"1. PixelOpsy: Zmiana kontrastu");
    m_combo1.AddString(L"1. PixelOpsy: Potegowanie");
    m_combo1.AddString(L"1. PixelOpsy: Negatyw");
    m_combo1.AddString(L"2. Histogram: Histogram");
    m_combo1.AddString(L"2. Histogram: Wyrownanie histogramu");
    m_combo1.AddString(L"2. Histogram: Binaryzacja");
    m_combo1.AddString(L"3. Progowanie: Gradient");
    m_combo1.AddString(L"3. Progowanie: Iteracyjny");
    m_combo1.AddString(L"3. Progowanie: Otsu");
    m_combo1.AddString(L"3. Progowanie: Lokalne");
    m_combo1.AddString(L"4. Filtry: LinearFilterAVG");
    m_combo1.AddString(L"4. Filtry: LinearFilterGauss");
    m_combo1.AddString(L"4. Filtry: LinearFilterSobel");
    m_combo1.AddString(L"4. Filtry: LinearFilterInverseSobel");
    m_combo1.AddString(L"4. Filtry: LinearFilterLaplasian");
    m_combo1.AddString(L"4. Filtry: LinearFilterSharp");
    m_combo1.AddString(L"4. Filtry: Median3x3Filter8");
    m_combo1.AddString(L"4. Filtry: Median5x5Filter8");
    m_combo1.AddString(L"4. Filtry: MedianCrossFilter8");
    m_combo1.AddString(L"4. Filtry: LoG");
    m_combo1.AddString(L"5. Szkieletyzacja: Pavlidis");
    m_combo1.AddString(L"5. Szkieletyzacja: Mapa odleglosci podglad");
    m_combo1.AddString(L"5. Szkieletyzacja: Mapa odleglosci wynik");
    m_combo1.AddString(L"6. Dylatacja");
    m_combo1.AddString(L"6. Erozja");
    m_combo1.AddString(L"6. Otwarcie");
    m_combo1.AddString(L"6. Zamkniecie");
    m_combo1.AddString(L"6. Dylatacja8");
    m_combo1.AddString(L"6. Erozja8");
    m_combo1.AddString(L"6. Otwarcie8");
    m_combo1.AddString(L"6. Zamkniecie8");
    m_combo1.AddString(L"6. KonturZewnetrzny");
    m_combo1.AddString(L"6. KonturWewnetrzny");
    m_combo1.AddString(L"6. WhiteTopHat");
    m_combo1.AddString(L"6. BlackTopHat");
    m_combo1.AddString(L"7. Hough"); 
    m_combo1.AddString(L"8. Fourier");
    m_combo1.SelectString(0, L"8. Fourier");

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPODlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPODlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CPODlg::OnBnClickedButtonLoad()
{
    WCHAR strFilter[] = { L"Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||" };

    CFileDialog FileDlg(TRUE, NULL, NULL, 0, strFilter);

    int width = 598;
    int height = 648;
    CRect imageRect(0, 0, width, height);
    m_imgOUT.BMPImage.CreateGreyscaleDIB(imageRect, 0, 0, m_imgIN.BMPImage.biBitCount);
    m_imgIN.BMPImage.CreateGreyscaleDIB(imageRect, 0, 0, m_imgIN.BMPImage.biBitCount);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            BYTE greyscale = 255;
            m_imgOUT.BMPImage.SetPixel8(x, y, greyscale);
            m_imgIN.BMPImage.SetPixel8(x, y, greyscale);
        }
    }
    m_imgOUT.InvalidateRect(NULL);
    m_imgIN.InvalidateRect(NULL);

    if (FileDlg.DoModal() == IDOK)
    {
        CString filePath = FileDlg.GetPathName();
        if (m_imgIN.BMPImage.LoadDIB(filePath))
        {
            MessageBox(L"Loading Succeeded", MB_OK);
            m_imgIN.InvalidateRect(NULL);
            return;
        }
        else
        {
            MessageBox(L"Loading Failed", MB_OK);
        }
    }
}

void CPODlg::DrawLine(float kat, float ro, int szerokosc, int wysokosc, int color)
{
    for (int x = 0; x < szerokosc; x++)
    {
        float yNorm = wysokosc / 2 + (ro - (x - szerokosc / 2) * cos(kat)) / sin(kat);

        if (yNorm < wysokosc && yNorm >= 0)
        {
            m_imgOUT.BMPImage.SetPixel8(x, (int)yNorm, color);
        }
    }

    for (int y = 0; y < wysokosc; y++)
    {
        float xNorm = szerokosc / 2 + (ro - (y - wysokosc / 2) * sin(kat)) / cos(kat);

        if (xNorm < szerokosc && xNorm >= 0)
        {
            m_imgOUT.BMPImage.SetPixel8((int)xNorm, y, color);
        }
    }
}

void ExterminateAccumulator(BMP* accumulator, int maxGlobalRo, int step, int maxStep, int maxRo, float maxVal)
{
    int maxfi = maxStep;
    int maxro = maxRo;

    bool**beenThereDoneThat = new bool*[360 / step];

    for (int i = 0; i < 360 / step; i++)
    {
        beenThereDoneThat[i] = new bool[maxGlobalRo];

        for (int j = 0; j < maxGlobalRo; j++)
        {
            beenThereDoneThat[i][j] = false;
        }
    }

    bool kontynuuj = true;
    int promien = 0;
    while (kontynuuj)
    {
        promien++;
        accumulator->SetPixel8(maxfi, maxro, 0);
        for (int ro = maxro - promien; ro <= maxro + promien; ro++)
        {
            for (int fi = maxfi - promien; fi <= maxfi + promien; fi++)
            {
                if (ro >= maxGlobalRo || fi >= 360 / step || ro < 0 || fi < 0) continue;

                if (beenThereDoneThat[fi][ro]) continue;

                if (accumulator->GetPixel8(fi, ro) == 0)
                {
                    beenThereDoneThat[fi][ro] = true;
                    continue;
                }

                if (accumulator->GetPixel8(fi, ro) >= (int)(maxVal / 10.0f))
                {
                    accumulator->SetPixel8(fi, ro, 0);
                    beenThereDoneThat[fi][ro] = true;
                }
                else
                {
                    kontynuuj = false;
                }
            }
        }

        if (promien > maxGlobalRo) kontynuuj = false;
    }

    delete beenThereDoneThat;
}

void FindAccumulatorMax(BMP* accumulator, int * maxVal, int * maxTheta, int * maxRo, int krok, int maxGlobalRo, int houghLines)
{
    for (int i = 0; i < houghLines; i++)
    {
        maxVal[i] = 0;
        maxTheta[i] = 0;
        maxRo[i] = 0;
    }

    maxVal[0] = 0;
    maxTheta[0] = 0;
    maxRo[0] = 0;

    for (int i = 0; i < 360 / krok; i++)
    {
        for (int ro = 0; ro < maxGlobalRo; ro++)
        {
            if (accumulator->GetPixel8(i, ro) > maxVal[0])
            {
                for (int a = houghLines - 2; a >= 0; a--)
                {
                    maxVal[a + 1] = maxVal[a];
                    maxTheta[a + 1] = maxTheta[a];
                    maxRo[a + 1] = maxRo[a];
                }

                maxVal[0] = accumulator->GetPixel8(i, ro);
                maxTheta[0] = i;
                maxRo[0] = ro;
            }
        }
    }
}

void CPODlg::OnBnClickedButtonProcess()
{
    CString sOption;
    m_combo1.GetLBText(m_combo1.GetCurSel(), sOption);


    if (sOption == L"1. PixelOpsy: Convert to greyscale")
    {
        CRect imageRect(0, 0, m_imgIN.BMPImage.GetWidth(), m_imgIN.BMPImage.GetHeight());
        m_imgOUT.BMPImage.CreateGreyscaleDIB(imageRect, 0, 0, m_imgIN.BMPImage.biBitCount);
        for (int x = 0; x < m_imgIN.BMPImage.GetWidth(); x++)
        {
            for (int y = 0; y < m_imgIN.BMPImage.GetHeight(); y++)
            {
                BYTE color = 0;
                if (m_imgIN.BMPImage.biBitCount == 1) {
                    color = m_imgIN.BMPImage.GetPixel1(x, y) * 255;
                }
                else if (m_imgIN.BMPImage.biBitCount == 8)
                {
                    color = m_imgIN.BMPImage.GetPixel8(x, y);
                }
                else if (m_imgIN.BMPImage.biBitCount == 24)
                {
                    RGBTRIPLE rgbtriple = m_imgIN.BMPImage.GetPixel24(x, y);
                    color = rgbtriple.rgbtRed * 0.299 + rgbtriple.rgbtGreen * 0.587 + rgbtriple.rgbtBlue * 0.114;
                }
                m_imgOUT.BMPImage.SetPixel8(x, y, color);
            }
        }
        m_imgOUT.InvalidateRect(NULL);
    }
    else if (sOption == L"1. PixelOpsy: Negatyw") {
        m_imgOUT.BMPImage.Negatyw(&m_imgIN.BMPImage);
        m_imgOUT.InvalidateRect(NULL);
    }
    else if (sOption == L"1. PixelOpsy: Zmiana jasnosci") {
        //CSlider.SetRange(-100, 100);
        m_imgOUT.BMPImage.ZmienJasnosc(&m_imgIN.BMPImage, CSlider.GetPos());
        m_imgOUT.InvalidateRect(NULL);
    }

    else if (sOption == L"1. PixelOpsy: Zmiana kontrastu") {
        //CSlider.SetRange(10, 50);
        m_imgOUT.BMPImage.ZmienKontrast(&m_imgIN.BMPImage, (float)(CSlider.GetPos()) / 10.0f);
        m_imgOUT.InvalidateRect(NULL);
    }
    else if (sOption == L"1. PixelOpsy: Potegowanie") {
        //CSlider.SetRange(10, 30); 
        m_imgOUT.BMPImage.Potegowanie(&m_imgIN.BMPImage, (float)(CSlider.GetPos()) / 10.0f);
        m_imgOUT.InvalidateRect(NULL);
    }
    else if (sOption == L"2. Histogram: Wyrownanie histogramu") {
        m_imgOUT.BMPImage.WyrownywanieHistogramu(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"2. Histogram: Histogram") {
        histogramDlg.BMPImage = m_imgIN.BMPImage;
        histogramDlg.DoModal();
    }
    else if (sOption == L"2. Histogram: Binaryzacja") {
        m_imgOUT.BMPImage.Binaryzacja(&m_imgIN.BMPImage, CSlider.GetPos());
        m_imgOUT.InvalidateRect(NULL);
    }
    else if (sOption == L"3. Progowanie: Gradient") {
        int prog = m_imgOUT.BMPImage.ProgowanieGradientowe(&m_imgIN.BMPImage);
        m_imgOUT.BMPImage.Binaryzacja(&m_imgIN.BMPImage, prog);
        m_imgOUT.InvalidateRect(NULL);
        histogramDlg.prog = prog;
        histogramDlg.BMPImage = m_imgIN.BMPImage;
        histogramDlg.DoModal();
    }
    else if (sOption == L"3. Progowanie: Iteracyjny") {
        int prog = m_imgOUT.BMPImage.ProgowanieIteracyjne(&m_imgIN.BMPImage);
        m_imgOUT.BMPImage.Binaryzacja(&m_imgIN.BMPImage, prog);
        m_imgOUT.InvalidateRect(NULL);
        histogramDlg.prog = prog;
        histogramDlg.BMPImage = m_imgIN.BMPImage;
        histogramDlg.DoModal();
    }
    else if (sOption == L"3. Progowanie: Otsu") {
        int prog = m_imgOUT.BMPImage.ProgowanieOtsu(&m_imgIN.BMPImage);
        m_imgOUT.BMPImage.Binaryzacja(&m_imgIN.BMPImage, prog);
        InvalidateRect(NULL);
    }
    else if (sOption == L"3. Progowanie: Lokalne") {
        m_imgOUT.BMPImage.ProgowanieLokalne(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LinearFilterAVG") {
        m_imgOUT.BMPImage.LinearFilter8(&m_imgIN.BMPImage, 1);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LinearFilterGauss") {
        m_imgOUT.BMPImage.LinearFilter8(&m_imgIN.BMPImage, 2);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LinearFilterSobel") {
        m_imgOUT.BMPImage.LinearFilter8(&m_imgIN.BMPImage, 3);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LinearFilterInverseSobel") {
        m_imgOUT.BMPImage.LinearFilter8(&m_imgIN.BMPImage, 4);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LinearFilterLaplasian") {
        m_imgOUT.BMPImage.LinearFilter8(&m_imgIN.BMPImage, 5);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LinearFilterSharp") {
        m_imgOUT.BMPImage.LinearFilter8(&m_imgIN.BMPImage, 6);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: Median3x3Filter8") {
        m_imgOUT.BMPImage.Median3x3Filter8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: Median5x5Filter8") {
        m_imgOUT.BMPImage.Median5x5Filter8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: MedianCrossFilter8") {
        m_imgOUT.BMPImage.MedianCrossFilter8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"4. Filtry: LoG") {
        m_imgOUT.BMPImage.FiltrLoG(&m_imgIN.BMPImage, (float)(CSlider.GetPos()) / 10.0f);
        InvalidateRect(NULL);
    }
    else if (sOption == L"5. Szkieletyzacja: Pavlidis") {
        //m_imgOUT.BMPImage.Pavlidis(&m_imgIN.BMPImage);
        CRect imageRect(0, 0, m_imgIN.BMPImage.GetWidth(), m_imgIN.BMPImage.GetHeight());
        m_imgOUT.BMPImage.CreateGreyscaleDIB(imageRect, 0, 0, m_imgIN.BMPImage.biBitCount);

        for (int y = 0; y < m_imgIN.BMPImage.GetHeight(); ++y) {
            for (int x = 0; x < m_imgIN.BMPImage.GetWidth(); ++x) {
                int color = m_imgIN.BMPImage.GetPixel8(x, y);
                m_imgOUT.BMPImage.SetPixel8(x, y, color);
            }
        }

        bool** toDelete = new bool*[m_imgIN.BMPImage.GetWidth()];
        bool** spookyScarySkeleton = new bool*[m_imgIN.BMPImage.GetWidth()];
        for (int i = 0; i < m_imgIN.BMPImage.GetWidth(); ++i) {
            toDelete[i] = new bool[m_imgIN.BMPImage.GetHeight()];
            spookyScarySkeleton[i] = new bool[m_imgIN.BMPImage.GetHeight()];
        }

        for (int y = 0; y < m_imgIN.BMPImage.GetHeight(); ++y) {
            for (int x = 0; x < m_imgIN.BMPImage.GetWidth(); ++x) {
                spookyScarySkeleton[x][y] = false;
            }
        }

        int masksWithRotations[6][3][3] = {
            {{ 2, 2, 2},{ 0, -1, 0},{ 3, 3, 3}},
            {{ 2, 0, 3 },{ 2, -1, 3 },{ 2, 0, 3 }},
            {{ 2, 2, 2 },{ 0, -1, 2 },{ 1, 0, 2 }},
            {{ 1, 0, 2 },{ 0, -1, 2 },{ 2, 2, 2 }},
            {{ 2, 0, 1 },{ 2, -1, 0 },{ 2, 2, 2 }},
            {{ 2, 2, 2 },{ 2, -1, 0 },{ 2, 0, 1 }}
        };

        bool hasBeenDeleted = true;
        while (hasBeenDeleted == true) {
            hasBeenDeleted = false;
            for (int q = 0; q < 4; ++q) {
                for (int y = 0; y < m_imgIN.BMPImage.GetHeight(); ++y) {
                    for (int x = 0; x < m_imgIN.BMPImage.GetWidth(); ++x) {
                        toDelete[x][y] = false;
                    }
                }
                int movX = 0;
                int movY = 0;
                if (q == 0) {
                    movX = 1;
                }
                else if (q == 1) {
                    movX = -1;
                }
                else if (q == 2) {
                    movY = 1;
                }
                else if (q == 3) {
                    movY = -1;
                }
                for (int y = 1; y < m_imgIN.BMPImage.GetHeight() - 1; ++y) {
                    for (int x = 1; x < m_imgIN.BMPImage.GetWidth() - 1; ++x) {
                        if (spookyScarySkeleton[x][y] == true)continue;
                        BYTE pixel = m_imgOUT.BMPImage.GetPixel8(x, y);
                        if (pixel == 0) {
                            BYTE neighbour = m_imgOUT.BMPImage.GetPixel8(x + movX, y + movY);
                            if (neighbour == 255) {
                                bool match[6] = { true, true, true, true, true, true };
                                for (int z = 0; z < 6; ++z) {
                                    int groupSumX = 0;
                                    int groupSumY = 0;
                                    for (int _x = -1; _x < 2; ++_x) {
                                        for (int _y = -1; _y < 2; ++_y) {
                                            if (_x == 0 && _y == 0)continue;
                                            BYTE pixtelToCheck = m_imgOUT.BMPImage.GetPixel8(x + _x, y + _y);
                                            if (masksWithRotations[z][_x + 1][_y + 1] == 0) {
                                                if (pixtelToCheck == 255) continue;
                                                else {
                                                    match[z] = false;
                                                }
                                            }
                                            else if (masksWithRotations[z][_x + 1][_y + 1] == 1) {
                                                if (pixtelToCheck == 0) continue;
                                                else {
                                                    match[z] = false;
                                                }
                                            }
                                            else if (masksWithRotations[z][_x + 1][_y + 1] == 2) {
                                                if (pixtelToCheck == 0) {
                                                    groupSumX++;
                                                }
                                            }
                                            else if (masksWithRotations[z][_x + 1][_y + 1] == 3) {
                                                if (pixtelToCheck == 0) {
                                                    groupSumY++;
                                                }
                                            }
                                        }
                                    }
                                    if (groupSumX == 0)match[z] = false;
                                    if (z < 2) {
                                        if (groupSumY == 0)match[z] = false;
                                    }
                                }
                                bool shouldDelete = true;
                                for (int z = 0; z < 6; ++z) {
                                    if (match[z] == true) {
                                        shouldDelete = false;
                                        spookyScarySkeleton[x][y] = true;
                                    }
                                }
                                toDelete[x][y] = shouldDelete;
                            }
                        }
                    }
                }
                for (int y = 1; y < m_imgIN.BMPImage.GetHeight() - 1; ++y) {
                    for (int x = 1; x < m_imgIN.BMPImage.GetWidth() - 1; ++x) {
                        if (toDelete[x][y] == true) {
                            m_imgOUT.BMPImage.SetPixel8(x, y, 255);
                            hasBeenDeleted = true;
                        }
                    }
                }
            }

            m_imgOUT.PaintImg();
            Sleep(5);
        }
        m_imgOUT.InvalidateRect(NULL);
        InvalidateRect(NULL);
    }
    else if (sOption == L"5. Szkieletyzacja: Mapa odleglosci podglad") {
        m_imgOUT.BMPImage.MapaOdleglosci(&m_imgIN.BMPImage, true, (float)(CSlider.GetPos()) / 10.0f);
        InvalidateRect(NULL);
    }
    else if (sOption == L"5. Szkieletyzacja: Mapa odleglosci wynik") {
        m_imgOUT.BMPImage.MapaOdleglosci(&m_imgIN.BMPImage, false, (float)(CSlider.GetPos()) / 10.0f);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Dylatacja") {
        m_imgOUT.BMPImage.Dylatacja(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Erozja") {
        m_imgOUT.BMPImage.Erozja(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Otwarcie") {
        m_imgOUT.BMPImage.Otwarcie(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Zamkniecie") {
        m_imgOUT.BMPImage.Zamkniecie(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Dylatacja8") {
        m_imgOUT.BMPImage.Dylatacja8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Erozja8") {
        m_imgOUT.BMPImage.Erozja8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Otwarcie8") {
        m_imgOUT.BMPImage.Otwarcie8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. Zamkniecie8") {
        m_imgOUT.BMPImage.Zamkniecie8(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. KonturZewnetrzny") {
        m_imgOUT.BMPImage.KonturZewnetrzny(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. KonturWewnetrzny") {
        m_imgOUT.BMPImage.KonturWewnetrzny(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. WhiteTopHat") {
        m_imgOUT.BMPImage.WhiteTopHat(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"6. BlackTopHat") {
        m_imgOUT.BMPImage.BlackTopHat(&m_imgIN.BMPImage);
        InvalidateRect(NULL);
    }
    else if (sOption == L"7. Hough") {

        int height = m_imgIN.BMPImage.GetHeight();
        int width = m_imgIN.BMPImage.GetWidth();
        float diag = float(sqrt(height*height + width*width)) / 2.0f;        
        int step = CSlider.GetPos();

        BMP accumulator;

        CRect imgRect(0, 0, width, height);
        m_imgOUT.BMPImage.CreateGreyscaleDIB(imgRect, 0, 0, m_imgIN.BMPImage.biBitCount);
                     
        CRect accRect(0, 0, 360 / step, diag);
        accumulator.CreateGreyscaleDIB(accRect, 0, 0, m_imgIN.BMPImage.biBitCount);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (m_imgIN.BMPImage.GetPixel1(x, y) != 0) {
                    continue;
                }
                for (int i = 0; i < 360 / step; i++)
                {
                    float kat = 3.14f * 2.0f * ((i * step) / 360.0);
                    float ro = (float)(x - (width / 2)) * cos(kat) + (float)(y - (height / 2)) * sin(kat);

                    if (ro < 0) {
                        continue;
                    }
                    int color = accumulator.GetPixel8(i, ro);
                    if (color < 255)
                    {
                        color++;
                    }
                    accumulator.SetPixel8(i, ro, color);
                }
            }
        }

        int * maxVal = new int[houghLines];
        int * maxTheta = new int[houghLines];
        int * maxRo = new int[houghLines];

        FindAccumulatorMax(&accumulator, maxVal, maxTheta, maxRo, step, diag, houghLines);

        //Copy IN to OUT
        for (uint32_t x = 0; x < m_imgIN.BMPImage.GetWidth(); ++x)
        {
            for (uint32_t y = 0; y < m_imgIN.BMPImage.GetHeight(); ++y)
            {
                m_imgOUT.BMPImage.SetPixel8(x, y, m_imgIN.BMPImage.GetPixel1(x, y) * 255);
            }
        }

        //Draw
        accumulator.biBitCount = 8;
        HistogramDlg histoDlg(accumulator);
        histoDlg.accumulator = accumulator;
        histoDlg.BMPImage = accumulator;
        if (histoDlg.DoModal() == IDOK)
        {

        }

        if (houghLines > 0)
        {
            for (int a = 0; a < houghLines; a++)
            {
                if (maxVal[a] == 0) continue;

                float kat = 3.14f * 2.0f * (float)(maxTheta[a] * step) / 360.0f;
                DrawLine(kat, maxRo[a], width, height, a * 255/houghLines);
            }
        }
        else
        {
            int globMax = maxVal[0];
            int b = 0;
            while (maxVal[0] >= (globMax / 2))
            {
                float kat = 3.14f * 2.0f * (float)(maxTheta[0] * step) / 360.0f;
                DrawLine(kat, maxRo[0], width, height, b*50);
                b++;
                ExterminateAccumulator(&accumulator, diag, step, maxTheta[0], maxRo[0], maxVal[0]);
                FindAccumulatorMax(&accumulator, maxVal, maxTheta, maxRo, step, diag, houghLines);
            }
        }

        m_imgOUT.InvalidateRect(NULL);
    }
    else if (sOption == L"8. Fourier")
    {
        float PI = 3.14159265359;
        int width = m_imgIN.BMPImage.GetWidth();
        int height = m_imgIN.BMPImage.GetHeight();
        CRect imgRect(0, 0, width, height);
        BMP amplitudeBMP, phaseBMP, tmpBMP;
        
        amplitudeBMP.CreateGreyscaleDIB(imgRect, 0, 0, m_imgIN.BMPImage.biBitCount);
        phaseBMP.CreateGreyscaleDIB(imgRect, 0, 0, m_imgIN.BMPImage.biBitCount);
        tmpBMP.CreateGreyscaleDIB(imgRect, 0, 0, m_imgIN.BMPImage.biBitCount);
        m_imgOUT.BMPImage.CreateGreyscaleDIB(imgRect, 0, 0, m_imgIN.BMPImage.biBitCount);
        
        float diag = sqrt(width * width + height * height);
        float centreX = width / 2, centreY = height / 2;
        float alfa = 0.53836;
        float beta = 1 - alfa;

        //Hamming
        for (uint32_t x = 0; x < width; ++x)
        {
            for (uint32_t y = 0; y < height; ++y)
            {
                float r = sqrt((centreX - x) * (centreX - x) + (centreY - y) * (centreY - y));
                float n = diag * 0.5f - r;
                float w = alfa - beta * cos((2 * PI * n) / (diag - 1));
                tmpBMP.SetPixel8(x, y, m_imgIN.BMPImage.GetPixel8(x, y) * w);
            }
        }

        float maxAmp = 0;
        float maxPhase = 0;

        std::vector<std::vector<float>> fourierReal(width);
        std::vector<std::vector<float>> fourierImag(width);
        for (float u = 0; u < width; ++u)
        {
            fourierReal[u].resize(height);
            fourierImag[u].resize(height);
        }

        //Fourier
        for (float u = 0; u < width; ++u)
        {
            for (float v = 0; v < height; ++v)
            {
                float real = 0;
                float imaginary = 0;
                for (float x = 0; x < width; ++x)
                {
                    for (float y = 0; y < height; ++y)
                    {
                        float commonPart = 2 * PI * (u * x / width + v * y / height);
                        real += tmpBMP.GetPixel8(x, y) * cos(commonPart);
                        imaginary += tmpBMP.GetPixel8(x, y) * ((-1) * sin(commonPart));
                    }
                }
                fourierImag[u][v] = imaginary;
                fourierReal[u][v] = real;
                float amplitude = log(1 + sqrt(real * real + imaginary * imaginary));
                if (amplitude > maxAmp)
                {
                    maxAmp = amplitude;
                }
                amplitudeBMP.SetPixel8(u, v, amplitude);

                float faza = atan2(imaginary, real) * 0.5f * 255.0f / PI + 127;
                phaseBMP.SetPixel8(u, v, faza);
            }
        }

        //Ma³peczki
        for (float x = 0; x < width; ++x){
            for (float y = 0; y < height; ++y){
                tmpBMP.SetPixel8(x, y, (float)amplitudeBMP.GetPixel8(x, y) / maxAmp * 255);
            }
        }
        for (float x = 0; x < width; ++x){
            for (float y = 0; y < height; ++y){
                int newX = (int)(x + width / 2) % width;
                int newY = (int)(y + height / 2) % height;
                amplitudeBMP.SetPixel8(newX, newY, tmpBMP.GetPixel8(x, y));
            }
        }
        for (float x = 0; x < width; ++x){
            for (float y = 0; y < height; ++y){
                tmpBMP.SetPixel8(x, y, (float)phaseBMP.GetPixel8(x, y));
            }
        }
        for (float x = 0; x < width; ++x){
            for (float y = 0; y < height; ++y){
                int newX = (int)(x + width / 2) % width;
                int newY = (int)(y + height / 2) % height;
                phaseBMP.SetPixel8(newX, newY, tmpBMP.GetPixel8(x, y));
            }
        }

        amplitudeBMP.biBitCount = 8;
        HistogramDlg ampDlg(amplitudeBMP);
        ampDlg.accumulator = amplitudeBMP;
        ampDlg.BMPImage = amplitudeBMP;

        if (ampDlg.DoModal() == IDOK)
        {
        }

        phaseBMP.biBitCount = 8;
        HistogramDlg phaseDlg(phaseBMP);
        phaseDlg.accumulator = amplitudeBMP;
        phaseDlg.BMPImage = amplitudeBMP;

        if (phaseDlg.DoModal() == IDOK)
        {
        }

        //DeMa³peczki
        for (float x = 0; x < width; ++x) {
            for (float y = 0; y < height; ++y) {
                tmpBMP.SetPixel8(x, y, (float)amplitudeBMP.GetPixel8(x, y) / maxAmp * 255);
            }
        }
        for (float x = 0; x < width; ++x) {
            for (float y = 0; y < height; ++y) {
                int newX = (int)(x + width / 2) % width;
                int newY = (int)(y + height / 2) % height;
                amplitudeBMP.SetPixel8(newX, newY, tmpBMP.GetPixel8(x, y));
            }
        }
        for (float x = 0; x < width; ++x) {
            for (float y = 0; y < height; ++y) {
                tmpBMP.SetPixel8(x, y, (float)phaseBMP.GetPixel8(x, y));
            }
        }
        for (float x = 0; x < width; ++x) {
            for (float y = 0; y < height; ++y) {
                int newX = (int)(x + width / 2) % width;
                int newY = (int)(y + height / 2) % height;
                phaseBMP.SetPixel8(newX, newY, tmpBMP.GetPixel8(x, y));
            }
        }

        //DeFourier
        float** defurier = new float*[width];

        for (int x = 0; x < width; ++x)
        {
            defurier[x] = new float[height];
        }

        float maxDef = 0;

        for (float x = 0; x < width; ++x)
        {
            for (float y = 0; y < height; ++y)
            {
                float sum = 0;
                for (float u = 0; u < width; ++u)
                {
                    for (float v = 0; v < height; ++v)
                    {
                        float commonPart = 2 * PI * (u * x / width + v * y / height);
                        float realPart = cos(commonPart);
                        float imaginaryPart = sin(commonPart);
                        sum += fourierReal[u][v] * realPart - fourierImag[u][v] * imaginaryPart;
                    }
                }
                if (sum > maxDef)
                {
                    maxDef = sum;
                }
                defurier[(int)x][(int)y] = sum;
            }
        }

        //DeHamming
        for (uint32_t x = 0; x < width; ++x)
        {
            for (uint32_t y = 0; y < height; ++y)
            {
                int r = sqrt((centreX - x) * (centreX - x) + (centreY - y) * (centreY - y));
                int n = diag * 0.5f - r;
                float w = alfa - beta * cos((2 * PI * n) / (diag - 1));
                float color = ((defurier[x][y] / maxDef * 255)) / w;
                if(color < 0) color = 0;
                else if (color > 255) color = 255; 
                m_imgOUT.BMPImage.SetPixel8(x, y, color);
            }
        }
        m_imgOUT.InvalidateRect(NULL);
    }
}

void CPODlg::OnBnClickedButtonSave()
{
    WCHAR strFilter[] = { L"Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||" };

    CFileDialog FileDlg(FALSE, NULL, NULL, 0, strFilter);

    if (FileDlg.DoModal() == IDOK)
    {
        CString filePath = FileDlg.GetPathName();
        if (m_imgOUT.BMPImage.SaveDIB(filePath))
        {
            MessageBox(L"Saving Succeeded", MB_OK);
            return;
        }
        else
        {
            MessageBox(L"Saving Failed", MB_OK);
        }
    }
}


void CPODlg::OnBnClickedButtonParams()
{
    CParamsDlg paramsDlg;
    CString s;

    if (paramsDlg.DoModal() == IDOK)
    {
        s = paramsDlg.m_sParams;
        houghLines = _wtoi(s);
    }
}


void CPODlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    *pResult = 0;
}



void CPODlg::OnCbnSelchangeCombo1()
{
    CString sOption;
    m_combo1.GetLBText(m_combo1.GetCurSel(), sOption);


    if (sOption == L"1. PixelOpsy: Zmiana jasnosci")
    {
        CSlider.SetRange(-100, 100);
    }
    else if (sOption == L"1. PixelOpsy: Zmiana kontrastu")
    {
        CSlider.SetRange(1, 50);
    }
    else if (sOption == L"1. PixelOpsy: Potegowanie")
    {
        CSlider.SetRange(1, 30);
    }
    else if (sOption == L"2. Histogram: Binaryzacja")
    {
        CSlider.SetRange(0, 255);
    }
    else if (sOption == L"4. Filtry: LoG")
    {
        CSlider.SetRange(10, 50);
    }
    else if (sOption == L"5. Szkieletyzacja: Mapa odleglosci wynik")
    {
        CSlider.SetRange(0, 250);
    }
    else if (sOption == L"5. Szkieletyzacja: Mapa odleglosci podglad")
    {
        CSlider.SetRange(0, 500);
    }
    else if (sOption == L"7. Hough")
    {
        CSlider.SetRange(1, 20);
        CSlider.SetPos(1);
    }
    else
    {
        CSlider.SetRange(1, 1);
    }
}