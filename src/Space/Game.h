#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <thread>

#include "Phil/Phil.h"

#include <thread>
#include <random>

class Game {
public:
	Game();
	~Game();

	void Loop();

	bool running;
private:
	Phil::Window* m_window;
	SDL_GLContext m_gl_context;
	Phil::Renderer* m_renderer;

	glm::ivec2 m_scrSize;
};