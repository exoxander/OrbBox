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
	double radius;
	double mass;
	vector2d velocity;
	vector2d position;
	vector2d accelleration;
	vector2d rotation;//x is angle in radians, y is rotation velocity
	mesh bodyMesh;

	//default
public: PhysicsBody() {
	position = vector2d();
	velocity = vector2d();
	accelleration = vector2d();
	rotation = vector2d();
	mass = 10;
	radius = 10;
	id = -1;
	bodyMesh = mesh();
}
	  //complete
public: PhysicsBody(vector2d _posistion, vector2d _velocity, double _mass = 10, int _id = -1) {
	position = _posistion;
	velocity = _velocity;
	rotation = vector2d();
	mass = _mass;
	id = _id;
	radius = sqrt(_mass / 100);
	bodyMesh = mesh(radius, int(6 + radius));
}

public: PhysicsBody(vector2d _posistion, vector2d _velocity, double _mass, int _id, mesh _mesh) {
	position = _posistion;
	velocity = _velocity;
	rotation = vector2d();
	mass = _mass;
	id = _id;
	radius = sqrt(_mass / 100);
	bodyMesh = _mesh;
}

	  //create mesh from vertex list | impliment later
public:mesh createMesh() {
	//origin vertex	

	mesh m = mesh();
	return m;
}

public:mesh getMesh() {
	return bodyMesh;
}
public: void setMesh(mesh _m) {
	bodyMesh = _m;
}
public:shared_ptr<polygon> getPolygonList() {
	return bodyMesh.polygonList;
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
public:void reset() {
	head = nullptr;
	tail = nullptr;
	length = 0;
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
public:void createBody(vector2d _position, vector2d _velocity, double _mass, mesh _mesh) {//now with mesh!
	createdBodies++;
	PhysicsBody newPhysicsBody = PhysicsBody(_position, _velocity, _mass, createdBodies, _mesh);//create physics body
	addBody(newPhysicsBody);//add to global list
}
public:void makeActual(shared_ptr<bodyList> _bodyList) {//copy all items from virtual list to actual list
	shared_ptr<body> currentBody = _bodyList->head;
		while (currentBody != nullptr) {
			vector2d nextPosition = currentBody->item->position;
			vector2d nextVelocity = currentBody->item->velocity;
			int nextID = currentBody->item->id;
			double nextMass = currentBody->item->mass;
			mesh nextMesh = currentBody->item->bodyMesh;

			createBody(nextPosition, nextVelocity, nextMass, nextMesh);
			currentBody = currentBody->next;
	}
}
};




