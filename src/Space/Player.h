#pragma once
#include <Phil/Phil.h>

using namespace glm;

class Player {
public:
	Player();
	vec2 pos;
	vec2 size;

	vec2 dir;

	vec2 vel;
	vec2 acc;
	float force;
	float mass;

private:
};