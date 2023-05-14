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

class GameStateManager;

class Game {
public:
	Game();
	~Game();

	void Loop();

	Phil::Window* window;
	Phil::Renderer* renderer;

	GameStateManager* stateManager;

	bool running;
private:
	SDL_GLContext m_gl_context;
	
	glm::ivec2 m_scrSize;
};