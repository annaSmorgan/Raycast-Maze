#pragma once
#ifndef _PCXLOADER_
#define _PCXLOADER_
//includes
#include <stdafx.h>

#define PCX_VALID_HEADER 10
#define PCX_RLE_ENCODING 1
#define PCX_VGA_PALLETE_OFFSET -769
#define PCX_RLE_MASK 0xC0 //ob1100 0000
#define PCX_RLE_FREQ_MASK 0x3F // 0b0011 1111

//default ega pallette (version 3)
const u8 PCX_defaultPallete[48] = 
{
	 0x00, 0x00, 0x00,	0x00,0x00,0x80,	0x00,0x80,0x00,
	 0x00, 0x80, 0x80,	0x80,0x00,0x00,	0x80,0x00,0x80,
	 0x80,0x80,0x00,	0x80,0x80,0x80,	0xC0,0xC0, 0xC0,
	 0x00,0x00,0xFF,	0x00,0xFF,0x00, 0x00,0xFF,0xFF,
	 0xFF,0x00,0x00,	0xFF,0x00,0xFF,	0xFF,0xFF,0x00,
	 0xFf, 0xFF,0xFF
};

struct PCXHeader //struct containing pcx image data
{
	u8 identifier;
	u8 version;
	u8 encoding;
	u8 bitsperpixel;
	struct PCXDimensions { u16 left; u16 top; u16 right; u16 bottom; }
	dimensions;
	u16 hRes;
	u16 vRes;
	struct PCXPalleteColour { u8 R; u8 G; u8 B; }
	colourPallete[16];
	u8 reservedBytes;
	u8 numColourPlanes;
	u16 BytesPerScanline;
	u16 palleteInfo;
	u16 hScreenRes;
	u16 vScreenRes;
	u8 padding[54];
};

class PCXLoader
{
public:
	PCXLoader() {}; //constructor
	~PCXLoader() {}; //deconstructor
	//functions
	static void* LoadFromFile(std::fstream* a_stream, u32& a_w, u32& a_h, u8& a_bpp, void*& a_imgPallete);
	static void* ConvertTo32bpp(void* a_imageData, void* a_palette, u32& a_w, u32& a_h, u8& a_bpp);
};

#endif //_PCXLOADER_
