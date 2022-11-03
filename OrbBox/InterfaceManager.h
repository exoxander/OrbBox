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
	editSim,
	runSim,
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
	iVector position;
	iVector size;
	string text;

public:
	Button() {
		type = BUTTON_TYPE::nonButton;
		position = iVector(-5,-5);
		size = iVector(10, 10);
		text = "none";
	}

	Button(BUTTON_TYPE _type, iVector _pos = iVector(-5,-5), iVector _size = iVector(10,10), string _text = "none") {
		type = _type;
		position = _pos;
		size = _size;
		text = _text;
	}
	virtual ~Button() {

	}

	bool mouseIsOn(iVector _mousePos) {
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
	ObjectHandle(shared_ptr<ScreenObject> _object, iVector _pos = iVector(-2,-2), iVector _size = iVector(4,4), string _text = "none")
		:Button(BUTTON_TYPE::handle, _pos, _size, _text) {
		item = _object;
	}
	ObjectHandle(shared_ptr<ScreenObject> _object, string _text)
		:Button(BUTTON_TYPE::handle, iVector(-2,2), iVector(4,4), _text) {
		item = _object;
	}
	~ObjectHandle() {
		item = nullptr;
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
	int activeButton;	

public:
	InterfaceManager() {
		activeButton = -1;
	}
	void addToCurrentPage(shared_ptr<ScreenObject> _object) {
		currentPage->item->addObject(_object);
	  }
	void addToCurrentPage(shared_ptr<ScreenObject> _object, shared_ptr<body> _body) {
		currentPage->item->addObject(_object, _body);
	}
	void switchToPage(){

	}
	void setAllBodyLinks() {
	}
	/*
	*/
};