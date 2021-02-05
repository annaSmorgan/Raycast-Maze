#pragma once
#ifndef  _INPUT_MANAGER_
#define _INPUT_MANAGER_
//includes
#include <stdafx.h>

class inputManager
{
public:
	//Singleton accessor, retrives instance of class
	static inputManager* GetInstance() { return m_instance; }
	//Singleton creation, called to created an insatnce of class and return it
	static inputManager* CreateInstance();
	//destroy instance is used to remove instance of class
	static void DestroyInstance();
	//handles window call back
	static LRESULT CALLBACK HandleWindowCallbacks(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	//set value of key in key state array
	void SetKey(u8 a_key, bool a_val);
	//get current state of key
	bool GetKeyDown(u8 a_key);

private:
	//static instance of class
	static inputManager* m_instance;
	//constructor and destructor are private so cannot be called outside of instance
	inputManager();
	~inputManager();

	//array of bools to keep track of keys that are being pressed or not
	bool m_keyCurrentState[256];

};

#endif // ! _INPUT_MANAGER_