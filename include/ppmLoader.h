#pragma once
#ifndef  PPM_IMAGE
#define PPM_IMAGE

class ppmLoader {

	ppmLoader();
	~ppmLoader();

private:

	void loadImage();

	template <typename T>
	struct ppmPixel
	{
		T r;
		T g;
		T b;
	};

	struct ppm
	{
		unsigned short id;
		char comment[512];
		unsigned int width;
		unsigned int height;
		unsigned int maxColours;

		void* pixelData;

	};
	   	
};

#endif // ! PPM_IMAGE


