//includes
#include "stdafx.h"
#include "console.h"
//defines
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 40
#define CONSOLE_XPOS 20
#define CONSOLE_YPOS 180
//constructor initialising variables
Console::Console() : m_XPos(0), m_YPos(0), m_consoleWidth(0), m_consoleHeight(0),
					m_consoleHandle(nullptr), m_consoleWindow(nullptr)
{

}
//deconstructor
Console::~Console()
{

}

bool Console::Initialise(const char* a_windowTitle, const unsigned short a_XPos,
	const unsigned short a_YPos , const unsigned short a_Width ,
	const unsigned short a_Height )
{
	//sets up dimensions, using a tenary operator to dtermine whether to use default variables
	m_XPos = a_XPos ? a_XPos : CONSOLE_XPOS;
	m_YPos = a_YPos ? a_YPos : CONSOLE_YPOS;
	m_consoleWidth = a_Width ? a_Width : CONSOLE_WIDTH;
	m_consoleHeight = a_Height ? a_Height : CONSOLE_HEIGHT;
	//setting up the console screen buffer
	m_consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); 
	SetConsoleActiveScreenBuffer(m_consoleHandle);
	COORD consoleSize = { (short)m_consoleWidth, (short)m_consoleHeight };
	SetConsoleScreenBufferSize(m_consoleHandle, consoleSize);
	//set the windows title 
	SetConsoleTitle(a_windowTitle);
	//setting up the console window 
	m_consoleWindow = GetConsoleWindow();
	SetWindowPos(m_consoleWindow, 0, m_XPos, m_YPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	return true;
}
unsigned int Console::WriteToLocation(wchar_t* a_data, short a_numCharacters, short a_XPos, short a_YPos)
{
	DWORD dwBytesWritten = 0;
	//get the amount of bytes to write by multiplying the amount of characters by the size of wchar_t
	WriteConsoleOutputCharacter(m_consoleHandle, (LPCSTR)a_data, a_numCharacters * sizeof(wchar_t), { a_XPos, a_YPos }, &dwBytesWritten);

	return dwBytesWritten;

}