#pragma once

#include <GLAD/glad.h>
#include <SDL2/SDL.h>

#include <chrono>
#include <thread>

static float _frameTime = 0;

static std::chrono::system_clock::time_point _programStart = std::chrono::system_clock::now();

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


