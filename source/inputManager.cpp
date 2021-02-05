//includes
#include <stdafx.h>
#include "inputManager.h"
#include "renderer.h"

inputManager::inputManager() //constructor setting memory for key current state variable
{
	memset(m_keyCurrentState, 0, 256);
}
//deconstructor
inputManager::~inputManager(){}

inputManager* inputManager::m_instance = nullptr; //setting the class pointer to null pointer

inputManager* inputManager::CreateInstance()
{
	if (m_instance == nullptr) // if instance is null pointer
	{
		m_instance = new inputManager(); //create new class instance
	}
	return m_instance; //return the instance
}

void inputManager::DestroyInstance()
{
	if (m_instance != nullptr) // if instance is not null pointer
	{
		delete m_instance; //delete the instance
		m_instance = nullptr; //set instance to null pointer
	}
}

LRESULT CALLBACK inputManager::HandleWindowCallbacks(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	PAINTSTRUCT paintStruct; //new struct containing application info
	HDC hdc; //new handle
	renderer* rendererI = renderer::GetInstance(); // instance of renderer

	if (hwnd == rendererI->getWindowHanlde()) //if window handle is the same as the renderer window handle
	{

		switch (message)
		{

		case WM_PAINT://called when window content invalidated
		{
			hdc = BeginPaint(hwnd, &paintStruct); //prep application and fill in struct

			RECT clRect; //new rectangle
			GetClientRect(hwnd, &clRect); //get coord of client area
			BitBlt(hdc, clRect.left, clRect.top, (clRect.right - clRect.left) + 1, //transfer colour data rectangle from renderer
				(clRect.bottom - clRect.top) + 1, rendererI->getbufferContext(), 0, 0, SRCCOPY);
			EndPaint(hwnd, &paintStruct); //end of painting to application
			break;
		}
		case WM_DESTROY: //called when window needs to close
		{
			PostQuitMessage(0); //makes a request to terminate
			break;
		}
		case WM_KEYDOWN: // when a key is pressed
		{
			inputManager* im = inputManager::GetInstance(); //creates a class instance
			im->SetKey((u8)wParam, true); //calls the set key function with true
			break;
		}
		case WM_KEYUP: //when a key is released
		{
			inputManager* im = inputManager::GetInstance();//creates a class instance
			im->SetKey((u8)wParam, false); //calls the set key function with false
			break;
		}
		default:
			return DefWindowProc(hwnd, message, wParam, lParam); //default procedure

		};

	}
	return DefWindowProc(hwnd, message, wParam, lParam); //default procedure

}

void inputManager::SetKey(u8 a_key, bool a_val)
{
	m_keyCurrentState[a_key] = a_val; //sets the key within the array to either true of false
}

bool inputManager::GetKeyDown(u8 a_key)
{
	return m_keyCurrentState[a_key]; //returns what key is called, whether it is true or false
}