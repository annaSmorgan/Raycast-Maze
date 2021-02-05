//includes
#include "stdafx.h"
#include "renderer.h"
#include "inputManager.h"
//set class pointer to null
renderer* renderer::m_instance = nullptr;
//constructor initalising variables and setting instance to class ppinter
renderer::renderer() : m_windowWidth(0), m_windowHeight(0), m_windowHanlde(nullptr), m_windowDC(nullptr),
	m_bmpInfo(nullptr), m_bitBuffer(nullptr), m_bufferBmp(nullptr), m_buffDevContext(nullptr), m_defBmp(nullptr)
{
	m_instance = this;
}
//deconstructor sets instance to null
renderer :: ~renderer()
{
	m_instance = nullptr;
}
//initalising window 
int renderer::Intitalise(HWND a_consoleWindnow, unsigned int a_width, unsigned int a_height) 
{
	m_windowWidth = a_width;
	m_windowHeight = a_height;
	//register a window class with console application
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = inputManager::GetInstance()->HandleWindowCallbacks;
	wndClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = "RayCaster FrameWork";

	if (!RegisterClassEx(&wndClass))
	{
		return 1;
	}

	//set up window for application
	LONG X = 0; LONG Y = 0;
	RECT  consoleRec = { NULL };

	if (GetWindowRect(a_consoleWindnow, &consoleRec))
	{

		X = consoleRec.right;
		Y = consoleRec.top;

	}

	RECT windowRect = { X, Y, X + (LONG)m_windowWidth, Y + (LONG)m_windowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	//create window based on description
	m_windowHanlde = CreateWindowA("RayCaster Framework", "Main Scene",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, METRICS_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, (HINSTANCE)GetModuleHandle(NULL), nullptr);
	ShowWindow(m_windowHanlde, SW_SHOW);
	MoveWindow(m_windowHanlde, X, Y, m_windowWidth, m_windowHeight, true);

	//back buffer render target
	m_windowDC = GetDC(m_windowHanlde);
	if (m_windowDC == nullptr)
	{
		return 1;
	}

	//create a bite size array for bitmap info header
	char* data = (char*)malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
	if (data != nullptr)
	{
		m_bmpInfo = (BITMAPINFO*)data;
		m_bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_bmpInfo->bmiHeader.biWidth = (LONG)m_windowWidth;
		m_bmpInfo->bmiHeader.biHeight = -(LONG)m_windowHeight;
		m_bmpInfo->bmiHeader.biBitCount = 32;
		m_bmpInfo->bmiHeader.biPlanes = 1;
		m_bmpInfo->bmiHeader.biCompression = BI_RGB;
		m_bmpInfo->bmiHeader.biSizeImage = 0;
		m_bmpInfo->bmiHeader.biXPelsPerMeter = 0;
		m_bmpInfo->bmiHeader.biYPelsPerMeter = 0;
		m_bmpInfo->bmiHeader.biClrUsed = 0;
		m_bmpInfo->bmiHeader.biClrImportant = 0;

		m_bufferBmp = CreateDIBSection(m_windowDC, m_bmpInfo, DIB_RGB_COLORS, &m_bitBuffer, NULL, 0);
		if (m_bufferBmp == nullptr)
		{
			free(data);
			return 1;
		}

		//buffer device context
		m_buffDevContext = CreateCompatibleDC(m_windowDC);
		if (m_buffDevContext == nullptr)
		{
			free(data);
			return 1;
		}

		m_defBmp = (HBITMAP)SelectObject(m_buffDevContext, m_bufferBmp);
		if (m_defBmp == nullptr)
		{
			free(data);
			return 1;
		}
		free(data);
		return 0;
	}
	return 1;
}

void renderer::ClearRenderBuffer()
{
	RECT clRect;
	GetClientRect(m_windowHanlde, &clRect);

	//clear bitmap window background
	FillRect(m_buffDevContext, &clRect, (HBRUSH)(0x0000) + 2);
}
#define BYTES_PER_PIXEL 4
//send data to the buffer, image to be sent to buffer
void renderer::FillRenderBuffer2(const unsigned int& a_X, const unsigned int& a_Y, const unsigned int& a_width, const unsigned int& a_height, const void* a_data)
{
	unsigned int* backbuffer = (unsigned int*)m_bitBuffer;
	unsigned int* imagedata = (unsigned int*)a_data;
	unsigned int index = a_X + (a_Y * m_windowWidth);
	if (index < (m_windowWidth * m_windowHeight))
	{
		for (unsigned int y = 0; y < a_height; y++) 
		{
			if (a_Y + y < m_windowHeight)
			{
				unsigned int bytesToCopy = BYTES_PER_PIXEL * a_width;

				if (a_X + a_width > m_windowWidth)
				{
					bytesToCopy = (a_width - ((a_X + a_width) - m_windowWidth)) * BYTES_PER_PIXEL;
				}
				index = a_X + ((a_Y + y) * m_windowWidth);
				memcpy(&backbuffer[index], &imagedata[y * a_width], bytesToCopy);
			}
		}
	}
}
//send data to the buffer, colour to be sent to buffer
void renderer::FillRenderBuffer(const unsigned int& a_x, const unsigned int& a_y, const unsigned int& a_width, const unsigned int& a_height, const u32 a_colour)
{
	unsigned int index = a_x + (a_y * m_windowWidth);
	if (index < (m_windowWidth * m_windowHeight))
	{
		unsigned int* backBuffer = (unsigned int*)m_bitBuffer;

		unsigned int pixelsToFill = a_width;
		if ((a_x + a_width) > m_windowWidth)
		{
			pixelsToFill = (a_width - ((a_x + a_width) - m_windowWidth));
		}

		u32* rowBuffer = new u32[pixelsToFill];
		for (u32 i = 0; i < pixelsToFill; ++i)
		{
			rowBuffer[i] = a_colour;
		}

		for (unsigned int y = 0; y < a_height; ++y)
		{
			index = a_x + ((a_y + y) * m_windowWidth);

			if ((a_y + y) < m_windowHeight)
			{
				memcpy(&backBuffer[index], rowBuffer, pixelsToFill * BYTES_PER_PIXEL);
				continue;
			}
			break;
		}
		delete[] rowBuffer;
	}
}
//draws the window handle to the screen
void renderer::Draw()
{
	RedrawWindow(m_windowHanlde, nullptr, nullptr, RDW_INVALIDATE);
}
//gets the window applcaition dimensions
void renderer::GetWindowSize(u32& a_w, u32& a_h)
{
	a_w = m_windowWidth; a_h = m_windowHeight;
}

