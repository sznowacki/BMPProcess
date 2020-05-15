#include "stdafx.h"
#include "BMP.h"
#include <cstdint>
#include <algorithm>

BMP::BMP()
{

}


BMP::~BMP()
{
    //HeapFree(GetProcessHeap(), 0, wskaznik_na_bufor);    
}

bool BMP::LoadDIB(CString sciezka_do_pliku)
{
    CFile file;
    
    file.Open(sciezka_do_pliku, CFile::modeReadWrite);

    //Read BMP File Header 
    file.Read((BYTE*)&bmpFileHeader, sizeof(bmpFileHeader));
    //Set pointer to BMPINFO
    if(wskaznik_na_bufor != nullptr)
    {
        HeapFree(GetProcessHeap(), 0, wskaznik_na_bufor);
        wskaznik_na_bufor = nullptr;
    }
    wskaznik_na_bufor = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bmpFileHeader.bfSize - sizeof(BITMAPFILEHEADER));
    //Read BMP BMPINFO
    file.Read(wskaznik_na_bufor, bmpFileHeader.bfSize - sizeof(BITMAPFILEHEADER));
    
    bmpInfoHeader = *(BITMAPINFOHEADER*)wskaznik_na_bufor;
    height = bmpInfoHeader.biHeight;
    width = bmpInfoHeader.biWidth;
    lineLength = (((bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth) + 31) / 32) * 4;;
    ih_to_bmp_px_offset = bmpFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
	biBitCount = bmpInfoHeader.biBitCount;
    if (height == 0 || width == 0)
    {
        return false;
    }
    return true;
}

bool BMP::PaintDIB(HDC kontekst, CRect prost_docelowy, CRect prost_zrodlowy)
{
    SetStretchBltMode(kontekst, COLORONCOLOR);
    if(wskaznik_na_bufor == nullptr)
    {
        return false;
    }
    int DestWidth;
    int DestHeight;
    if (prost_zrodlowy.Width() * prost_docelowy.Height() > prost_docelowy.Width() * prost_zrodlowy.Height())
    {
        DestWidth = prost_docelowy.Width();
        DestHeight = (prost_zrodlowy.Height() * prost_docelowy.Width()) / prost_zrodlowy.Width();            
    }
    else
    {
        DestWidth = (prost_zrodlowy.Width() * prost_docelowy.Height()) / prost_zrodlowy.Height();
        DestHeight = prost_docelowy.Height();
    }
    return StretchDIBits(
        kontekst,
        prost_docelowy.left,
        prost_docelowy.top,
        DestWidth,
        DestHeight,
        prost_zrodlowy.left,
        prost_zrodlowy.top,
        prost_zrodlowy.Width(),
        prost_zrodlowy.Height(),
        (BYTE*)wskaznik_na_bufor + bmpFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER),
        (BITMAPINFO*)wskaznik_na_bufor,
        DIB_RGB_COLORS,
        SRCCOPY);
}

bool BMP::CreateGreyscaleDIB(CRect rozmiar_obrazu, int xPPM, int yPPM, WORD bBC)
{
    width = rozmiar_obrazu.Width();
    height = rozmiar_obrazu.Height();
    lineLength = ((8 * width + 31) / 32) * 4;
    ih_to_bmp_px_offset = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;

    bmpInfoHeader =
    {
        sizeof(BITMAPINFOHEADER),
        width,
        height,
        1,
        8,
        BI_RGB,
        0,
        xPPM,
        yPPM,
        256,
        256
    };
    bmpFileHeader =
    {
        0x4D42,
        ih_to_bmp_px_offset + sizeof(BITMAPFILEHEADER) + (lineLength * height),
        0,
        0,
        ih_to_bmp_px_offset + sizeof(BITMAPFILEHEADER)
    };
    RGBQUAD colors[256];
    for (int i = 0; i < 256; ++i)
    {
        colors[i].rgbBlue = i;
        colors[i].rgbGreen = i;
        colors[i].rgbRed = i;
    }
	
	//HeapFree(GetProcessHeap(), 0, wskaznik_na_bufor);
	wskaznik_na_bufor = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + (lineLength * height));
	memcpy(wskaznik_na_bufor, &bmpInfoHeader, sizeof(bmpInfoHeader));
	memcpy((BYTE*)wskaznik_na_bufor + sizeof(bmpInfoHeader), colors, sizeof(RGBQUAD) * 256);
    return true;
}

bool BMP::GetPixel1(int x, int y)
{
    int row = (height - y - 1) * lineLength; 
    return *((BYTE*)wskaznik_na_bufor + ih_to_bmp_px_offset + (row + (x/8))) & (256 >> ((x % 8) + 1));
}

BYTE BMP::GetPixel8(int x, int y)
{
    int row = (height - y - 1) * lineLength;
    return *((BYTE*)wskaznik_na_bufor + ih_to_bmp_px_offset + row + x);
}

RGBTRIPLE BMP::GetPixel24(int x, int y)
{
    int row = (height - y - 1) * lineLength;
    return *(RGBTRIPLE*)((BYTE*)wskaznik_na_bufor + ih_to_bmp_px_offset + row + (x*3));
}

bool BMP::SetPixel8(int x, int y, BYTE val)
{
    int row = (height - y - 1) * lineLength;
    *((BYTE*)wskaznik_na_bufor + ih_to_bmp_px_offset + row + x) = val;
    return true;
}

bool BMP::SaveDIB(CString sciezka_do_pliku)
{
    CFile file;
    file.Open(sciezka_do_pliku, CFile::modeWrite | CFile::modeCreate);
    file.Write((BYTE*)&bmpFileHeader, sizeof(BITMAPFILEHEADER));
    file.Write((BYTE*)wskaznik_na_bufor, bmpFileHeader.bfSize - sizeof(BITMAPFILEHEADER));
    return true;
}

int BMP::GetHeight() {
    return height;
}

int BMP::GetWidth() {
    return width;
}

void BMP::Negatyw(BMP* bmp)
{
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
    for (int i = 0; i < bmp->GetWidth(); i++)
    {
        for (int j = 0; j < bmp->GetHeight(); j++)
        {
            SetPixel8(i, j, 255 - bmp->GetPixel8(i,j));
        }
    }    
}

void BMP::Binaryzacja(BMP* bmp, int prog)
{
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
	for (int i = 0; i < bmp->GetWidth(); i++)
	{
		for (int j = 0; j < bmp->GetHeight(); j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			BYTE result = color > prog ? 255 : 0;
			SetPixel8(i, j, result);
		}
	}
}

void BMP::ProgowanieLokalne(BMP* bmp) {

    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
    int dx = imageRect.Width() / 10;
    int dy = imageRect.Height() / 10;

    for (int X = 0; X < 10; X++) 
    {
        for (int Y = 0; Y < 10; Y++) 
        {

            int histogram[256] = { 0 };

            int jMin = 255;
            int jMax = 0;

            for (int x = 0 + X * dx; x < (X + 1) * dx; x++) 
            {
                for (int y = 0 + Y * dy; y < (Y + 1) * dy; y++) 
                {
                    histogram[bmp->GetPixel8(x, y)]++;
                    if (bmp->GetPixel8(x, y) < jMin)
                    {
                        jMin = bmp->GetPixel8(x, y);
                    }
                        
                    if (bmp->GetPixel8(x, y) > jMax)
                    {
                        jMax = bmp->GetPixel8(x, y);
                    }                        
                }
            }

            if (jMax - jMin <= 10)
            {
                continue;                
            }               

            int n = dx * dy;

            int tOpt = 0;
            double f[256] = { 0 };

            for (int t = 1; t < 255; t++) 
            {
                float mi0 = 0, p0 = 0;

                for (int i = 0; i <= t; i++) 
                {
                    p0 += histogram[i];
                    mi0 += i * histogram[i];
                }

                p0 /= n;
                mi0 /= n;

                if (p0 != 0)
                {
                    mi0 /= p0;
                }                    

                float mi1 = 0, p1 = 0;

                for (int i = t + 1; i < 256; i++) 
                {
                    p1 += histogram[i];
                    mi1 += i * histogram[i];
                }

                p1 /= n;
                mi1 /= n;

                if (p1 != 0)
                {
                    mi1 /= p1;
                }
                    
                float sigma02p0 = 0;

                for (int i = 0; i <= t; i++) 
                {
                    sigma02p0 += (i - mi0)*(i - mi0) * histogram[i];
                }
                sigma02p0 /= n;

                float sigma12p1 = 0;

                for (int i = t + 1; i < 255; i++) 
                {
                    sigma12p1 += (i - mi1)*(i - mi1) * histogram[i];
                }
                sigma12p1 /= n;

                f[t] = 1.0 / (double)(sigma02p0 + sigma12p1);

                if (f[t] > f[tOpt])
                {
                    tOpt = t;
                }                    
            }

            for (int x = 0 + X * dx; x < (X + 1) * dx; x++) 
            {
                for (int y = 0 + Y * dy; y < (Y + 1) * dy; y++) 
                {
                    SetPixel8(x, y, 255 * (bmp->GetPixel8(x, y) > tOpt));
                }
            }
        }
    }
}

int BMP::ProgowanieOtsu(BMP* bmp) {

    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

    int histogram[256] = { 0 };
    int histogramColsMax = 0;

    for (int x = 0; x < imageRect.Width(); x++)
    {
        for (int y = 0; y < imageRect.Height(); y++)
        {
            int color = bmp->GetPixel8(x, y);
            histogram[color]++;
            if (histogram[color] > histogramColsMax)
            {
                histogramColsMax = histogram[color];
            }
        }
    }

    int n = imageRect.Width() * imageRect.Height();

    int tOpt = 0;
    double* f = new double[256];

    for (int t = 1; t < 255; t++) 
    {
        float mi0 = 0, p0 = 0;

        for (int i = 0; i <= t; i++) {
            p0 += histogram[i];
            mi0 += i * histogram[i];
        }

        p0 /= n;
        mi0 /= n;

        if (p0 != 0) {
            mi0 /= p0;
        }

        float mi1 = 0, p1 = 0;

        for (int i = t + 1; i < 256; i++) 
        {
            p1 += histogram[i];
            mi1 += i * histogram[i];
        }

        p1 /= n;
        mi1 /= n;

        if (p1 != 0)
        {
            mi1 /= p1;            
        }

        float sigma02p0 = 0;

        for (int i = 0; i <= t; i++) 
        {
            sigma02p0 += (i - mi0)*(i - mi0) * histogram[i];
        }
        sigma02p0 /= n;

        float sigma12p1 = 0;

        for (int i = t + 1; i < 255; i++) 
        {
            sigma12p1 += (i - mi1)*(i - mi1) * histogram[i];
        }
        sigma12p1 /= n;

        f[t] = 1.0 / (double)(sigma02p0 + sigma12p1);

        if (f[t] > f[tOpt])
            tOpt = t;
    }

    double ratio = histogramColsMax / f[tOpt];

    for (int t = 1; t < 255; t++) 
    {
        f[t] *= ratio;
    }

    return tOpt;
}

int BMP::ProgowanieIteracyjne(BMP* bmp)
{
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	int histogram[256] = { 0 };
	int histogramColsMax = 0;

	for (int x = 0; x < imageRect.Width(); x++)
	{
		for (int y = 0; y < imageRect.Height(); y++)
		{
			int color = bmp->GetPixel8(x, y);
			histogram[color]++;
			if (histogram[color] > histogramColsMax)
			{
				histogramColsMax = histogram[color];
			}
		}
	}

	int max = 0;
	int min = 255;
	for (int i = 0; i < bmp->GetWidth(); i++)
	{
		for (int j = 0; j < bmp->GetHeight(); j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			if (color > max) {
				max = color;
			}
			if (color < min) {
				min = color;
			}
		}
	}

	int prog = (max + min) / 2;

	int above = 0;
	int sumAbove = 0;
	int avgAbove = 0;
	int below = 0;
	int sumBelow = 0;
	int avgBelow = 0;

	float prev = 0;
	float current = 255;
	float epsilon = 0.1;
	while (abs(prev - current) > epsilon) {
		for (int i = 0; i < prog; i++) {
			below += histogram[i];
			sumBelow += histogram[i] * i;
		}
		for (int i = 255; i > prog; i--) {
			above += histogram[i];
			sumAbove += histogram[i] * i;
		}
		avgAbove = sumAbove / above;
		avgBelow = sumBelow / below;

		int t = (avgAbove + avgBelow) / 2;

		prev = current;
		current = t;
	}
	return current;
}

int BMP::ProgowanieGradientowe(BMP* bmp)
{
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
	int above = 0;
	int below = 0;
	for (int i = 1; i < bmp->GetWidth()-1; i++)
	{
		for (int j = 1; j < bmp->GetHeight()-1; j++)
		{			
			BYTE gradient = 0;
			BYTE gradientX = 0;
			BYTE gradientY = 0;

			gradientX = abs(bmp->GetPixel8(i + 1, j) - bmp->GetPixel8(i - 1, j));
			gradientY = abs(bmp->GetPixel8(i, j+1) - bmp->GetPixel8(i, j-1));
			gradient = max(gradientX, gradientY);	

			above += bmp->GetPixel8(i, j) * gradient;
			below += gradient;
		}
	}
	int prog = above / below;
	return prog;
}

void BMP::Potegowanie(BMP* bmp, float val)
{
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
    for (int i = 0; i < bmp->GetWidth(); i++)
    {
        for (int j = 0; j < bmp->GetHeight(); j++)
        {
            SetPixel8(i, j, pow((float)(bmp->GetPixel8(i,j))/255.0f, val)*255);
        }
    }    
}

void BMP::ZmienJasnosc(BMP* bmp, int val)
{
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
    for (int i = 0; i < bmp->GetWidth(); i++)
    {
        for (int j = 0; j < bmp->GetHeight(); j++)
        {
            int color = bmp->GetPixel8(i, j) + val;
            if ( color > 255) 
            {
                SetPixel8(i, j, 255);
            }
            else if ( color < 0)
            {
                SetPixel8(i, j, 0);                
            }
            else
            {
                SetPixel8(i, j, color);                
            }
        }
    }
}

void BMP::ZmienKontrast(BMP* bmp, float val)
{
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
    for (int i = 0; i < bmp->GetWidth(); i++)
    {
        for (int j = 0; j < bmp->GetHeight(); j++)
        {
            int color = (bmp->GetPixel8(i, j) - 127 ) * val + 127;
            if (color > 255)
            {
                SetPixel8(i, j, 255);
            }
            else if (color < 0)
            {
                SetPixel8(i, j, 0);
            }
            else
            {
                SetPixel8(i, j, color);
            }
        }
    }    
}

void BMP::WyrownywanieHistogramu(BMP* bmp) 
{
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
    int histogramColors[256] = { 0 };
    int nowaJasnosc = 0;
    float dystrybuanta[256] = { 0 };
    int sumaPunktowObrazu = imageRect.Width() * imageRect.Height();

    for (int x = 0; x < imageRect.Width(); x++) 
    {
        for (int y = 0; y < imageRect.Height(); y++) 
        {
            histogramColors[bmp->GetPixel8(x, y)]++;
        }
    }

    for (int i = 0; i < 256; i++) 
    {
        nowaJasnosc += histogramColors[i];
        dystrybuanta[i] = ( (float)nowaJasnosc * 255.0f ) / (float)sumaPunktowObrazu;
    }    
    
    for (int x = 0; x < imageRect.Width(); x++) 
    {
        for (int y = 0; y < imageRect.Height(); y++) 
        {
            SetPixel8(x, y, (int)dystrybuanta[bmp->GetPixel8(x, y)]);
        }
    }
}

//Lab4 - Filtry 

int AvgMask[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };

int GaussMask[3][3] = { { 1, 4, 1 }, { 4, 12, 4 }, { 1, 4, 1 } };

int SobelMask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

int InverseSobelMask[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };

int LaplasianMask[3][3] = { { -2, 1, -2 }, { 1, 4, 1 }, { -2, 1, -2 } };

int SharpMask[3][3] = { { 0, -1, 0 }, { -1, 5, -1 }, { 0, -1, 0 } };


void BMP::LinearFilter8(BMP* bmp, int maskID){

	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	int w = 0;
	bool additive = true;
	if (maskID == 1){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				w += AvgMask[j][i];
			}
		}
		for (int i = 1; i < bmp->GetWidth() - 1; i++)
		{
			for (int j = 1; j < bmp->GetHeight() - 1; j++)
			{
				int color = bmp->GetPixel8(i, j)		* AvgMask[1][1];
				color += bmp->GetPixel8(i, j - 1)		* AvgMask[1][0];
				color += bmp->GetPixel8(i, j + 1)		* AvgMask[1][2];
				color += bmp->GetPixel8(i - 1, j)		* AvgMask[0][0];
				color += bmp->GetPixel8(i - 1, j - 1)	* AvgMask[0][1];
				color += bmp->GetPixel8(i - 1, j + 1)	* AvgMask[0][2];
				color += bmp->GetPixel8(i + 1, j)		* AvgMask[2][0];
				color += bmp->GetPixel8(i + 1, j - 1)	* AvgMask[2][1];
				color += bmp->GetPixel8(i + 1, j + 1)	* AvgMask[2][2];

				if (additive == true && w > 0){
					color /= w;
					BYTE colorBYTE = BYTE(color);
				}
				else {
					if (color == 0){
						color += 127;
					}
					if (color > 255){
						color = 255;
					}
					BYTE colorBYTE = BYTE(color);
				}

				SetPixel8(i, j, color);
			}
		}
	}
	else if (maskID == 2){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				w += GaussMask[j][i];
			}
		}
		for (int i = 1; i < bmp->GetWidth() - 1; i++)
		{
			for (int j = 1; j < bmp->GetHeight() - 1; j++)
			{
				int color = bmp->GetPixel8(i, j)		* GaussMask[1][1];
				color += bmp->GetPixel8(i, j - 1)		* GaussMask[1][0];
				color += bmp->GetPixel8(i, j + 1)		* GaussMask[1][2];
				color += bmp->GetPixel8(i - 1, j)		* GaussMask[0][0];
				color += bmp->GetPixel8(i - 1, j - 1)	* GaussMask[0][1];
				color += bmp->GetPixel8(i - 1, j + 1)	* GaussMask[0][2];
				color += bmp->GetPixel8(i + 1, j)		* GaussMask[2][0];
				color += bmp->GetPixel8(i + 1, j - 1)	* GaussMask[2][1];
				color += bmp->GetPixel8(i + 1, j + 1)	* GaussMask[2][2];

				if (additive == true && w > 0){
					color /= w;
					BYTE colorBYTE = BYTE(color);
				}
				else {
					if (color == 0){
						color += 127;
					}
					if (color > 255){
						color = 255;
					}
					BYTE colorBYTE = BYTE(color);
				}
				SetPixel8(i, j, color);
			}
		}
	}
	else if (maskID == 3){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				w += SobelMask[j][i];
			}
		}
		additive = false;
		for (int i = 1; i < bmp->GetWidth() - 1; i++)
		{
			for (int j = 1; j < bmp->GetHeight() - 1; j++)
			{
				int color = bmp->GetPixel8(i, j)		* SobelMask[1][1];
				color += bmp->GetPixel8(i, j - 1)		* SobelMask[1][0];
				color += bmp->GetPixel8(i, j + 1)		* SobelMask[1][2];
				color += bmp->GetPixel8(i - 1, j)		* SobelMask[0][0];
				color += bmp->GetPixel8(i - 1, j - 1)	* SobelMask[0][1];
				color += bmp->GetPixel8(i - 1, j + 1)	* SobelMask[0][2];
				color += bmp->GetPixel8(i + 1, j)		* SobelMask[2][0];
				color += bmp->GetPixel8(i + 1, j - 1)	* SobelMask[2][1];
				color += bmp->GetPixel8(i + 1, j + 1)	* SobelMask[2][2];

				color += 127;
				if (color > 255){
					color = 255;
				}
				if (color < 0){
					color = 0;
				}
				BYTE colorBYTE = BYTE(color);

				SetPixel8(i, j, color);
			}
		}
	}
	else if (maskID == 4){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				w += InverseSobelMask[j][i];
			}
		}
		additive = false;
		for (int i = 1; i < bmp->GetWidth() - 1; i++)
		{
			for (int j = 1; j < bmp->GetHeight() - 1; j++)
			{
				int color = bmp->GetPixel8(i, j)		* InverseSobelMask[1][1];
				color += bmp->GetPixel8(i, j - 1)		* InverseSobelMask[1][0];
				color += bmp->GetPixel8(i, j + 1)		* InverseSobelMask[1][2];
				color += bmp->GetPixel8(i - 1, j)		* InverseSobelMask[0][0];
				color += bmp->GetPixel8(i - 1, j - 1)	* InverseSobelMask[0][1];
				color += bmp->GetPixel8(i - 1, j + 1)	* InverseSobelMask[0][2];
				color += bmp->GetPixel8(i + 1, j)		* InverseSobelMask[2][0];
				color += bmp->GetPixel8(i + 1, j - 1)	* InverseSobelMask[2][1];
				color += bmp->GetPixel8(i + 1, j + 1)	* InverseSobelMask[2][2];
				
				color += 127;
				if (color > 255){
					color = 255;
				}
				if (color < 0){
					color = 0;
				}
				BYTE colorBYTE = BYTE(color);

				SetPixel8(i, j, color);
			}
		}
	}
	else if (maskID == 5){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				w += LaplasianMask[j][i];
			}
		}
		for (int i = 1; i < bmp->GetWidth() - 1; i++)
		{
			for (int j = 1; j < bmp->GetHeight() - 1; j++)
			{
				int color = bmp->GetPixel8(i, j)		* LaplasianMask[1][1];
				color += bmp->GetPixel8(i, j + 1)		* LaplasianMask[2][1];
				color += bmp->GetPixel8(i, j - 1)		* LaplasianMask[0][1];
				color += bmp->GetPixel8(i + 1, j + 1)	* LaplasianMask[2][2];
				color += bmp->GetPixel8(i - 1, j - 1)	* LaplasianMask[0][0];
				color += bmp->GetPixel8(i - 1, j)		* LaplasianMask[1][0];
				color += bmp->GetPixel8(i - 1, j + 1)	* LaplasianMask[2][0];
				color += bmp->GetPixel8(i + 1, j)		* LaplasianMask[0][2];
				color += bmp->GetPixel8(i + 1, j - 1)	* LaplasianMask[1][2];

				color += 127;
				if (color > 255){
					color = 255;
				}
				if (color < 0){
					color = 0;
				}

				SetPixel8(i, j, color);
			}
		}
	}
	else if (maskID == 6){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				w += SharpMask[j][i];
			}
		}
		additive = false;
		for (int i = 1; i < bmp->GetWidth() - 1; i++)
		{
			for (int j = 1; j < bmp->GetHeight() - 1; j++)
			{
				int color = bmp->GetPixel8(i, j)		* SharpMask[1][1];
				color += bmp->GetPixel8(i, j - 1)		* SharpMask[1][0];
				color += bmp->GetPixel8(i, j + 1)		* SharpMask[1][2];
				color += bmp->GetPixel8(i - 1, j)		* SharpMask[0][0];
				color += bmp->GetPixel8(i - 1, j - 1)	* SharpMask[0][1];
				color += bmp->GetPixel8(i - 1, j + 1)	* SharpMask[0][2];
				color += bmp->GetPixel8(i + 1, j)		* SharpMask[2][0];
				color += bmp->GetPixel8(i + 1, j - 1)	* SharpMask[2][1];
				color += bmp->GetPixel8(i + 1, j + 1)	* SharpMask[2][2];
				
				if (color > 255){
					color = 255;
				}
				if (color < 0){
					color = 0;
				}
				BYTE colorBYTE = BYTE(color);
				SetPixel8(i, j, color);
			}
		}
	}
	
}

void BMP::Median3x3Filter8(BMP* bmp){

	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	int colors[9] = { 0 };

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			int color = 0;
			colors[3] = bmp->GetPixel8(i - 1, j);
			colors[4] = bmp->GetPixel8(i - 1, j - 1);
			colors[5] = bmp->GetPixel8(i - 1, j + 1);
			colors[0] = bmp->GetPixel8(i, j);
			colors[1] = bmp->GetPixel8(i, j - 1)		;
			colors[2] = bmp->GetPixel8(i, j + 1)		;
			colors[6] = bmp->GetPixel8(i + 1, j)		;
			colors[7] = bmp->GetPixel8(i + 1, j - 1)	;
			colors[8] = bmp->GetPixel8(i + 1, j + 1)	;
			
			std::sort(colors, colors + 9);

			BYTE colorBYTE = BYTE(colors[4]);

			SetPixel8(i, j, colorBYTE);
		}
	}
}

void BMP::MedianCrossFilter8(BMP* bmp){

	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	int colors[5] = { 0 };

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			int color = 0;
			colors[0] = bmp->GetPixel8(i - 1, j);
			colors[1] = bmp->GetPixel8(i, j);
			colors[2] = bmp->GetPixel8(i, j - 1);
			colors[3] = bmp->GetPixel8(i, j + 1);
			colors[4] = bmp->GetPixel8(i + 1, j);

			std::sort(colors, colors + 5);

			BYTE colorBYTE = BYTE(colors[2]);

			SetPixel8(i, j, colorBYTE);
		}
	}
}

void BMP::Median5x5Filter8(BMP* bmp){
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	int colors[25] = { 0 };

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			int color = 0;
			for (int k = 0; k < 5; k++){
				for (int h = 0; h < 5; h++){
					colors[5*k+h] = bmp->GetPixel8(i + h - 2, j + k - 2);
				}
			}

			std::sort(colors, colors + 25);

			BYTE colorBYTE = BYTE(colors[12]);

			SetPixel8(i, j, colorBYTE);
		}
	}
}

void BMP::FiltrLoG(BMP* bmp, float delta) {
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

    int maksWidth = 1 + 2 * ceil(2.5 * delta);

    float** mask = new float*[maksWidth];
    for (int i = 0; i < maksWidth; ++i)
        mask[i] = new float[maksWidth];

    float e = 2.7182818284590452353602874713527;
    float delta2 = delta * delta;
    float delta4 = delta2 * delta2;

    int offset = (maksWidth - 1) / 2;
    for (int _y = -offset; _y <= offset; ++_y) {
        for (int _x = -offset; _x <= offset; ++_x) {
            float x2 = _x * _x;
            float y2 = _y * _y;

            float power = (x2 + y2) / (2 * delta2) * -1;
            float ePow = pow(e, power);

            float base = -1 * (x2 + y2 + delta2) / delta4;

            float result = base * ePow;
            mask[_x + offset][_y + offset] = result;
        }
    }

    float suma = 0.0f;
    for (int _y = -offset; _y <= offset; ++_y) {
        for (int _x = -offset; _x <= offset; ++_x) {
            suma += mask[_x + offset][_y + offset];
        }
    }
    for (int _y = -offset; _y <= offset; ++_y) {
        for (int _x = -offset; _x <= offset; ++_x) {
            mask[_x + offset][_y + offset] = mask[_x + offset][_y + offset] / suma;
            mask[_x + offset][_y + offset] -= 1.0f / (float)(maksWidth*maksWidth);
        }
    }

    for (int x = 0; x < bmp->GetWidth(); ++x) {
        for (int y = 0; y < bmp->GetHeight(); ++y) {
            SetPixel8(x, y, 255);
        }
    }

    for (int x = offset; x < bmp->GetWidth() - offset; ++x) {
        for (int y = offset; y < bmp->GetHeight() - offset; ++y) {

            float sum2 = 0.0f;
            for (int _y = -offset; _y <= offset; ++_y) {
                for (int _x = -offset; _x <= offset; ++_x) {
                    float pixel = (float)bmp->GetPixel8(x - _x, y - _y);
                    pixel *= mask[_x + offset][_y + offset];
                    sum2 += pixel;
                }
            }

            if (sum2 + 4.0f < 0) {
                SetPixel8(x, y, 0);
            }

        }
    }
}

//Lab5 - Szkieletyzacja

void BMP::Pavlidis(BMP* bmp){
    //in PO1DLg.cpp
}

void BMP::MapaOdleglosci(BMP* bmp, bool mapIt, float sliderVal)
{
    CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
    CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

    int** distances = new int*[bmp->GetWidth()];
    for (int i = 0; i < bmp->GetWidth(); ++i) {
        distances[i] = new int[bmp->GetHeight()];
    }
    int maxCounter = 0;

    for (int y = 0; y < bmp->GetHeight(); ++y) {
        for (int x = 0; x < bmp->GetWidth(); ++x) {
            BYTE pixel = bmp->GetPixel8(x, y);
            if (pixel == 0) {
                BYTE pixel2 = bmp->GetPixel8(x, y);
                int counter = 0;
                int movX = 0;
                int movY = 0;
                int dir = 0; // 0r, 1d, 2l, 3u
                int s = 1;
                int _x = x, _y = y;

                while (pixel2 == 0) {
                    for (int j = 0; j < 2; j++)
                    {
                        for (int i = 0; i < s; i++)
                        {
                            pixel2 = bmp->GetPixel8(_x, _y);
                            counter++;
                            if (pixel2 == 255)break;

                            switch (dir)
                            {
                            case 0: _y++; break;
                            case 1: _x++; break;
                            case 2: _y--; break;
                            case 3: _x--; break;
                            }
                        }
                        dir = (dir + 1) % 4;
                        if (pixel2 == 255)break;
                    }
                    s = s + 1;
                }
                distances[x][y] = counter;
                if (counter > maxCounter)maxCounter = counter;
            }
        }
    }

    for (int y = 0; y < bmp->GetHeight(); ++y) {
        for (int x = 0; x < bmp->GetWidth(); ++x) {
            if (mapIt == true)
            {
                SetPixel8(x, y, 255 - 255.0f*(float)distances[x][y] / (float)maxCounter);
            }
            else {
                if (100.0f*(float)distances[x][y] / (float)maxCounter > sliderVal) {
                    SetPixel8(x, y, 0);
                }
                else {
                    SetPixel8(x, y, 255);
                }
            }
        }
    }
}

void BMP::Dylatacja(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			SetPixel8(i, j, 255);
		}
	}

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i,j);
			if (color == 0) {
				SetPixel8(i, j, color);
				SetPixel8(i, j-1, color);
				SetPixel8(i, j+1, color);
				SetPixel8(i-1, j-1, color);
				SetPixel8(i-1, j, color);
				SetPixel8(i-1, j+1, color);
				SetPixel8(i+1, j, color);
				SetPixel8(i+1, j-1, color);
				SetPixel8(i+1, j+1, color);
			}
		}
	}
}
void BMP::Erozja(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
	
	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			SetPixel8(i, j, 0);
		}
	}

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			if (color == 255) {
				SetPixel8(i, j, color);
				SetPixel8(i, j - 1, color);
				SetPixel8(i, j + 1, color);
				SetPixel8(i - 1, j - 1, color);
				SetPixel8(i - 1, j, color);
				SetPixel8(i - 1, j + 1, color);
				SetPixel8(i + 1, j, color);
				SetPixel8(i + 1, j - 1, color);
				SetPixel8(i + 1, j + 1, color);
			}
		}
	}
}
void BMP::Otwarcie(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			if (color == 255) {
				tmpBMP.SetPixel8(i, j, color);
				tmpBMP.SetPixel8(i, j - 1, color);
				tmpBMP.SetPixel8(i, j + 1, color);
				tmpBMP.SetPixel8(i - 1, j - 1, color);
				tmpBMP.SetPixel8(i - 1, j, color);
				tmpBMP.SetPixel8(i - 1, j + 1, color);
				tmpBMP.SetPixel8(i + 1, j, color);
				tmpBMP.SetPixel8(i + 1, j - 1, color);
				tmpBMP.SetPixel8(i + 1, j + 1, color);
			}
			else {
				tmpBMP.SetPixel8(i, j, 0);
			}
		}
	}

	for (int i = 1; i < tmpBMP.GetWidth() - 1; i++)
	{
		for (int j = 1; j < tmpBMP.GetHeight() - 1; j++)
		{
			BYTE color = tmpBMP.GetPixel8(i, j);
			if (color == 0) {
				SetPixel8(i, j, color);
				SetPixel8(i, j - 1, color);
				SetPixel8(i, j + 1, color);
				SetPixel8(i - 1, j - 1, color);
				SetPixel8(i - 1, j, color);
				SetPixel8(i - 1, j + 1, color);
				SetPixel8(i + 1, j, color);
				SetPixel8(i + 1, j - 1, color);
				SetPixel8(i + 1, j + 1, color);
			}
			else {
				SetPixel8(i, j, 255);
			}
		}
	}
}
void BMP::Zamkniecie(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);
	
	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			if (color == 0) {
				tmpBMP.SetPixel8(i, j, color);
				tmpBMP.SetPixel8(i, j - 1, color);
				tmpBMP.SetPixel8(i, j + 1, color);
				tmpBMP.SetPixel8(i - 1, j - 1, color);
				tmpBMP.SetPixel8(i - 1, j, color);
				tmpBMP.SetPixel8(i - 1, j + 1, color);
				tmpBMP.SetPixel8(i + 1, j, color);
				tmpBMP.SetPixel8(i + 1, j - 1, color);
				tmpBMP.SetPixel8(i + 1, j + 1, color);
			}
			else {
				tmpBMP.SetPixel8(i, j, 255);
			}
		}
	}

	for (int i = 1; i < tmpBMP.GetWidth() - 1; i++)
	{
		for (int j = 1; j < tmpBMP.GetHeight() - 1; j++)
		{
			BYTE color = tmpBMP.GetPixel8(i, j);
			if (color == 255) {
				SetPixel8(i, j, color);
				SetPixel8(i, j - 1, color);
				SetPixel8(i, j + 1, color);
				SetPixel8(i - 1, j - 1, color);
				SetPixel8(i - 1, j, color);
				SetPixel8(i - 1, j + 1, color);
				SetPixel8(i + 1, j, color);
				SetPixel8(i + 1, j - 1, color);
				SetPixel8(i + 1, j + 1, color);
			}
			else {
				SetPixel8(i, j, 0);
			}
		}
	}	
}
void BMP::Dylatacja8(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	for (int i = 0; i < bmp->GetWidth(); i++)
	{
		for (int j = 0; j < bmp->GetHeight(); j++)
		{
			SetPixel8(i, j, 255);
		}
	}

	BYTE colors[9] = { 0 };

	for (int i = 0; i < bmp->GetWidth(); i++)
	{
		for (int j = 0; j < bmp->GetHeight(); j++)
		{
			if (i == 0 || i == bmp->GetWidth() - 1 || j == 0 || j == bmp->GetHeight() - 1) {
				SetPixel8(i, j, bmp->GetPixel8(i,j));
				continue;
			}

			colors[0] = bmp->GetPixel8(i, j);
			colors[1] = bmp->GetPixel8(i, j - 1);
			colors[2] = bmp->GetPixel8(i, j + 1);
			colors[3] = bmp->GetPixel8(i - 1, j - 1);
			colors[4] = bmp->GetPixel8(i - 1, j);
			colors[5] = bmp->GetPixel8(i - 1, j + 1);
			colors[6] = bmp->GetPixel8(i + 1, j);
			colors[7] = bmp->GetPixel8(i + 1, j - 1);
			colors[8] = bmp->GetPixel8(i + 1, j + 1);

			std::sort(colors, colors + 9);
			SetPixel8(i, j, colors[0]);
		}
	}
}
void BMP::Erozja8(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			SetPixel8(i, j, 0);
		}
	}

	BYTE colors[9] = { 0 };

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			colors[0] = bmp->GetPixel8(i, j);
			colors[1] = bmp->GetPixel8(i, j - 1);
			colors[2] = bmp->GetPixel8(i, j + 1);
			colors[3] = bmp->GetPixel8(i - 1, j - 1);
			colors[4] = bmp->GetPixel8(i - 1, j);
			colors[5] = bmp->GetPixel8(i - 1, j + 1);
			colors[6] = bmp->GetPixel8(i + 1, j);
			colors[7] = bmp->GetPixel8(i + 1, j - 1);
			colors[8] = bmp->GetPixel8(i + 1, j + 1);

			std::sort(colors, colors + 9);
			SetPixel8(i, j, colors[8]);
		}
	}
}
void BMP::Otwarcie8(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);

	tmpBMP.Erozja8(bmp);
	Dylatacja8(&tmpBMP);
}
void BMP::Zamkniecie8(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);

	tmpBMP.Dylatacja8(bmp);
	Erozja8(&tmpBMP);
}
void BMP::KonturZewnetrzny(BMP* bmp){
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			SetPixel8(i, j, 255);
		}
	}

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			if (color == 0) {
				SetPixel8(i, j, color);
				SetPixel8(i, j - 1, color);
				SetPixel8(i, j + 1, color);
				SetPixel8(i - 1, j - 1, color);
				SetPixel8(i - 1, j, color);
				SetPixel8(i - 1, j + 1, color);
				SetPixel8(i + 1, j, color);
				SetPixel8(i + 1, j - 1, color);
				SetPixel8(i + 1, j + 1, color);
			}
		}
	}

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			if (color == 0) {
				SetPixel8(i, j, 255);
			}
		}
	}
}
void BMP::KonturWewnetrzny(BMP* bmp){
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);
	
	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			BYTE color = bmp->GetPixel8(i, j);
			SetPixel8(i, j, color);
		}
	}

	tmpBMP.Erozja(bmp);

	for (int i = 0; i < bmp->GetWidth(); i++)
	{
		for (int j = 0; j < bmp->GetHeight(); j++)
		{
			BYTE color = tmpBMP.GetPixel8(i, j);
			if (color == 0) {
				SetPixel8(i, j, 255);
			}
		}
	}

}
void BMP::WhiteTopHat(BMP* bmp) {
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);

	tmpBMP.Otwarcie8(bmp);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			int color = bmp->GetPixel8(i, j);
			int color2 = tmpBMP.GetPixel8(i, j);
			color = color - color2;
			SetPixel8(i, j, (BYTE)color);
		}
	}
}
void BMP::BlackTopHat(BMP* bmp){
	CRect imageRect(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CRect imageRect2(0, 0, bmp->GetWidth(), bmp->GetHeight());
	CreateGreyscaleDIB(imageRect, 0, 0, bmp->biBitCount);

	BMP tmpBMP;
	tmpBMP.CreateGreyscaleDIB(imageRect2, 0, 0, bmp->biBitCount);

	tmpBMP.Zamkniecie8(bmp);

	for (int i = 1; i < bmp->GetWidth() - 1; i++)
	{
		for (int j = 1; j < bmp->GetHeight() - 1; j++)
		{
			int color = GetPixel8(i, j);
			int color2 = bmp->GetPixel8(i, j);
			color = color - color2;
			SetPixel8(i, j, (BYTE)color);
		}
	}
}

void BMP::Hough(BMP* bmp, int sliderVal)
{
    //In Po1Dlg.cpp
}

void BMP::ClearAccPixel(int x, int y, int max) {
    if (x >= 0 && y >= 0 && x < GetWidth() && y < GetHeight()) {
        if (255 - GetPixel8(x, y) > 25) {
            SetPixel8(x, y, 255);
            ClearAccPixel( x - 1, y, max);
            ClearAccPixel( x + 1, y, max);
            ClearAccPixel( x, y - 1, max);
            ClearAccPixel( x, y + 1, max);
        }
    }
}