#pragma once
#ifndef  _CONSOLE_H_
#define _CONSOLE_H_
//includes
#include "stdafx.h"

class Console
{
public:
	Console(); //constructor
	~Console(); //deconstructor

	bool Initialise(const char* a_windowTitle, const unsigned short a_XPos = 0, const unsigned short a_YPos = 0,
		const unsigned short a_Width = 0, const unsigned short a_Height = 0); //setting up the console

	unsigned int WriteToLocation(wchar_t* a_data, short a_numCharacters, short a_XPos, short a_YPos); //writing to the console

	HWND GetConsoleWindowHandle() { return m_consoleWindow; } //return the console window handle

private:
	//console variables
	unsigned short m_XPos;
	unsigned short m_YPos;
	unsigned short m_consoleWidth;
	unsigned short m_consoleHeight;
	//console handles
	HANDLE m_consoleHandle;
	HWND m_consoleWindow;

};
#endif // ! _CONSOLE_H_