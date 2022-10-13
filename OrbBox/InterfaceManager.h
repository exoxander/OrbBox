#pragma once
#include "Utility.h"
#include "olcPixelGameEngine.h"
//#include "MarkupCore.h"
#include "ScreenObject.h"

using std::cout;
using std::endl;
using olc::Pixel;
enum struct ACTION {
	nonAction,
	togglePlay,
	forcePlay,
	forcePause,
	forceEdit,
	forceReset,
	saveSim,
	loadSim,
	startSim,
	exportMesh,
	loadMesh,
	switchPage
};

enum struct PAGE_TYPE {
	nonPage,
	menu,
	simulation,
	meshEdit
};

enum struct BUTTON_TYPE {
	nonButton,
	button,
	handle
};
class Button{
public:
	BUTTON_TYPE type;
	vector2i position;
	vector2i size;
	string text;

public:
	Button() {
		type = BUTTON_TYPE::nonButton;
		position = vector2i(-5,-5);
		size = vector2i(10, 10);
		text = "none";
	}

	Button(BUTTON_TYPE _type, vector2i _pos = vector2i(-5,-5), vector2i _size = vector2i(10,10), string _text = "none") {
		type = _type;
		position = _pos;
		size = _size;
		text = _text;
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
	ObjectHandle(shared_ptr<ScreenObject> _object, vector2i _pos = vector2i(-2,-2), vector2i _size = vector2i(4,4), string _text = "none")
		:Button(BUTTON_TYPE::handle, _pos, _size, _text) {
		item = _object;
	}
};

class Page {
public:
	list<ScreenObject> pageObjects;
	list<body> pageBodies;
	list<Button> pageButtons;
	PAGE_TYPE type;

public:
	Page() {
		pageObjects = list<ScreenObject>();
		pageBodies = list<body>();
		pageButtons = list<Button>();
		type = PAGE_TYPE::nonPage;
	}

	//by full definition, as in loading from file
	Page(list<ScreenObject> _screenObjects, list<Button> _buttons, PAGE_TYPE _type) {
		pageObjects = _screenObjects;
		pageBodies = getAllBodies(_screenObjects);
		pageButtons = _buttons;
		type = _type;
	}

	//return all physics bodies inside the screenobjects list
	list<body> getAllBodies(list<ScreenObject> _objects) {
		list<body> newList = list<body>();
		shared_ptr<bin<ScreenObject>> currentObject = _objects.head;
		while (currentObject != nullptr) {

			//if listed as having physics and a physics body exists
			if (currentObject->item->hasPhysics && currentObject->item->physicsBody != nullptr) {
				newList.add(currentObject->item->physicsBody);
			}
			currentObject = currentObject->next;
		}

		return newList;
	}

	//add a screen object
	void addObject(shared_ptr<ScreenObject> _object) {
		pageObjects.add(_object);
		if (_object->hasPhysics && _object->physicsBody != nullptr) {
			pageBodies.add(_object->physicsBody);
		}
	}

	//add object and associated body
	void addObject(shared_ptr<ScreenObject> _object, shared_ptr<body> _body) {
		pageObjects.add(_object);
		_object->hasPhysics = true;
		_object->physicsBody = _body;
		pageBodies.add(_body);
	}
};

class InterfaceManager {
public:
	list<Button> buttons;
	list<Page> pages;
	shared_ptr<bin<Page>> currentPage;

public:
	InterfaceManager() {
	}
	void addToCurrentPage(shared_ptr<ScreenObject> _object) {
		currentPage->item->addObject(_object);
	  }
	void addToCurrentPage(shared_ptr<ScreenObject> _object, shared_ptr<body> _body) {
		currentPage->item->addObject(_object, _body);
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