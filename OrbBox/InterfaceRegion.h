#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
#include "MarkupCore.h"

using std::cout;
using std::endl;
using olc::Pixel;
enum struct ACTION {
	togglePlay,
	forcePlay,
	forcePause,
	forceEdit,
	forceReset,
	saveSim,
	startSim
};
struct interfaceItem {
	vector2d position;
	int id;
};

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
	//position.multiply(_position);
	dimensions = _dimensions;
	//dimensions.multiply(_position);
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
public:void takeAction(ACTION _action) {
	switch (_action) {
	case ACTION::togglePlay://toggle pause / play
		util->game_state = (util->game_state == GAME_STATE::pause ? GAME_STATE::play : GAME_STATE::pause);
		break;
	case ACTION::forcePlay://force play
		util->game_state = GAME_STATE::play;
		break;
	case ACTION::forcePause ://force pause
		util->game_state = GAME_STATE::pause;
		break;
	case ACTION::forceEdit://go into edit (NOT RESET) mode and add virtual body with handles
		break;
	case ACTION::saveSim:
		//save to existing filepath
		readWriter->markupWriter();
		break;
	case ACTION::forceReset://force reset mode
		util->game_state = GAME_STATE::edit;
		physicsBodies->reset();
		viewport->location = vector2d();//not working?
		viewport->target = nullptr;
		viewport->zoom = 1;
		break;
	case ACTION::startSim://edit mode -> sim paused
		util->game_state = GAME_STATE::pause;
		physicsBodies->makeActual(virtualBodies);
		break;
	}
}
};