#pragma once
#include "Utility.h"
class Camera {
public:
	vector2d location;
	vector2d screen;
	double zoom;
	double panSpeed;
	double zoomSpeed;
	

public:Camera() {
	location = vector2d(0, 0);
	screen = vector2d(256, 128);
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.1;
}
public:Camera(vector2d _position, vector2d _screen) {
	location = vector2d(_position.x - (screen.x / 2), _position.y - (screen.y / 2));
	screen = _screen;
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.02;
}
public:vector2d translate(vector2d parent, vector2d vertex) {//returns the coordinant of a vertex after converting between object and camera space
	vector2d result = vector2d(((vertex.x + parent.x) + location.x), ((vertex.y + parent.y) + location.y));
	return result;
}
public:vector2d reverseTranslate(vector2d input) {
	vector2d result = vector2d(input.x - location.x + (screen.x / 2),input.y - location.y + (screen.y / 2));
	return result;
}
};