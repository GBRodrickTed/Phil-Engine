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
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

	bool running = true;

	SDL_Event event;

	window.Resize(100, 100);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Phil::Renderer renderer(window.GetWindow());
	renderer.SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	renderer.SetDrawColor(glm::vec4(1.0f));

	Phil::Rect rect1 = { {0, 0}, {100, 100} };

	Phil::Rect rect2 = { {150, 150}, {100, 100} };

	Phil::Rect rect3 = { {250, 250}, {100, 100} };

	Phil::Rect rect4 = { {450, 450}, {100, 100} };

	bool wireMode = false;

	Phil::Texture texture1("res/gfx/pixel_phil.png");
	Phil::Texture texture2("res/gfx/noob_shot.png");

	while (running) {
		while (SDL_PollEvent(&event)) {
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
			}
		}
		renderer.Clear();
		
		renderer.AddRect(&texture1, rect3);

		renderer.SetDrawColor(glm::vec4(0.2f, 1.0f, 0.8f, 1.0f));
		renderer.AddLine(150, 150, 250, 250);

		renderer.Present();

		gameTime += frameTime;

		FPS(60);
	}

	glfwTerminate();
	SDL_Quit();
	return 0;
}