#include <stdafx.h>
#pragma once
#ifndef __RC_INPUT_MANAGER__
#define __RC_INPUT_MANAGER__

//This is the main input handler class, as such it will be a static instance that is accessable from anywhere in the application
class InputManager
{
public:
	//Singleton accessor function call this to retrieve an instance of InputManager 
	static InputManager* GetInstance() { return m_instance; }
	//Create instance of our inpit manager and return it
	static InputManager* CreateInstance();
	//Destroy the Instance of the Input Manager
	static void DestroyInstance();
	//Lest have that function to handle window callback stuff in our Input Manager
	static LRESULT CALLBACK HandleWindowCallbacks(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	//set the value of a key in our key state array
	void SetKey(u8 a_key, bool a_val);
	//get the current state of a key in our key state array
	bool GetKeyDown(u8 a_key);

private:
	//this is the static instance of our inputmanager class
	static InputManager* m_instance;
	//Constructor and destructor are private this means that they they can only be accessed from inside an instance of the class
	//therefore we cannot call InputManager* im = new InputManager() unless we are inside a function belonging to this class
	InputManager();
	~InputManager();

	//an array of 256 bools to keep track of kets that are currently pressed (or not)
	bool m_keyCurrentState[256];
};

#endif
