#pragma once
#include "Utility.h"
class Camera {
public:
	dVector location;
	dVector screen;
	double zoom;
	double panSpeed;
	double zoomSpeed;
	shared_ptr<body> target;

public:Camera() {
	location = dVector(0, 0);
	screen = dVector(256, 128);
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.02;
	target = nullptr;
}
public:Camera(dVector _position, dVector _screen) {
	location = dVector(_position.x, _position.y);
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
	location = dVector();
}
	iVector translate(dVector parent, dVector vertex = dVector(), double scale = 1) {//returns the coordinant of a vertex after converting between object and camera space
	dVector result = parent;
	result.add(vertex);
	result.add(location);
	result.multiply(zoom * scale);
	result.add(screen);
	return result.toiVector();
}
	iVector translate(iVector parent, dVector vertex, double scale = 1) {
		iVector result = parent;
		dVector newVertex = vertex;
		newVertex.multiply(scale);
		result.add(newVertex.toiVector());		
		return result;
	}
	  //can only return the world space, not object space coordinant
	dVector reverseTranslate(dVector input) {//translates a position on the viewport to a position in game space	
	dVector result = input;
	result.subtract(screen);
	result.multiply(1 / zoom);
	result.subtract(location);
	return result;
}
};