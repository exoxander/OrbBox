#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
using olc::Pixel;

class SubWindow {
public:
	vector2d position;
	vector2d dimensions;
	int border;
	Pixel primary;
	Pixel secondary;
	Pixel highlight;

public:SubWindow() {
	position = vector2d();
	dimensions = vector2d(150,100);
	border = 5;
	primary = Pixel(255,150,0);
	secondary = Pixel(50, 240, 255);
	highlight = Pixel(240, 240, 0);
}
public:SubWindow(vector2d _position, vector2d _dimensions) {
	position = _position;
	dimensions = _dimensions;
	border = 5;
	primary = Pixel(255, 150, 0);
	secondary = Pixel(50, 240, 255);
	highlight = Pixel(240, 240, 0);
}
};