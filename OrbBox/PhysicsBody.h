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
public: PhysicsBody(coord _posistion, coord _velocity, double _mass = 10) {
	position = _posistion;
	velocity = _velocity;
	mass = _mass;
	id = -1;
	body = mesh();
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
	length = 0;
}
public:void addBody(PhysicsBody _body) {//call addbody overload for pointer
	addBody(make_shared<PhysicsBody>(_body));
}
public:void addBody(shared_ptr<PhysicsBody> _body) {
	if (length == 0) {
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

//--------------< MATRIX ITEM >---------------
struct matrixItem {
	int a;
	int b;
	double xFalloff;
	double yFalloff;
	shared_ptr<matrixItem> next;
public:matrixItem() {
	a = -1;
	b = -2;
	xFalloff = 1;
	yFalloff = 1;
	next = nullptr;
}
public:matrixItem(int _a, int _b, double xDist, double yDist) {//make sure distances are absolute values
	a = _a;
	b = _b;
	//gravitational force falloff is 1 / distance squared
	xFalloff = 1 / (sqrt(xDist));
	yFalloff = 1 / (sqrt(yDist));
	next = nullptr;
}
};

//----------------< DISTANCE MATRIX >----------------
struct distanceMatrix {
public:
	shared_ptr<matrixItem> head;
	shared_ptr<bodyList> allBodies;
public:distanceMatrix(shared_ptr<bodyList> _allBodies) {
	head = nullptr;
	allBodies = _allBodies;
}
public:void generateMatrix() {
	//only create a matrix for more than one item in the list
	//because each link is two-way, each item only needs to create items (links) for each item after itself in the list
	//to check for a link to any particular body you just need to check the 2nd ID in matrixItem
	if (allBodies->length > 1) {
		shared_ptr<body> activeBody = allBodies->head;
		shared_ptr<body> linkingBody = activeBody->next;
		shared_ptr<matrixItem> currentItem = nullptr;
		shared_ptr<matrixItem> lastLinked = nullptr;

		while (activeBody != nullptr) {//loop through all bodies in the bodylist and create links to all bodies after it	
			int a = activeBody->item->id;
			coord activePosition = activeBody->item->position;

			while (linkingBody != nullptr) {//going through each body needing to be linked by the active one
				//difference between x and y coords of the two bodies
				int b = linkingBody->item->id;
				double xDist = abs(activePosition.x - linkingBody->item->position.x);
				double yDist = abs(activePosition.y - linkingBody->item->position.y);
				currentItem = make_shared<matrixItem>(a, b, xDist, yDist);
				//add to matrix
				if (head == nullptr) {
					head = currentItem;
				}
				else {
					lastLinked->next = currentItem;
				}
				lastLinked = currentItem;
				linkingBody = linkingBody->next;
			}
			//set to next activebody
			activeBody = activeBody->next;
		}
	}else{
		//throw an error?
	}
}
};



