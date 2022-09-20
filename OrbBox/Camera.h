#pragma once
#include "Utility.h"
class Camera {
public:
	vector2d location;
	vector2d screen;
	double zoom;
	double panSpeed;
	double zoomSpeed;
	shared_ptr<body> target;

public:Camera() {
	location = vector2d(0, 0);
	screen = vector2d(256, 128);
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.02;
	target = nullptr;
}
public:Camera(vector2d _position, vector2d _screen) {
	location = vector2d(_position.x, _position.y);
	screen = _screen;
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.02;
	target = nullptr;
}
public:void zoomIn() {
	double temp = zoom;
	zoom += temp * zoomSpeed;
}
public: void zoomOut() {
	double temp = zoom;
	zoom -= temp * zoomSpeed;
}
public:void track(shared_ptr<body> _target) {
	target = _target;
}
public:void freeCamera() {
	target = nullptr;
}
public:vector2d translate(vector2d parent, vector2d vertex) {//returns the coordinant of a vertex after converting between object and camera space
	vector2d result = vector2d(((vertex.x + parent.x) * zoom + location.x * zoom) + screen.x, ((vertex.y + parent.y) * zoom + location.y * zoom) + screen.y);
	return result;
}
public:vector2d reverseTranslate(vector2d input) {//translates a position on the viewport to a position in game space	
	vector2d result = vector2d(location.x, location.y);
	result.multiply(-1);
	result.add(screen);
	return result;
}
};