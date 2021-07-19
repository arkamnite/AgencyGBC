#include "Display.h"


Display::Display(float w, float h)
{
	init(w, h);
}


Display::~Display()
{
	// Stop the rendering loop.
	done = false;
	// Deallocate surfaces
	SDL_FreeSurface(surface);
	SDL_FreeSurface(background);
	SDL_FreeSurface(window);
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

	background = SDL_CreateRGBSurface(0, 160, 144, 32, 0, 0, 0, 0);
	window = SDL_CreateRGBSurface(0, 160, 50, 32, 0, 0, 0, 0);
	return success;
}

void Display::Update()
{

	col++;

	if (col > 255)
		col = 0;

	SDL_FillRect(background, NULL, SDL_MapRGB(background->format, col, 0, 0));
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 0, 255 - col, 255 - col));
	// Apply all surfaces
	//SDL_BlitSurface(background, NULL, SDL_GetWindowSurface(display), NULL);
	SDL_BlitSurface(window, NULL, background, NULL);
	SDL_BlitScaled(background, NULL, surface, NULL);
	SDL_UpdateWindowSurface(display);
}