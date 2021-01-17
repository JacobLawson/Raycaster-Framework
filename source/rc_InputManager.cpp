#include <stdafx.h>
#include "rc_InputManager.h"
#include "Renderer.h"

InputManager* InputManager::m_instance = nullptr;

//construct this new Input manager
InputManager* InputManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new InputManager();
	}
	return m_instance;
}

//free / destroy the input manager (called when game ends)
void InputManager::DestroyInstance()
{
	if (m_instance == nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

LRESULT InputManager::HandleWindowCallbacks(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	HDC hDC;
	PAINTSTRUCT paintStruct;

	//get pointer to the game renderer
	Renderer* renderer = Renderer::GetInstance();

	//handle
	if (hwnd == renderer->GetWindowHandle())
	{
		switch (message)
		{
		case WM_PAINT:	//called when window is invalidated
		{
			hDC = BeginPaint(hwnd, &paintStruct);

			//Draw /redraw the screen
			RECT clRect;
			GetClientRect(hwnd, &clRect);
			BitBlt(hDC, clRect.left, clRect.top, (clRect.right - clRect.left) + 1, (clRect.bottom - clRect.top) + 1, renderer->GetBufferContext(), 0, 0, SRCCOPY);
			EndPaint(hwnd, &paintStruct);
			break;
		}
		case WM_DESTROY:	//called when window is closed
		{
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN: //set the key pressed to true
		{		
			InputManager* im = InputManager::GetInstance();
			im->SetKey((u8)wParam, true);
			break;
		}
		case WM_KEYUP: 	//return the key pressed to false
		{
			InputManager* im = InputManager::GetInstance();
			im->SetKey((u8)wParam, false);
			break;
		}
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		};
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void InputManager::SetKey(u8 a_key, bool a_val)
{
	m_keyCurrentState[a_key] = a_val;
}

bool InputManager::GetKeyDown(u8 a_key)
{
	return m_keyCurrentState[a_key];
}

InputManager::InputManager()
{
	memset(m_keyCurrentState, 0, 256);
}

InputManager::~InputManager()
{
}
