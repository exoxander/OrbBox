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
	coord velocity;
	coord position;
private:
	mesh body;

	//default
public: PhysicsBody() {
	position = coord();
	velocity = coord();
	mass = 10;
	id = -1;
	body = mesh();
}
	  //complete
public: PhysicsBody(coord _posistion, coord _velocity, double _mass = 10, int _vertecies = 12) {
	position = _posistion;
	velocity = _velocity;
	mass = _mass;
	id = -1;
	body = mesh(sqrt(_mass/10),_vertecies);
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

public:bodyList() {
	head = nullptr;
	tail = nullptr;
	length = -1;
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
		//throw an error
	}
}

};




