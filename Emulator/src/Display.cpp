#include "Display.h"


Display::Display(float w, float h)
{
	init(w, h);
}


Display::~Display()
{
	// Deallocate surface
	SDL_FreeSurface(surface);
	SDL_FreeSurface(background);
	SDL_DestroyWindow(display);
	SDL_Quit();
}

bool Display::init(float w, float h)
{
	bool success = true;

	// initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialise the display for the Game Boy- SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		// Create our window
		display = SDL_CreateWindow("Game Boy Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
		if (display == NULL)
		{
			std::cout << "Window for the Game Boy display could not be created- error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else 
		{
			// Get the window surface
			surface = SDL_GetWindowSurface(display);
		}
	}

	return success;
}

void Display::Update()
{

	col++;

	if (col > 255)
		col = 0;

	background = SDL_CreateRGBSurface(0, 800, 800, 32, 0, 0, 0, 0);
	SDL_FillRect(background, NULL, SDL_MapRGB(background->format, col, 0, 0));

	// Apply all surfaces
	SDL_BlitSurface(background, NULL, SDL_GetWindowSurface(display), NULL);
	SDL_UpdateWindowSurface(display);
}