//includes
#include <stdafx.h>
#include "ppmLoader.h"

void ppmLoader::loadImage()
{
	std::fstream file; //new fstream
	file.open("mario.ppm", std::ios_base::in); //open file
	if (file.is_open()) //if file is open
	{
		std::cout << "file loaded successfully" << std::endl; //inform load was successful
		ppm image;
		//read in first two bytes of file as txt
		char buffer[512];
		memset(buffer, 0, 512);
		file.getline(buffer, 512);
		memcpy(&image.id, buffer, 2);
		if (image.id != 13904) //if id is incorrect
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

		file.read((char*)image.pixelData, pixelSize); //read pixel data from file
		file.close(); //close file
		for (int i = 0; i < (float)image.width * (float)image.height; i++)//loop for all pixels in image
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
		//open file
		file.open("noBlue.ppm", std::ios_base::out | std::ios_base::binary);
		file << "P6" << std::endl; //set id
		file << image.comment << std::endl; //set comment
		file << image.width << " " << image.height << std::endl; //set width
		file << image.maxColours << std::endl; //set max colours
		file.write((const char*)image.pixelData, pixelSize); //write the image with image data

		file.close();//close file

	}

}
