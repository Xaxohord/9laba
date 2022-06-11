#pragma once
#include <string>
#include <fstream>
#include "../inc/mathutils/matrix.hpp"
#include "../inc/FunctionsHeader.hpp"
using namespace mtmath;
struct Pixel
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};
#pragma pack()

class BMP {
public:
	BMP();
	BMP(int h, int w);
    void Read();
   // void Pixels();
    void DarkFilter();
    void Write();
    void Clean();
	void Rotate(double angle);
	void Correct();

private:
    int m_width, m_height;
    Pixel** m_pixels;
	Vec2d** m_coordinates = nullptr;

#pragma pack(1) 
    struct BMPHEADER
    {
        unsigned short    Type;
        unsigned int      Size;
        unsigned short    Reserved1;
        unsigned short    Reserved2;
        unsigned int      OffBits;
    };

#pragma pack()

#pragma pack(1)
    struct BMPINFO
    {
        unsigned int    Size;
        int             Width;
        int             Height;
        unsigned short  Planes;
        unsigned short  BitCount;
        unsigned int    Compression;
        unsigned int    SizeImage;
        int             XPelsPerMeter;
        int             YPelsPerMeter;
        unsigned int    ClrUsed;
        unsigned int    ClrImportant;
    };
#pragma pack()
    BMPHEADER bmpHeader;
    BMPINFO bmpInfo;
};