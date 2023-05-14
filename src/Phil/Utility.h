#pragma once

#include <GLAD/glad.h>
#include <SDL2/SDL.h>

#include <chrono>
#include <thread>

#include <random>

int rand_i(int min, int max) {
	std::random_device rd;
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rd);
}

float rand_f(float min, float max) {
	std::random_device rd;
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rd);
}


