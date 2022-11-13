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
	iVector spaceTranslate(dVector base, dVector offset = dVector(), double scale = 1) {//returns the coordinant of a offset after converting between object and camera space
	dVector result = base;
	result.add(offset.returnMultiply(scale));
	result.add(location);
	result.multiply(zoom);
	result.add(screen);
	return result.toiVector();
}
	iVector spaceTranslate(iVector base, dVector offset, double scale = 1) {
		iVector result = base;
		dVector newoffset = offset;
		newoffset.multiply(scale);
		result.add(newoffset.toiVector());		
		return result;
	}

	  //can only return the world space, not object space coordinant
	dVector reversespaceTranslate(iVector input, double scale = 1) {//spaceTranslates a position on the viewport to a position in game space	
		dVector result = dVector();
		result.makedVector(input);
		result.subtract(screen);
		result.multiply(1 / (zoom * scale));
		result.subtract(location);
		return result;
}
};