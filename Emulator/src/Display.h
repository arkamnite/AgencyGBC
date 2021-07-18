#pragma once

#include <SDL.h>
#include <iostream>

class Display
{
private:
	// SDL window pointer to the GameBoy screen
	SDL_Window* display = NULL;

	// GB Display flags
	SDL_WindowFlags display_flags;

	// Main display surface
	SDL_Surface* surface = NULL;

	// GB background surface
	SDL_Surface* background = NULL;

	// Initialise the display with a set width and height.
	bool init(float w, float h);

	// The window will close when this variable is true.
	bool done = false;

	float col = 0x00ff0000;

public:
	// Constructor for the Game Boy Display
	Display(float w, float h);

	// Update the display
	void Update();

	~Display();
};