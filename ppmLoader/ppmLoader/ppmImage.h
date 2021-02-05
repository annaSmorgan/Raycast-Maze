#pragma once
#ifndef  PPM_IMAGE_H
#define PPM_IMAGE_H

template <typename T> //create a new template
struct ppmPixel //struct for pixel RGB
{
	T r; // red
	T g; // greem
	T b; //blue
};

struct ppm //struct for ppm image data
{

	unsigned short id;
	char comment[512];
	unsigned int width;
	unsigned int height;
	unsigned int maxColours;

	void* pixelData;


};

#endif // ! PPM_IMAGE_h


