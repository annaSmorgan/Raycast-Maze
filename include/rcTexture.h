#pragma once
#ifndef _TEXTURE_H
#define _TEXTURE_H
//includes
#include <stdafx.h>

class Texture
{
public:
	Texture(); //constructor
	~Texture(); //deconstructor
	//load texture function
	bool Load(const char* a_fileName, u32 a_format);
	//getter functions
	const std::string& GetFileName() const { return m_fileName; }
	void GetDimensions(u32& a_width, u32& a_height) const;
	u8 GetBPP() { return m_bpp; }
	u16 GetPalette(void* a_palette);
	void* GetPixelData() { return m_pixelData; }

private:
	//variables
	std::string m_fileName;
	u32 m_width;
	u32 m_height;
	u16 m_paletteSize;
	u8 m_bpp;
	void* m_palette;
	void* m_pixelData;
};

#endif // !_TEXTURE_H
