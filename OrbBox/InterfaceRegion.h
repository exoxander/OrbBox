#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
using std::cout;
using std::endl;
using olc::Pixel;

class InterfaceRegion {
public:
	//shared_ptr<bodyList> allBodies;
	//shared_ptr<Utility> util;
	//shared_ptr<Camera> viewport;
	vector2d position;
	vector2d dimensions;
	int border;
	Pixel primary;
	Pixel secondary;
	Pixel highlight;

public:InterfaceRegion() {
	//allBodies = nullptr;
	//util = nullptr;
	//viewport = nullptr;
	position = vector2d();
	dimensions = vector2d(150,100);
	border = 5;
	primary = Pixel(255,150,0);
	secondary = Pixel(50, 240, 255);
	highlight = Pixel(240, 240, 0);
}
public:InterfaceRegion(vector2d _position, vector2d _dimensions) {
	//allBodies = _bodyList;
	//util = _u;
	//viewport = _viewport;
	position = _position;
	position.multiply(_position);
	dimensions = _dimensions;
	dimensions.multiply(_position);
	border = 5;
	primary = Pixel(255, 150, 0);
	secondary = Pixel(10, 48, 52);
	highlight = Pixel(240, 240, 0);
}
public:void takeAction(int _action, shared_ptr<bodyList> _bodyList, shared_ptr<Utility> _u, shared_ptr<Camera> _viewport) {
	switch (_action) {
	case 0://toggle pause / play
		//cout << "attempt to toggle pause";
		//s
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
		//create virtual body at camera center
		//grab mass from slider / numerical input
		//use mouse to position
		//make new interfaceRegion and use widget to make a velocity vector with prediction path
		//use that data in _bodyList->createBody();
		vector2d temp = _viewport->reverseTranslate(vector2d());
		_bodyList->createBody(temp, vector2d((double(rand() % 10) / 10) - 1, (double(rand() % 10) / 10) - 1), 300);
		break;
	}

}
};