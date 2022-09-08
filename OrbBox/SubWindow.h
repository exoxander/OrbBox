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
public:SubWindow(vector2d _position, vector2d _dimensions, vector2d _screen) {
	position = _position;
	position.multiply(_screen);
	dimensions = _dimensions;
	dimensions.multiply(_screen);
	border = 5;
	primary = Pixel(255, 150, 0);
	secondary = Pixel(10, 48, 52);
	highlight = Pixel(240, 240, 0);
}
public:void takeAction(int _action, shared_ptr<bodyList> _bodyList, shared_ptr<Utility> _u, shared_ptr<Camera> _viewport) {
	switch (_action) {
	case 0://toggle pause / play
		_u->game_paused = (_u->game_paused ? false : true);
		break;
	case 1://force play
		_u->game_paused = false;
		break;
	case 2://force pause
		_u->game_paused = true;
		break;
	case 3://pause and create new (virtual?) body at camera position
		_u->game_paused = true;
		_bodyList->createBody(_viewport->location, vector2d(), 300);
		break;
	}

}
};