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
	panSpeed = 0.01;
	zoomSpeed = 0.1;
}
public:Camera(double _x, double _y, double _zoom = 1) {
	location = coord(_x, _y);
	zoom = _zoom;
	panSpeed = 0.01;
	zoomSpeed = 0.1;
}
};