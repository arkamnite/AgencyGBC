#include <iostream>
#include "imgui.h"
#include <SDL.h>


class Video
{
protected:

	// SDL Surface pointer
	SDL_Surface* winSurface;
	// SDL Window pointer
	SDL_Window* window;

protected:
	
	// Initialise SDL.
	int initVideo();
		
public:

	Video(int x, int y);

	~Video();

	void drawVideo();
};