#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "Phil/Opengl Objects/Shader.h"
#include "Phil/Opengl Objects/GLDebug.h"
#include "Phil/Renderer.h"
#include "Phil/Window.h"

#include "imGui/imgui.h"

#include "imGui/backends/imgui_impl_sdl2.h"
#include "imGui/backends/imgui_impl_opengl3.h"

#define SCR_W 800
#define SCR_H 600

using std::string;
using std::cout;
using std::endl;

float frameTime = 0;
float accumulator = 0;
float dt = 1.0f / 120.0f;
float gameTime = 0;
float fixedTime = 0;
unsigned int ticks = 0;

std::chrono::system_clock::time_point programStart = std::chrono::system_clock::now();

std::chrono::duration<double>getTime() {
	return std::chrono::system_clock::now() - programStart;
}

void FPS(unsigned char fps) {
	using namespace std::chrono;

	static duration<double> currTime = duration<double>::zero();
	static duration<double> eventTime = duration<double>::zero();

	duration<double> invFps = duration<double>{ 1. / fps };

	eventTime = getTime() - currTime;
	// :)
	if (eventTime < invFps) {
		std::this_thread::sleep_for(invFps - eventTime);
	}

	frameTime = static_cast<float>(getTime().count() - currTime.count());
	currTime = getTime();
}

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	
	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
		SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetSwapInterval(1); // vsync


	//SDL_Window* window = NULL;
	Phil::Window window;
	window.CreateWindow(SDL_CreateWindow(
		"Phil Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCR_W,
		SCR_H,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	));

	SDL_GLContext gl_context = SDL_GL_CreateContext(window.GetWindow());
	SDL_GL_MakeCurrent(window.GetWindow(), gl_context);
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	glViewport(0, 0, window.GetW(), window.GetH());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window.GetWindow(), gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool running = true;

	SDL_Event event;

	window.Resize(100, 100);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Phil::Renderer renderer(&window);
	renderer.SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	renderer.SetDrawColor(glm::vec4(1.0f));

	Phil::Rect rect1 = { {-50, -50}, {100, 100} };

	Phil::Rect rect2 = { {150, 150}, {100, 100} };

	Phil::Rect rect3 = { {250, 250}, {100, 100} };

	Phil::Rect rect4 = { {450, 450}, {100, 100} };

	bool wireMode = false;

	Phil::Texture texture1("res/gfx/pixel_phil.png");
	Phil::Texture texture2("res/gfx/noob_shot.png");

	float factor = 1;

	while (running) {

		const unsigned char* keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
		{
			rect1.pos.y -= 10;
		}

		if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
		{
			rect1.pos.y += 10;
		}

		if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
		{
			rect1.pos.x -= 10;
		}

		if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
		{
			rect1.pos.x += 10;
		}

		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_0:
					wireMode = !wireMode;
					if (wireMode) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}
					else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					break;
				}
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y < 0) // scroll up
				{
					factor += 0.1;
				}
				else if (event.wheel.y > 0) // scroll down
				{
					factor -= 0.1;
					if (factor <= 0.1) {
						factor = 0.1;
					}
				}
				break;
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		glm::ivec2 mouse;
		glm::vec2 gMouse;
		glm::vec2 scrSize = glm::vec2(SCR_W, SCR_H);

		//float factor = 0.5f + ((0.5 * sin(gameTime * 2)) + 0.5) * 2;

		renderer.camera.SetSize(glm::vec2(SCR_W * static_cast<float>(pow(factor, 2)), SCR_H * static_cast<float>(pow(factor, 2))));
		renderer.camera.SetPos(rect1.pos + rect1.size / 2.0f - renderer.camera.GetSize()/2.0f);//glm::vec2(sin(gameTime * 2) * 50, cos(gameTime * 2) * 50) 
		//renderer.camera.SetPos((glm::vec2)mouse - scrSize/2.0f - renderer.camera.GetSize() / 2.0f);
		//renderer.camera.SetPos(glm::vec2(sin(gameTime * 2) * 50, cos(gameTime * 2) * 50));
		SDL_GetMouseState(&mouse.x, &mouse.y);
		gMouse = renderer.camera.TransMouse(mouse);
		rect2.pos = gMouse;

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!"); // Create a window called "Hello,
			// world!" and append into it.

			ImGui::Text(
				"This is some useful text."); // Display some text (you can use
			// a format strings too)

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		renderer.Clear();
		
		renderer.AddRect(&texture1, rect2);
		renderer.AddLine(gMouse.x, gMouse.y, 0, 0);
		renderer.AddRect(&texture2, rect1);
		renderer.AddRect(&texture2, rect4);
		
		renderer.SetDrawColor(glm::vec4(0.2f, 1.0f, 0.8f, 1.0f));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		renderer.Present();

		gameTime += frameTime;

		FPS(160);
	}

	glfwTerminate();
	SDL_Quit();
	return 0;
}