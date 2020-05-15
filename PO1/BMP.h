#pragma once
class BMP
{
private:
	void* wskaznik_na_bufor = nullptr;
	void* wskaznik_na_linie = nullptr;
	int lineLength = 0;
    int height = 0;
    int width = 0; 
    int ih_to_bmp_px_offset = 0;
	tagBITMAPFILEHEADER bmpFileHeader;
	tagBITMAPINFOHEADER bmpInfoHeader;
	tagBITMAPINFO bmpInfo;

public:
	BMP();
	~BMP();
	bool LoadDIB(CString sciezka_do_pliku);
	bool PaintDIB(HDC kontekst, CRect prost_docelowy, CRect prost_zrodlowy);
	bool CreateGreyscaleDIB(CRect rozmiar_obrazu, int xPPM, int yPPM, WORD biBitCount);
	bool GetPixel1(int x, int y);
	BYTE GetPixel8(int x, int y);
	RGBTRIPLE GetPixel24(int x, int y);
	bool SetPixel8(int x, int y, BYTE val);
	bool SaveDIB(CString sciezka_do_pliku);
	int GetHeight();
	int GetWidth();
	WORD biBitCount = 0;
    void ZmienJasnosc(BMP* bmp, int val);
    void ZmienKontrast(BMP* bmp, float val);
    void Negatyw(BMP* bmp);
	void Binaryzacja(BMP* bmp, int prog);
    void Potegowanie(BMP* bmp, float val);
    void WyrownywanieHistogramu(BMP* bmp);
    int ProgowanieIteracyjne(BMP* bmp);
    int ProgowanieGradientowe(BMP* bmp);
    int ProgowanieOtsu(BMP* bmp); 
    void ProgowanieLokalne(BMP* bmp);
	void LinearFilter8(BMP* bmp, int mask);
	void BMP::Median3x3Filter8(BMP* bmp);
	void BMP::MedianCrossFilter8(BMP* bmp);
	void BMP::Median5x5Filter8(BMP* bmp);
    void BMP::FiltrLoG(BMP* bmp, float delta);
	void BMP::Pavlidis(BMP* bmp);
	void BMP::MapaOdleglosci(BMP* bmp, bool mapIt, float sliderVal);
	void BMP::Dylatacja(BMP* bmp);
	void BMP::Erozja(BMP* bmp);
	void BMP::Dylatacja8(BMP* bmp);
	void BMP::Erozja8(BMP* bmp);
	void BMP::Otwarcie(BMP* bmp);
	void BMP::Otwarcie8(BMP* bmp);
	void BMP::Zamkniecie(BMP* bmp);
	void BMP::Zamkniecie8(BMP* bmp);
	void BMP::KonturZewnetrzny(BMP* bmp);
	void BMP::KonturWewnetrzny(BMP* bmp);
	void BMP::WhiteTopHat(BMP* bmp);
	void BMP::BlackTopHat(BMP* bmp);
    void BMP::Hough(BMP* bmp, int sliderVal);
    void BMP::ClearAccPixel(int x, int y, int max);
};