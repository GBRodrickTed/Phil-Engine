#pragma once

#include "Phil/Phil.h"

using namespace glm;

class Object {
public:
	Object();
	Object(float r, vec2 pos, vec2 vel, vec2 acc, float mass);

	float r;
	vec2 pos;
	vec2 vel;
	vec2 acc;
	float mass;

private:
};