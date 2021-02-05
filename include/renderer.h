#pragma once
#ifndef  _RENDERER_
#define _RENDERER_
//includes
#include <stdafx.h>

class renderer
{
public:
	renderer();//constructor
	~renderer();//deconstructor 

	static renderer* GetInstance() { return m_instance; } //instance getter function
	//functions
	int Intitalise(HWND a_consoleWindow, unsigned int a_width, unsigned int a_height);
	void ClearRenderBuffer();
	void Draw();
	void GetWindowSize(u32& a_w, u32& a_h);
	//send items to buffer 
	void FillRenderBuffer2(const unsigned int& a_X, const unsigned int& a_Y, const unsigned int& a_width, const unsigned int& a_height, const void* a_data);
	void FillRenderBuffer(const unsigned int& a_x, const unsigned int& a_y, const unsigned int& a_width, const unsigned int& a_height, const u32 a_colour);
	//getters
	HWND getWindowHanlde() const { return m_windowHanlde; }
	HDC getbufferContext() const { return m_buffDevContext; }
	

private:
	//class pointer
	static renderer*	 m_instance;
	//window and buffer variables and handles
	unsigned int		 m_windowWidth;
	unsigned int		 m_windowHeight;
	HWND				 m_windowHanlde;
	HDC					 m_windowDC;
	void*				 m_bitBuffer;
	BITMAPINFO*			 m_bmpInfo;
	HBITMAP				 m_bufferBmp;
	HDC					 m_buffDevContext;
	HBITMAP				 m_defBmp;

};


#endif // ! _RENDERER_
