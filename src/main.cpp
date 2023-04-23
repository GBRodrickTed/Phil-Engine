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
	//duration<double> invFps = duration_cast<system_clock::duration>(duration<double>{ 1. / fps });

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

	SDL_Window* window = NULL;
	window = SDL_CreateWindow(
		"Phil Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCR_W,
		SCR_H,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	bool running = true;

	SDL_Event event;

	//Phil::VertexArray1 vertArray;
	//vertArray.Bind();

	Phil::Shader shader;
	shader.CreateShaderFromFile("res/shaders/basic_V.shader", "res/shaders/basic_F.shader");

	glViewport(0, 0, SCR_W, SCR_H);

	glm::mat4 projection = glm::ortho(0.0f, SCR_W * 1.0f, SCR_H * 1.0f, 0.0f, -1.0f, 1.0f);

	GLCall(Phil::Renderer renderer);
	GLCall(renderer.SetProjection(projection));
	GLCall(renderer.SetShader(shader));
	GLCall(renderer.SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)));
	GLCall(renderer.SetDrawColor(glm::vec4(1.0f)));

	Phil::Rect rect1 = { {0, 0}, {100, 100} };

	Phil::Rect rect2 = { {150, 150}, {100, 100} };

	bool wireMode = false;

	//shader.use(); // important to do before setting uniforms
	//shader.set_iv("u_Textures", 32, samplers);
	//shader.set_vec4("u_Color", glm::vec4(1, 1, 1, 1));

	//unsigned int texture[2] = {
		//Phil::LoadTexture("res/gfx/pixel_david.png"), Phil::LoadTexture("res/gfx/pixel_phil.png")
	//};

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
		//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		//vertArray.Clear();
		//glClear(GL_COLOR_BUFFER_BIT);
		renderer.Clear();

		
		rect1.pos = glm::vec2((0.5f + sin(gameTime) * 0.5f) * (SCR_W*1.0f-rect1.size.x), (0.5f + cos(gameTime) * 0.5f) * (SCR_H * 1.0f - rect1.size.y));
		//rect1.size = glm::vec2((0.5f + sin(gameTime*0.9177f) * 0.5f) * SCR_W * 1.0f, (0.5f + cos(gameTime*1.17f) * 0.5f) * SCR_H * 1.0f);

		renderer.SetDrawColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		renderer.AddRect(rect1, sin(gameTime*13) * 45);

		renderer.SetDrawColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		renderer.AddRect(rect2);

		//vertArray.AddRect(texture[0], rect2);

		renderer.DrawBatch();

		gameTime += frameTime;

		SDL_GL_SwapWindow(window);
		FPS(60);
	}

	glfwTerminate();
	SDL_Quit();
	return 0;
}