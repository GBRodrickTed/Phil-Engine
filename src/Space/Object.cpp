#include "Object.h"

Object::Object() {
	r = 1;
	pos = vec2(0);
	pos_prev = vec2(0);
	acc = vec2(0);
	mass = 1;
}

Object::Object(float r, vec2 pos, vec2 vel, vec2 acc, float mass) {
	this->r = r;
	this->pos = pos;
	this->pos_prev = vel;
	this->acc = acc;
	this->mass = mass;
}