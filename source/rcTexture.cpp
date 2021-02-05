//includes
#include <stdafx.h>
#include "rcTexture.h"
#include "rcImageLoader.h"
//constructor initalising variables 
Texture::Texture() :
	m_fileName(""), m_width(0), m_height(0), m_bpp(0), m_paletteSize(0),
	m_palette(nullptr), m_pixelData(nullptr)
{}
Texture::~Texture() //deconstructor deleting and setting to null if not null
{
	if (m_palette) { delete[] m_palette; m_palette = nullptr; }
	if (m_pixelData) { delete[] m_pixelData; m_pixelData = nullptr; }
}

bool Texture::Load(const char* a_fileName, u32 a_format)
{
	m_pixelData =
		ImageLoader::LoadFromFile(a_fileName, a_format, m_width, m_height,
			m_bpp, m_palette); //palette size missing as not needed 
	if (m_pixelData)
	{
		m_fileName = a_fileName; //setting the file to the parameter file
		return true;
	}
	m_width = m_height = 0; //reseting data
	m_bpp = 0;
	m_paletteSize = 0;
	return false;
}
//getting the dimensions of texture
void Texture::GetDimensions(u32& a_width, u32& a_height) const
{
	a_width = m_width;
	a_height = m_height;
}
//getting the palette of texture
u16 Texture::GetPalette(void* a_palette)
{
	a_palette = m_palette;
	return m_paletteSize;
}

