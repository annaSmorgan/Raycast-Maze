//includes
#include <stdafx.h>
#include "rcPCXLoader.h"

int PCX_getEncodedByte(u8& a_value, u8& a_frequency, std::fstream* a_stream)
{
	if (a_stream->peek() == EOF) //test for unexpectefd end of file
	{
		return EOF;
	}
	a_frequency = 1; //default frequency of 1
	a_stream->read((char*)(&a_value), 1); //read in one character from file
	if ((a_value & PCX_RLE_MASK) == PCX_RLE_MASK) //test if run length frequency
	{
		a_frequency = a_value & PCX_RLE_FREQ_MASK; //use a frenquency mask to convert
		if (a_stream->peek() == EOF)//test for unexpectefd end of file
		{
			return EOF;
		}
		a_stream->read((char*)(&a_value), 1); // read next byte to get pixel value
	}
	return 0; //end function
}

void* PCXLoader::LoadFromFile(std::fstream* a_stream, u32& a_w, u32& a_h, u8& a_bpp, void*& a_imgPallete)
{
	UNREFERENCED_PARAMETER(a_w);
	UNREFERENCED_PARAMETER(a_h);
	UNREFERENCED_PARAMETER(a_bpp);
	UNREFERENCED_PARAMETER(a_imgPallete);

	std::fstream* file = (std::fstream*)(a_stream); //creating new fstream and setting it to fstream parameter
	PCXHeader header; //instance of struct
	u32 sizeOfHeader = sizeof(PCXHeader); //create a int for header size

	file->read((char*)(&header), sizeOfHeader);//read the header 
	//test the identifier and encoding bytes
	if (header.identifier != PCX_VALID_HEADER || header.encoding != PCX_RLE_ENCODING)
	{
		file->close();//close file
		return nullptr; //return null pointer
	}
	if (header.version == 3) //check is version 3
	{
		//version 3 has no pallete data and uses default pallete
		u8* egaPallete = (u8*)(header.colourPallete);
		for (int i = 0; i < 48; i++)
		{
			egaPallete[i] = PCX_defaultPallete[i];
		}
	}

	if (header.bitsperpixel == 8)
	{
		//there is no palette data if there is less than 8 bpp
		file->seekg(PCX_VGA_PALLETE_OFFSET, std::ios_base::end);//seek to the palette offset location
		char palleteIndicator = 0;
		file->read(&palleteIndicator, 1); //read in the signle byte at the offset location
		if (palleteIndicator == 0xC) //if the byte is 0xc than this image VGA palette data
		{//palette at end of file proceed to read palette data from file
			a_imgPallete = new PCXHeader::PCXPalleteColour[256];
			file->read((char*)(a_imgPallete), 256 * sizeof(PCXHeader::PCXPalleteColour));
		}
		file->clear(); //clear flags
		file->seekg(0, std::ios_base::beg); //set to begining
		file->seekg(sizeof(PCXHeader), std::ios_base::beg); //seek from start to end of header
	}//if we get here and a_palette is still null
	if (!a_imgPallete && (header.numColourPlanes * header.bitsperpixel) < 24)
	{
		a_imgPallete = new PCXHeader::PCXPalleteColour[16];//allocate memory for 16 colour pallete
		memcpy(a_imgPallete, header.colourPallete, 48);//copy to palette buffer
	}
	//get pixel size of image
	a_w = header.dimensions.right - header.dimensions.left + 1; //width in pixels
	a_h = header.dimensions.bottom - header.dimensions.top + 1; //number of scanlines in image (pixel height)
	a_bpp = header.bitsperpixel * header.numColourPlanes;
	//size of decomposed image in bytes
	u32 bytesInRow = (u32)(a_w * (float)(a_bpp / 8.f));
	u32 decompImageSize = a_h * bytesInRow;
	//process image data by decompress image scanline 
	u32 decompScanLine = header.BytesPerScanline * header.numColourPlanes;
	//calculate line padding
	u32 scanlinePadding = decompScanLine - bytesInRow;
	u32 actualBytesPerImageRow = decompScanLine - scanlinePadding;
	//create a data buffer large enough to hold decompressed image
	u8* imageData = new u8[decompImageSize];
	memset(imageData, 0, decompImageSize);
	u8* scanLineData = new u8[decompScanLine];
	memset(scanLineData, 0, decompScanLine);
	//stack values to hold value and frequency
	u8 value = 0;//current pixel value to be decoded
	u8 frequency = 0; //the frequency of this pixel value
	//while all of the image data has not been extracted
	u32 bytesProcessed = 0;
	std::streamsize streamLocation;
	u32 row = 0;

	while (row < a_h - 1)
	{
		streamLocation = file->tellg();
		//for each row of the image decode the compress image data
		for (u8* slp = scanLineData; slp < (scanLineData + decompScanLine);)
		{//test for premature end of file
			if (EOF == PCX_getEncodedByte(value, frequency, file))
			{//if file ends release and null data
				delete[] imageData;
				imageData = nullptr;
				if (!a_imgPallete)
				{
					delete[] a_imgPallete;
					a_imgPallete = nullptr;
				}
				return imageData;
			}

			//for the number of runs insert value into image data
			for (u8 i = 0; i < frequency; i++)
			{
				*slp++ = value;
			}
		}

		row++;
	
		if (header.numColourPlanes != 1)
		{//now need to iterate through image data and copy across to appropriate rgb channels
			u8* red = scanLineData;
			u8* green = scanLineData + header.BytesPerScanline;
			u8* blue = scanLineData + (header.BytesPerScanline * 2);
			u8* alpha = header.numColourPlanes == 4 ? scanLineData + (header.BytesPerScanline * 3) : nullptr;

			for (u32 processedBytes = bytesProcessed; processedBytes < (bytesProcessed + actualBytesPerImageRow);)
			{
				if (header.bitsperpixel == 8)
				{
					imageData[processedBytes + 0] = *red++;
					imageData[processedBytes + 1] = *green++;
					imageData[processedBytes + 2] = *blue++;
					if (alpha != nullptr)
					{
						imageData[processedBytes + 3] = *alpha++;
					}
					processedBytes += header.numColourPlanes;
				}
				else
				{
					//nothing as format not supported yet
				}
			}
		}
		else
		{
			memcpy(&imageData[bytesProcessed], scanLineData, actualBytesPerImageRow);
		}
		bytesProcessed += actualBytesPerImageRow;
	}
	return imageData;
}
void* PCXLoader::ConvertTo32bpp(void* a_imageData, void* a_palette, u32& a_w, u32& a_h, u8& a_bpp)
{
	u8* rawImage = new u8[a_w * a_h * 4];//allocate enough memory for raw image data inRGBA format
	u32 currentDataSize = a_w * (u32)(a_h * ((float)a_bpp / 8.f));
	u8* currentImage = (u8*)a_imageData;
	if (a_palette != nullptr)//convert a palettised image
	{
		PCXHeader::PCXPalleteColour* palette = (PCXHeader::PCXPalleteColour*)a_palette;
		//for each pixel in current data set
		for (u32 pixel = 0, i = 0; pixel < currentDataSize; ++pixel, i += 4)
		{
			u32 pi = currentImage[pixel];
			if (a_bpp == 8)
			{
				rawImage[i + 0] = palette[pi].B;
				rawImage[i + 1] = palette[pi].G;
				rawImage[i + 2] = palette[pi].R;
				rawImage[i + 3] = 0;
			}
			else if (a_bpp == 4)
			{
				rawImage[i + 0] = palette[(pi >> 4) & 0xF].B;
				rawImage[i + 1] = palette[(pi >> 4) & 0xF].G;
				rawImage[i + 2] = palette[(pi >> 4) & 0xF].R;
				rawImage[i + 3] = 0;
				i += 4;
				rawImage[i + 0] = palette[pi & 0xF].B;
				rawImage[i + 1] = palette[pi & 0xF].G;
				rawImage[i + 2] = palette[pi & 0xF].R;
				rawImage[i + 3] = 0;
			}
			else
			{
				//nothing as format not supported yet
			}
		}
	}
	else//convert an RGB image to RGBA
	{
		for (u32 pixel = 0, i = 0; pixel < currentDataSize; pixel+=3, i += 4)
		{
			rawImage[i + 0] = currentImage[pixel + 2];
			rawImage[i + 1] = currentImage[pixel + 1];
			rawImage[i + 2] = currentImage[pixel + 0];
			rawImage[i + 3] = 0;
		}
	}
	delete[] a_imageData;
	a_imageData = nullptr;
	delete[] a_palette;
	a_palette = nullptr;
	return rawImage;
}
