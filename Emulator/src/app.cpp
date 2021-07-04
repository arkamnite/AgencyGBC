#include <stdio.h>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include "CPU.h"
#include "Video.h"

// SDL Window flags
SDL_WindowFlags window_flags;

// SDL window pointer
SDL_Window* window;

// SDL OpenGL context
SDL_GLContext gl_context;

// ImGUI clear colour
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// ImGUI Window Flags
ImGuiWindowFlags imgui_window_flags = 0;

// Main CPU
CPU cpu;


void printReg(CPU* cpu)
{
	std::vector<std::string> v = cpu->getRegisterValues();
	for (const auto& i : v)
		std::cout << i << '\n';
}

// Used to initialise SDL
int init(float x, float y)
{
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}
	
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("AgencyGBE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x, y, window_flags);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	
	bool err = gladLoadGL() == 0;

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return 0;
}

bool show_demo_window = false;
bool show_another_window = false;

// Show the register values in a dialogue
bool show_registers = false;

void showRegisters()
{
	// Retrieve vector of string values from cpu.
	std::vector<std::string> v = cpu.getRegisterValues();

	if (show_registers)
	{
		ImGui::Begin("Register values");

		// Place each string in a box
		for (const auto& i : v)
		{
			ImGui::Text(i.c_str());
		}

		ImGui::End();
	}
}

// Show the controls for the CPU
bool show_controls = false;

char InputBuf[256];

static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	return 0;
}

void showControls()
{
	uint16_t opcode;
	char opString[128] = "";

	if (show_controls) {
		ImGui::Begin("CPU Controls");

		// Reset
		
		if (ImGui::Button("Reset CPU"))
			cpu.reset();

		ImGui::Separator();

		if (ImGui::Button("Reset program counter"))
			cpu.resetPC();

		ImGui::Separator();
		
		// TODO: Add the opcode loading

		ImGui::NewLine();

		ImGui::Separator();

		ImGui::End();
	}
}

// Used to draw to the screen with SDL and ImGUI
void draw(bool &done)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			done = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			done = true;
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	
	/// Create menu bar at top of screen
	if (ImGui::BeginMainMenuBar())
	{
		// File: used to open a new ROM, open a recent ROM.
		if (ImGui::BeginMenu("File"))
		{
			// Load ROM.
			if (ImGui::MenuItem("Load ROM"))
				std::cout << "Loading rom" << std::endl;
			
			ImGui::EndMenu();
		}

		// Developer options
		if (ImGui::BeginMenu("Developer Options"))
		{
			// show registers + memory query
			if (ImGui::MenuItem("Registers", "Alt + S", &show_registers))
			{
				//show_registers = !show_registers;
				std::cout << show_registers << std::endl;
			}
			// show controls
			if (ImGui::MenuItem("CPU Controls", "Alt + C", &show_controls))
			{
				
			}

			ImGui::EndMenu();
		}
		showRegisters();
		showControls();

		ImGui::EndMainMenuBar();
	}
	
	
	


	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (ImGui::Button("Exit"))
		{
			done = !done;
			std::cout << done << std::endl;
		}

		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}



int main(int argc, char** args)
{
	std::cout << "hello world! This is the GB emulator." << std::endl;
	int x = 0;
	
	/*uint8_t val1 = 0x00FA;
	uint16_t val2 = 0x1000;
	uint16_t val3 = val1 + val2;
	uint8_t castTest = (uint8_t)0xAFFE;
	std::cout << std::hex << val3 << std::endl;
	std::cout << std::hex << castTest << std::endl;*/

	cpu = CPU();
	cpu.reset();
	
	cpu.loadOpcode(0x0006);
	cpu.loadOpcode(0x00FA);
	cpu.loadOpcode(0x0004);
	cpu.loadOpcode(0x0003);
	cpu.resetPC();

	/*cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);*/

	cpu.cycleLog();
	cpu.cycleLog();
	cpu.cycleLog();
	cpu.cycleLog();

	std::cout << std::hex << std::to_string(cpu.getRegisters()[1]) << std::endl;

	init(1270, 800);
	bool b = true;
	while (b)
		draw(b);

    return 0;

}
