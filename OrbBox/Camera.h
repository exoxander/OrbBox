#pragma once
#include "Utility.h"
class Camera {
public:
	coord location;
	double zoom;
	double panSpeed;
	double zoomSpeed;

public:Camera() {
	location = coord(0, 0);
	zoom = 1;
	panSpeed = 4;
	zoomSpeed = 0.1;
}
public:Camera(double _x, double _y, double _zoom = 1) {
	location = coord(_x, _y);
	zoom = _zoom;
	panSpeed = 4;
	zoomSpeed = 0.1;
}
public:coord translate(coord parent, coord vertex) {//returns the coordinant of a vertex after converting between object and camera space
	coord result = coord(((vertex.x + parent.x) * zoom + location.x), ((vertex.y + parent.y) * zoom + location.y));
	return result;
}
};