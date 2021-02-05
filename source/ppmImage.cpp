#include <stdafx.h>
#include "rcImageLoader.h"
#include "ppgLoader.h"


int main(int argv, char* args[])
{

	std::fstream file;
	file.open("mario.ppm", std::ios_base::in);
	if (file.is_open())
	{
		std::cout << "file loaded successfully" << std::endl;
		ppm image;
		//read in first two bytes of file as txt
		char buffer[512];
		memset(buffer, 0, 512);
		file.getline(buffer, 512);
		unsigned short id;
		memcpy(&image.id, buffer, 2);
		if (image.id != 13904)
		{
			std::cout << "this is not proper ppm file" << buffer << std::endl;
		}
		memset(buffer, 0, 512);
		file.getline(buffer, 512);
		if (buffer[0] == '#')
		{
			//this file has a comment
			memcpy(image.comment, buffer, 512);
			std::cout << image.comment << std::endl;
		}
		file >> image.width >> image.height;
		file >> image.maxColours;
		file.ignore(1);

		unsigned int pixelSize = 0;

		//create a ppm image object to store pixel data
		if (image.maxColours > 255)
		{
			image.pixelData = new ppmPixel<unsigned short>[image.width * image.height];
			pixelSize = image.width * image.height * sizeof(ppmPixel<unsigned short>);

		}
		else
		{
			image.pixelData = new ppmPixel<unsigned char>[image.width * image.height];
			pixelSize = image.width * image.height * sizeof(ppmPixel<unsigned char>);
		}

		file.read((char*)image.pixelData, pixelSize);
		file.close();
		for (int i = 0; i < image.width * image.height; i++)
		{
			if (image.maxColours > 255)
			{
				ppmPixel<unsigned short>* pixel = &((ppmPixel<unsigned short>*)image.pixelData)[i];
				pixel->b = 0;
			}
			else
			{
				ppmPixel<unsigned char>* pixel = &((ppmPixel<unsigned char>*)image.pixelData)[i];
				pixel->b = 0;
			}
		}

		file.open("noBlue.ppm", std::ios_base::out | std::ios_base::binary);
		file << "P6" << std::endl;
		file << image.comment << std::endl;
		file << image.width << " " << image.height << std::endl;
		file << image.maxColours << std::endl;
		file.write((const char*)image.pixelData, pixelSize);

		file.close();

	}



	return 0;
}