#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
#include "MarkupCore.h"
#include "ScreenObject.h"

using std::cout;
using std::endl;
using olc::Pixel;
enum struct ACTION {
	none,
	togglePlay,
	forcePlay,
	forcePause,
	forceEdit,
	forceReset,
	saveSim,
	startSim
};

enum BUTTON_TYPE {
	none,
	button,
	handle
};
class Button{
public:
	BUTTON_TYPE type;
	vector2i position;
	vector2i size;

public:
	Button() {
		type = none;
		position = vector2i(-5,-5);
		size = vector2i(10, 10);
	}

	Button(BUTTON_TYPE _type, vector2i _pos = vector2i(-5,-5), vector2i _size = vector2i(10,10)) {
		type = _type;
		position = _pos;
		size = _size;
	}

	bool mouseIsOn(vector2i _mousePos) {
		if (_mousePos.x > position.x && _mousePos.x < position.x + size.x &&
			_mousePos.y > position.y && _mousePos.y < position.y + size.y) {
			//return true if the mouse is inside the bounds of the button
			return true;
		}
		return false;
	}
};

class ObjectHandle : public Button {
public:
	shared_ptr<ScreenObject> item;

public:
	ObjectHandle() 
		:Button() {
		item = nullptr;
	}
	ObjectHandle(shared_ptr<ScreenObject> _object, vector2i _pos = vector2i(-2,-2), vector2i _size = vector2i(4,4))
		:Button(handle, _pos, _size) {
		item = _object;
	}
};

class InterfaceManager {
public:
	list<Button> buttons;

public:InterfaceManager() {
}
	  /*
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
*/
};