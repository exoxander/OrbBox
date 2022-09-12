#pragma once
#include <math.h>
#include "Utility.h"
using std::shared_ptr;
using std::make_shared;


//===================-< PhysicsBody >-==================
class PhysicsBody
{
public:
	int id;
	double mass;
	vector2d velocity;
	vector2d position;
	vector2d accelleration;
private:
	mesh body;

	//default
public: PhysicsBody() {
	position = vector2d();
	velocity = vector2d();
	accelleration = vector2d();
	mass = 10;
	id = -1;
	body = mesh();
}
	  //complete
public: PhysicsBody(vector2d _posistion, vector2d _velocity, double _mass = 10, int _id = -1) {
	position = _posistion;
	velocity = _velocity;
	mass = _mass;
	id = _id;
	body = mesh(sqrt(_mass/100),int(6+sqrt(_mass/100)));
}

	  //create mesh from vertex list | impliment later
public:mesh createMesh() {
	//origin vertex	

	mesh m = mesh();
	return m;
}

public:mesh getMesh() {
	return body;
}
public: void setMesh(mesh _m) {
	body = _m;
}
public:shared_ptr<polygon> getPolygonList() {
	return body.polygonList;
}
};

class virtualBody {
	vector2d position;
	vector2d velocity;
	double mass;
	int radius;
	int id;
public: virtualBody() {
	position = vector2d();
	velocity = vector2d();
	mass = 1;
	radius = 10;
	int id = -1;
}
public:virtualBody(vector2d _position, vector2d _velocity, double _mass, int _id) {
	position = _position;
	velocity = _velocity;
	mass = _mass;
	radius = int(sqrt(_mass / 100));
	id = _id;
}
};

//-------------------------< GLOBAL OBJECT LIST >-------------------------
struct body {
	shared_ptr<PhysicsBody> item;
	shared_ptr<body> next;
	shared_ptr<body> prev;

public:body() {
	item = nullptr;
	next = nullptr;
	prev = nullptr;
}
public:body(shared_ptr<PhysicsBody> _item) {
	item = _item;
	next = nullptr;
	prev = nullptr;
}
};

//---------------------< BODY LIST >--------------------------
struct bodyList {
public:
	shared_ptr<body> head;
	shared_ptr<body> tail;
	int length;
	int createdBodies;

public:bodyList() {
	head = nullptr;
	tail = nullptr;
	length = -1;
	createdBodies = 0;
}
public:void addBody(PhysicsBody _body) {//call addbody overload for pointer
	addBody(make_shared<PhysicsBody>(_body));
}
public:void addBody(shared_ptr<PhysicsBody> _body) {
	if (length == 0 || length == -1) {
		length = 1;
		head = make_shared<body>(_body);
		tail = make_shared<body>(_body);
	}
	else if(length == 1) {
		length++;
		shared_ptr<body> b = make_shared<body>();
		b->item = _body;
		head->next = b;
		b->prev = head;
		tail = b;
	}
	else {
		length++;
		shared_ptr<body> b = make_shared<body>();
		b->item = _body;
		tail->next = b;
		b->prev = tail;
		tail = b;
	}
}
public:bool exists(int _id) {
	bool found = false;
	shared_ptr<body> currentBody = head;
	while (currentBody != nullptr) {
		if (currentBody->item->id == _id) {
			found = true;
			break;
		}
		else {
			currentBody = currentBody->next;
		}

	}
	return found;
}
public:shared_ptr<body> getBody(int _id) {
	shared_ptr<body> currentBody = head;
	while (currentBody != nullptr) {
		if (currentBody->item->id == _id) {
			return currentBody;
		}
		currentBody = currentBody->next;
	}
	return shared_ptr<body>();//to make the compiler happy, never actually gets used
}
public: void removeBody(int _id) {
	bool found = false;
	shared_ptr<body> current = head;
	do {
		if (current->item->id == _id) {
			//logically remove from list
			current->prev->next = current->next;
			current->next->prev = current->prev;
			//call deconstructor on current once thats completed
			length--;
			break;
		}
	} while (current->next);
	if (!found) {
		//throw an error?
		//or could just use .exists to check beforehand
	}
}
public:void createBody(vector2d _position, vector2d _velocity, double _mass) {
	createdBodies++;
	PhysicsBody newPhysicsBody = PhysicsBody(_position, _velocity, _mass, createdBodies);//create physics body
	addBody(newPhysicsBody);//add to global list
}
};




