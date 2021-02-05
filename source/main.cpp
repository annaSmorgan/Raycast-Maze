//includes
#include <stdafx.h>
#include "console.h"
#include "renderer.h"
#include "rcImageLoader.h"
#include "ppmLoader.h"
#include "rcTextureManager.h"
#include "rcTexture.h"
#include "inputManager.h"
#include "player.h"
#include "GameStateManager.h"
#include "SplashState.h"
#include "GamePlayState.h"
#include "Level.h"

int main(int argc, char* argv[]) 
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	//instances
	Console console;
	renderer mainWindow;
	Player player;
	Level level;
	TextureManager::CreateInstance();
	GameStateManager::CreateInstance();
	SplashState* splashScreen = new SplashState();
	inputManager::CreateInstance();

	
	//initialise the console
	console.Initialise("raycaster console");

	//setting up splash states attributes
	splashScreen->SetManager(GameStateManager::GetInstance()); // setting the mananger to the game state manager
	splashScreen->SetStateName("SplashState"); //setting the name of the state
	GameStateManager::GetInstance()->PushState(splashScreen); //pushing the state to the manager

	//renderer context window / bitmap render window;
	u32 windowWidth = 640;
	u32 windowHeight = 640;

	if (!mainWindow.Intitalise(console.GetConsoleWindowHandle(), windowWidth, windowHeight))
	{
		MSG msg = { 0 };

		//timer to get delta time between frames
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto previousTime = currentTime;
		unsigned int frame = 0;

		//delta time variable
		std::chrono::duration<double> elapsedTime;

		while (msg.message != WM_QUIT) //while not quit
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))//checks for messages, to see if quit
			{
				TranslateMessage(&msg);//translates to characters
				DispatchMessage(&msg);//sent to window procedure

			}
			else
			{
				//previous time will chnage to be current time
				previousTime = currentTime;
				//current time is set to the time based on a high res clocks current time
				currentTime = std::chrono::high_resolution_clock::now();
				//elapsed time is the difference between current and previous
				elapsedTime = currentTime - previousTime;
				//count the difference to create the difference between frames
				double fElapsedTime = elapsedTime.count();
				//use an instance of game state manager to run its update function
				GameStateManager::GetInstance()->Update((float)(fElapsedTime));
				//clear renderer buffer
				mainWindow.ClearRenderBuffer();
				//calling the draw function within the game state manager
				GameStateManager::GetInstance()->Draw();
				//calling the renderer to draw
				mainWindow.Draw();
				//if the frames are a multiple of 30
				if (frame % 30 == 0)
				{
					wchar_t fpsBuffer[8];//to store FPS value
					unsigned int fps = (unsigned int)(1.0f / fElapsedTime);//frames per second
					//capping the FPS to a max of 999
					swprintf_s(fpsBuffer, 8, L"FPS=%3u", fps > 999 ? 999 : fps);
					//writing the fps buffer to the console
					console.WriteToLocation(fpsBuffer, 8, 0, 0);

				}
				++frame;
			}

		}
	}
	//deleting the instances of the managers as the game has been exited
	GameStateManager::DestroyInstance();
	TextureManager::DestroyInstance();
	inputManager::DestroyInstance();
	return 0;
	
}
