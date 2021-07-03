#include "video.h"

Video::Video(int x, int y)
{
	initVideo();
	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_SHOWN);

	// Make sure creating the window succeeded
	if (!window) {
		std::cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
	}

	
}

int Video::initVideo()
{
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return 1;
	}


}

void Video::drawVideo()
{
	// Create our window
	/*window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);*/
	

	// Get the surface from the window
	winSurface = SDL_GetWindowSurface(window);

	// Make sure getting the surface succeeded
	if (!winSurface) {
		std::cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		//return 1;
	}

	// Fill the window with a white rectangle
	SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));

	// Update the window display
	SDL_UpdateWindowSurface(window);

	// Wait
	//system("pause");
}

Video::~Video()
{
	// Destroy the window. This will also destroy the surface
	SDL_DestroyWindow(window);

	// Quit SDL
	SDL_Quit();
}
