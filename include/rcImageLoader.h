#pragma once
#ifndef _IMAGE_LOADER_H
#define _IMAGE_LOADER_H
//includes
#include <stdafx.h>

typedef enum rc_imageType //enum for possible image types
{
	IM_PCX,
	IM_PPM,
	IM_BITMAP,

	IM_MAX
}rc_imageType;

class ImageLoader
{
public:
	ImageLoader() {}; //constructor
	~ImageLoader() {}; //deconstructor
	//load image function
	static void* LoadFromFile(const char* a_fileName, u32 a_type, u32& a_w,
	u32& a_h, u8& a_bpp, void*& a_imgPallete);
};


#endif // !_IMAGE_LOADER_H





