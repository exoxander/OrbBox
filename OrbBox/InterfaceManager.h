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
	list<ScreenObject> staticObjects;
	list<ScreenObject> dynamicObjects;
	list<body> staticBodies;
	list<body> dynamicBodies;
	list<Button> pageButtons;
	PAGE_TYPE type;

public:
	Page() {
		staticObjects = list<ScreenObject>();
		dynamicObjects = list<ScreenObject>();
		staticBodies = list<body>();
		dynamicBodies = list<body>();
		pageButtons = list<Button>();
		type = PAGE_TYPE::nonPage;
	}

	//by full definition, as in loading from file
	Page(list<ScreenObject> _screenObjects, list<Button> _buttons, PAGE_TYPE _type) {
		staticObjects = _screenObjects;
		dynamicObjects = list<ScreenObject>();//copy from physics bodies
		staticBodies = getAllBodies(_screenObjects);
		dynamicBodies = list<body>();//copy from physics bodies
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

	void dynamicObjectsVisible(bool _value) {
		shared_ptr<bin<ScreenObject>> currentObject = dynamicObjects.head;
		while (currentObject != nullptr) {
			currentObject->item->show = _value;
			currentObject = currentObject->next;
		}
	}

	void staticObjectsVisible(bool _value) {
		shared_ptr<bin<ScreenObject>> currentObject = staticObjects.head;
		while (currentObject != nullptr) {
			currentObject->item->show = _value;
			currentObject = currentObject->next;
		}
	}
	 
	//add a screen object
	void addStaticObject(shared_ptr<ScreenObject> _object) {
		staticObjects.add(_object);
		if (_object->physicsBody != nullptr) {
			staticBodies.add(_object->physicsBody);
			staticObjects.tail->item->physicsBody->linkedObject = staticBodies.tail->itemID;
		}
	}
	void addDynamicObject(shared_ptr<ScreenObject> _object) {
		dynamicObjects.add(_object);
		if (_object->physicsBody != nullptr) {
			dynamicBodies.add(_object->physicsBody);
			dynamicObjects.tail->item->physicsBody->linkedObject = dynamicBodies.tail->itemID;
		}
	}

	//add object and associated body
	void addStaticObject(shared_ptr<ScreenObject> _object, shared_ptr<body> _body) {
		staticObjects.add(_object);
		_object->hasPhysics = true;
		_object->physicsBody = _body;
		staticBodies.add(_body);
		staticBodies.tail->item->linkedObject = staticObjects.tail->itemID;
	}
	void addDynamicObject(shared_ptr<ScreenObject> _object, shared_ptr<body> _body) {
		dynamicObjects.add(_object);
		_object->hasPhysics = true;
		_object->physicsBody = _body;
		dynamicBodies.add(_body);
		dynamicBodies.tail->item->linkedObject = dynamicObjects.tail->itemID;
	}

	void linkDynamicObjects() {
		//do later
	}

	void copyDynamicObjects(list<ScreenObject> screenObjects, list<body> bodies) {
		//copy bodies
		dynamicBodies = bodies.deepCopy(body());
		shared_ptr<bin<body>> currentBody = dynamicBodies.head;
		//copy ScreenObjects
		//custom deepCopy of list
		dynamicObjects = list<ScreenObject>();
		shared_ptr<bin<ScreenObject>> currentObject = screenObjects.head;
		//only deep copy objects with physics
		while (currentObject != nullptr) {
			if (currentObject->item->hasPhysics) {
				dynamicObjects.deepCopyBin(currentObject);
			}
			currentObject = currentObject->next;
		}

		currentObject = dynamicObjects.head;
		//set screenObject pointers to body link IDs
		while (currentBody != nullptr) {
			currentObject->item->hasPhysics = false;
			int ID = currentObject->itemID;
			shared_ptr<ScreenObject> temp = dynamicObjects.getByBinID(ScreenObject(), currentBody->item->linkedObject);
			if (temp != nullptr) {
				temp->physicsBody = currentBody->item;
				temp->hasPhysics = true;
			}
			currentBody = currentBody->next;
		}
	}
};

class InterfaceManager {
public:
	list<Button> buttons;
	list<Page> pages;
	shared_ptr<bin<Page>> currentPage;
	list<dVector> renderedPaths;
	int activeButton;

public:
	InterfaceManager() {
		activeButton = -1;
	}
	void addToCurrentPage(shared_ptr<ScreenObject> _object, bool dynamic = false) {
		if (dynamic) {
			currentPage->item->addDynamicObject(_object);
		}
		else {
			currentPage->item->addStaticObject(_object);
		}
	  }
	void addToCurrentPage(shared_ptr<ScreenObject> _object, shared_ptr<body> _body, bool dynamic = false) {
		if (dynamic) {
			currentPage->item->addDynamicObject(_object, _body);
		}
		else {
			currentPage->item->addStaticObject(_object, _body);
		}
	}
	void switchToPage(){

	}
	void setAllBodyLinks() {
	}
	//creating all object handles
	void clearObjectHandles() {
		shared_ptr<bin<Button>> currentButton = currentPage->item->pageButtons.head;

		//clear all existing handles
		while (currentButton != nullptr) {
			if (currentButton->item->type == BUTTON_TYPE::handle) {
				currentPage->item->pageButtons.removeByItem(currentButton->item);
			}
			currentButton = currentButton->next;
		}
	}
	void buildObjectHandles() {
		if (currentPage != nullptr ) {//&& currentPage->item->type == PAGE_TYPE::editSim
			shared_ptr<bin<ScreenObject>> currentObject = currentPage->item->staticObjects.head;
			clearObjectHandles();
			//make all new handles
			while (currentObject != nullptr) {
				if (currentObject->item->hasPhysics) {
					currentPage->item->pageButtons.add(
						std::dynamic_pointer_cast<Button>(make_shared<ObjectHandle>(currentObject->item, iVector(-6, -6), iVector(12, 12), "Handle")));
				}
				currentObject = currentObject->next;
			}
		}
	}
	/*
	*/
};