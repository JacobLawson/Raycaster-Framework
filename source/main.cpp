#include <stdafx.h>

#include "rc_InputManager.h"
#include "rc_GameStateManager.h"
#include "rc_imageLoader.h"
#include "renderer.h"
#include "Console.h"
#include "rc_TextureManager.h"

#include "rc_SplashState.h"

int main(int argv, char* argc[])
{
	UNREFERENCED_PARAMETER(argv);
	UNREFERENCED_PARAMETER(argc);

	//Set up console for application
	Console* console = new Console;
	//Name console Window
	console->Initialise("Raycaster Console");

	//Initialise state manager and initial state
	TextureManager* texMan = new TextureManager();
	GameStateManager* gsManager = new GameStateManager();
	SplashState* splashState = new SplashState();
	splashState->SetStateName("SplashState");
	splashState->SetManager(gsManager);
	splashState->SetTextureManager(texMan);
	splashState->SetConsole(console);
	gsManager->PushState(splashState);

	//Create an object for handling inputs
	InputManager::CreateInstance();

	//Create a render context window - or bitmap render window
	u32 windowWidth = 640;
	u32 windowHeight = 480;

	Renderer mainWindow;
	
	//initialise main bit map window
	if (!mainWindow.Initialise(console->GetConsoleWindowHandle(), windowWidth, windowHeight))
	{
		MSG msg{ 0 };

		//Create a timer with delta time
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto previousTime = currentTime;
		std::chrono::duration<double> elapsedTime;

		unsigned int frame = 0;

		//enter main game loop
		while (msg.message != WM_QUIT)
		{
			//Check windows message
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//Update delta time
				previousTime = currentTime;
				currentTime = std::chrono::high_resolution_clock::now();
				elapsedTime = currentTime - previousTime;
				double fElapsedTime = elapsedTime.count();
				
				//Update current game state
				gsManager->Update((float)(fElapsedTime));

				//clear the render buffer
				mainWindow.ClearRenderBuffer();
				//Draw the game
				gsManager->Draw();

				if (frame % 30 == 0)
				{
					//Buffer to hold FPS value
					wchar_t fpsBuffer[8];
					//Calculate FPS from the elaspsed time
					unsigned int fps = (unsigned int)(1.0f / fElapsedTime);
					// swprintf_s -- adds a null terminator at n-1 instead of requiring it manually
					// Due to this buffer needs to be an additional wchar_t longer.
					// Using a ternary to limit fps display to 999 if over 1000 as not enough charaters in buffer
					swprintf_s(fpsBuffer, 8, L"FPS=%3u", fps > 999 ? 999 : fps);
					//Write fps to the console window
					console->WriteToLocation(fpsBuffer, 8, 0, 0);
				}
				++frame;
			}

		}
	}
//Delete the game management objects
delete texMan;
delete gsManager;
delete console;
InputManager::DestroyInstance();
return 0;
}