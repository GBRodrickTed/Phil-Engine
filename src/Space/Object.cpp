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

bool pointCircle(vec2 p, vec2 c, float r) {

	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	float distX = p.x - c.x;
	float distY = p.y - c.y;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's
	// radius the point is inside!
	if (distance <= r) {
		return true;
	}
	return false;
}