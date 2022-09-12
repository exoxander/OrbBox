#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
using std::cout;
using std::endl;
using olc::Pixel;

class InterfaceRegion {
public:
	shared_ptr<bodyList> physicsBodies;
	shared_ptr<Utility> util;
	shared_ptr<Camera> viewport;
	vector2d position;
	vector2d dimensions;
	int border;
	Pixel primary;
	Pixel secondary;
	Pixel highlight;

public:InterfaceRegion() {
	physicsBodies = nullptr;
	util = nullptr;
	viewport = nullptr;
	position = vector2d();
	dimensions = vector2d(150,100);
	border = 5;
	primary = Pixel(255,150,0);
	secondary = Pixel(50, 240, 255);
	highlight = Pixel(240, 240, 0);
}
public:InterfaceRegion(vector2d _position, vector2d _dimensions, shared_ptr<bodyList> _bodyList, shared_ptr<Utility> _u, shared_ptr<Camera> _viewport) {
	physicsBodies = _bodyList;
	util = _u;
	viewport = _viewport;
	position = _position;//position is given as a fraction of the screen, not a pixel distance, this was dumb
	position.multiply(_position);
	dimensions = _dimensions;
	dimensions.multiply(_position);
	border = 5;
	primary = Pixel(255, 150, 0);
	secondary = Pixel(10, 48, 52);
	highlight = Pixel(240, 240, 0);
}

public:bool checkMouseInBounds(int _x, int _y) {
	bool temp = false;
	if (_x >= position.x && _x <= dimensions.x) {//seperating x and y for clarity
		if (_y >= position.y && _y <= dimensions.y) {
			temp = true;
		}
	}

	return temp;
}
public:void takeAction(int _action) {
	switch (_action) {
	case 0://toggle pause / play
		//cout << "attempt to toggle pause";
		//s
		util->game_paused = (util->game_paused ? false : true);
		break;
	case 1://force play
		util->game_paused = false;
		break;
	case 2://force pause
		util->game_paused = true;
		break;
	case 3://pause and create new (virtual?) body at camera position
		util->game_paused = true;
		//create virtual body at camera center
		//grab mass from slider / numerical input
		//use mouse to position
		//make new interfaceRegion and use widget to make a velocity vector with prediction path
		//use that data in _bodyList->createBody();
		vector2d temp = viewport->reverseTranslate(vector2d());
		physicsBodies->createBody(temp, vector2d((double(rand() % 10) / 10) - 1, (double(rand() % 10) / 10) - 1), 300);
		break;
	}

}
};