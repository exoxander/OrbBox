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
public:
	void zoomIn() {
	double temp = zoom;
	zoom += temp * zoomSpeed;
}
	void zoomOut() {
	double temp = zoom;
	zoom -= temp * zoomSpeed;
}
	void track(shared_ptr<body> _target) {
	target = _target;
}
	void freeCamera() {
	target = nullptr;
}
	void home() {
	location = vector2d();
}
	vector2i translate(vector2d parent, vector2d vertex = vector2d(), double scale = 1) {//returns the coordinant of a vertex after converting between object and camera space
	vector2d result = parent;
	result.add(vertex);
	result.add(location);
	result.multiply(zoom * scale);
	result.add(screen);
	return result.convertTo2i();
}
	vector2i translate(vector2i parent, vector2d vertex, double scale = 1) {
		vector2d result;
		result.x = double(parent.x);
		result.y = double(parent.y);

		result.add(vertex);
		result.add(location);
		result.multiply(zoom * scale);
		result.add(screen);
		return result.convertTo2i();
	}
	  //can only return the world space, not object space coordinant
	vector2d reverseTranslate(vector2d input) {//translates a position on the viewport to a position in game space	
	vector2d result = input;
	result.subtract(screen);
	result.multiply(1 / zoom);
	result.subtract(location);
	return result;
}
};