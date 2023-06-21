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


int main(int argc, char** argv) {
	Game game;

	game.Loop();

#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_MAINLOOP_END;
#endif

	return 0;
}