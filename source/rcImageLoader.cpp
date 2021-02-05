//includes
#include <stdafx.h>
#include "rcImageLoader.h"
#include "rcPCXLoader.h"

void* ImageLoader::LoadFromFile(const char* a_fileName, u32 a_type, u32& a_w,
	u32& a_h, u8& a_bpp, void*& a_imgPallete)
{
	UNREFERENCED_PARAMETER(a_w);
	UNREFERENCED_PARAMETER(a_h);
	UNREFERENCED_PARAMETER(a_bpp);
	//UNREFERENCED_PARAMETER(a_paletteSize);
	UNREFERENCED_PARAMETER(a_imgPallete);
	//new fstream
	std::fstream file;
	//opening the called file
	file.open(a_fileName, std::ios_base::in | std::ios_base::binary);
	if (file.is_open()) //if the file is open
	{
		file.ignore(std::numeric_limits<std::streamsize>::max()); //ignore limits and max
		std::streamsize fileLength = file.gcount();//creates the file size as a variable
		file.clear();//clear error flags from being read
		file.seekg(0, std::ios_base::beg);//set to begining
		if (fileLength == 0) //if length 0
		{
			file.close();//close file
			return nullptr;//end 
		}
		void* imageData = nullptr; //set pointer to null

		switch(a_type)//switch on the image type
		{
		case IM_PCX:
		{
			imageData =PCXLoader::LoadFromFile(&file, a_w, a_h, a_bpp, a_imgPallete);//run load from gile function from pcx loader class
			if (a_bpp != 32) //if bits per pixel isnt 32
			{//convert image to have 32 bits per pixel
				imageData = PCXLoader::ConvertTo32bpp(imageData, a_imgPallete, a_w, a_h, a_bpp);
			}
			break;
		}
		case IM_BITMAP: //possible bitmap loading
		{
			break;
		}

		default:
		{
			file.close(); //close file
			return nullptr; //sset to null
		}
		}

		file.close(); //close file
		return imageData; //return image
	}

	return nullptr; //return null
}