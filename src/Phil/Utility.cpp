#include "Utility.h"

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