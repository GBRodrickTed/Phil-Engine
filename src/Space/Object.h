#pragma once

#include "Phil/Phil.h"

using namespace glm;

class Object {
public:
	Object();
	Object(float r, vec2 pos, vec2 vel, vec2 acc, float mass);

	float r;
	vec2 pos;
	vec2 pos_prev;
	vec2 acc;
	float mass;

	vec2 getVelocity()
	{
		 return pos - pos_prev;
	}

	void setVelocity(vec2 v)
	{
		pos_prev = pos - v;
	}

	void addVelocity(vec2 v)
	{
		pos_prev -= v;
	}

	void addImpulse(vec2 I)
	{
		vec2 v = (I / mass) + getVelocity();
		pos_prev = pos - v;
	}

	void update(float dt) {
		vec2 displacement = pos - pos_prev;
		pos_prev = pos;
		pos += displacement + acc * (dt * dt);
		acc = vec2(0);
	}

private:
};