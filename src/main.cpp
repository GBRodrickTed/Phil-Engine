#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <thread>

#include "Phil/Phil.h"
#include "Phil/Utility.h"

#include "Space/Game.h"
#include "Space/GameState.h"



static std::chrono::system_clock::time_point _programStart = std::chrono::system_clock::now();

static float _frameTime = 0;

std::chrono::duration<double>getTime() {
	return std::chrono::system_clock::now() - _programStart;
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

	_frameTime = static_cast<float>(getTime().count() - currTime.count());
	currTime = getTime();
}

using std::string;
using std::cout;
using std::endl;

float gameTime = 0;

int main(int argc, char** argv) {
	
	Game game;

	game.Loop();

#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_MAINLOOP_END;
#endif

	return 0;
}