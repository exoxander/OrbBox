#pragma once
#include "Utility.h"
class Camera {
public:
	vector2d location;
	double zoom;
	double panSpeed;
	double zoomSpeed;

public:Camera() {
	location = vector2d(0, 0);
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.1;
}
public:Camera(double _x, double _y, double _zoom = 1) {
	location = vector2d(_x, _y);
	zoom = _zoom;
	panSpeed = 4;
	zoomSpeed = 0.05;
}
public:vector2d translate(vector2d parent, vector2d vertex) {//returns the coordinant of a vertex after converting between object and camera space
	vector2d result = vector2d(((vertex.x + parent.x) * zoom + location.x), ((vertex.y + parent.y) * zoom + location.y));
	return result;
}
};