#pragma once

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <SDL.h>
#include <glad/glad.h>
#include <glfw3.h>

class Video
{
public:
	Video();

	void draw();

	~Video();

private:

	const char* glsl_version;
	SDL_WindowFlags window_flags;
	SDL_Window* window;
	SDL_GLContext gl_context;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//ImGuiIO& io;


	// Main loop
	bool done = false;

	int initialise();

	void initIMGUI();



};
