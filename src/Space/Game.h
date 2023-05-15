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
#include <chrono>

class GameStateManager;

class Time {
public:
	std::chrono::system_clock::time_point programStart = std::chrono::system_clock::now();
	float dt = 0;
	float gameTime = 0;
	std::chrono::duration<double>getTime();

	void FPS(unsigned char fps);
};

class Game {
public:
	Game();
	~Game();

	void Loop();

	Phil::Window* window;
	Phil::Renderer* renderer;

	Time time;

	GameStateManager* stateManager;

	bool running;
private:
	SDL_GLContext m_gl_context;
	
	glm::ivec2 m_scrSize;
};