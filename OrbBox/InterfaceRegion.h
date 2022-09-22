#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
#include "MarkupCore.h"

using std::cout;
using std::endl;
using olc::Pixel;

class InterfaceRegion {
public:
	shared_ptr<bodyList> physicsBodies;
	shared_ptr<bodyList> virtualBodies;
	shared_ptr<Utility> util;
	shared_ptr<Camera> viewport;
	shared_ptr<MarkupCore> readWriter;
	vector2d position;
	vector2d dimensions;
	int border;
	Pixel primary;
	Pixel secondary;
	Pixel highlight;

public:InterfaceRegion() {
	physicsBodies = nullptr;
	virtualBodies = nullptr;
	util = nullptr;
	viewport = nullptr;
	readWriter = nullptr;
	position = vector2d();
	dimensions = vector2d(150,100);
	border = 5;
	primary = Pixel(255,150,0);
	secondary = Pixel(50, 240, 255);
	highlight = Pixel(240, 240, 0);
}
public:InterfaceRegion(
	vector2d _position, vector2d _dimensions, shared_ptr<bodyList> _bodyList, shared_ptr<bodyList>  _virtuals,
	shared_ptr<Utility> _u, shared_ptr<Camera> _viewport, shared_ptr<MarkupCore> _readWriter) {
	physicsBodies = _bodyList;
	virtualBodies = _virtuals;
	util = _u;
	viewport = _viewport;
	readWriter = _readWriter;

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
		util->game_state = (util->game_state == 2 ? 1 : 2);
		break;
	case 1://force play
		util->game_state = 2;
		break;
	case 2://force pause
		util->game_state = 1;
		break;
	case 3://pause and create new (virtual?) body at camera position
		//util->game_state;
		//create virtual body at camera center
		//grab mass from slider / numerical input
		//use mouse to position
		//make new interfaceRegion and use widget to make a velocity vector with prediction path
		//use that data in _bodyList->createBody();
		//vector2d tempvec = ;
		physicsBodies->createBody(
			viewport->reverseTranslate(vector2d()),
			vector2d((double(rand() % 10) / 10) - 1,
			(double(rand() % 10) / 10) - 1), 300);
		break;
	case 4:
		//save to existing filepath
		readWriter->markupWriter();
		break;
	case 5://force edit mode
		util->game_state = 0;
		physicsBodies->reset();
		viewport->location = vector2d(0,0);//not working?
		viewport->target = nullptr;
		viewport->zoom = 1;
		break;
	case 6://edit mode -> sim paused
		util->game_state = 1;
		physicsBodies->makeActual(virtualBodies);
		break;
	}

}
};