#pragma once
#include <memory>
#include "PhysicsBody.h"
using std::shared_ptr;


//--------------< MATRIX ITEM >---------------
struct matrixItem {
	int aID;
	int bID;
	double aMass;
	double bMass;
	vector2d direction;
	double distance;
	shared_ptr<matrixItem> next;
public:matrixItem() {
	aID = -1;
	bID = -2;
	aMass = 10;
	bMass = 10;
	direction = vector2d();
	distance = 0;
	next = nullptr;
}
public:matrixItem(int _a, int _b,double _aMass, double _bMass, vector2d _direction, double _magnitude) {
	aID = _a;
	bID = _b;
	aMass = _aMass;
	bMass = _bMass;
	//gravitational force falloff is 1 / distance squared
	direction = _direction;
	distance = _magnitude;
	next = nullptr;
}
};

//----------------< distanceFactor MATRIX >----------------
struct distanceFactorMatrix {
public:
	shared_ptr<matrixItem> head;
	shared_ptr<bodyList> allBodies;

public:distanceFactorMatrix() {
	head = nullptr;
	allBodies = nullptr;
}
public:distanceFactorMatrix(shared_ptr<bodyList> _allBodies) {
	head = nullptr;
	allBodies = _allBodies;
	generateMatrix();//create the item matrix
}
public:void generateMatrix() {
	head = nullptr;
	//only create a matrix for more than one item in the list
	//because each link is two-way, each item only needs to create items (links) for each item after itself in the list
	//to check for a link to any particular body you just need to check the 2nd ID in matrixItem
	if (allBodies->length > 0) {
		shared_ptr<body> activeBody = allBodies->head;
		shared_ptr<body> linkingBody = activeBody->next;
		shared_ptr<matrixItem> currentItem = nullptr;
		shared_ptr<matrixItem> lastLinked = nullptr;
		while (activeBody != nullptr) {//loop through all bodies in the bodylist and create links to all bodies after it	
			int a = activeBody->item->id;
			vector2d activePosition = activeBody->item->position;

			while (linkingBody != nullptr) {//going through each body needing to be linked by the active one
				//difference between x and y coords of the two bodies
				int b = linkingBody->item->id;
				vector2d linkingPosition = linkingBody->item->position;

				vector2d normalizedDirection = vector2d(activePosition.x - linkingPosition.x, activePosition.y - linkingPosition.y);
				double magnitude = normalizedDirection.getMagnitude();
				normalizedDirection.normalize();

				currentItem = make_shared<matrixItem>(a, b, activeBody->item->mass, linkingBody->item->mass, normalizedDirection, magnitude);
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
	}
	else {
		//throw an error?
	}
}
};

//--------------------< PHYSICS SOLVER >------------------------
class PhysicsSolver {
	distanceFactorMatrix distMatrix;
	shared_ptr<bodyList> allBodies;
public:PhysicsSolver(shared_ptr<bodyList> _bodyList) {
	distMatrix = distanceFactorMatrix(_bodyList);
	allBodies = _bodyList;
}

	  //step function does all the calculations
public:void step( shared_ptr<bodyList> _bodies, double stepFactor = 1) {
	shared_ptr<body> currentBody = _bodies->head;
	//create the matrix for this step
	//distMatrix.generateMatrix(true);

	//loop through each body in the body list and calculate the forces of all other bodies from the distanceFactor matrix
	//update all accellerations first
	while (currentBody != nullptr) {//for each body in the global list
		//shared_ptr<matrixItem> currentItem = distMatrix.head;
		shared_ptr<body> actingBody = _bodies->head;
		double dx = 0;
		double dy = 0;
		double linkMass = 0;
		currentBody->item->accelleration = vector2d();
		int currentId = currentBody->item->id;

		//loop through all bodies
		//replace with distance matrix later on
		while (actingBody != nullptr) {
			if (actingBody->item->id != currentBody->item->id) {
				//get distance
				double x = currentBody->item->position.x - actingBody->item->position.x;
				double y =  currentBody->item->position.y - actingBody->item->position.y;
				double rawDistance = sqrt(x * x + y * y);
				double distanceFactor = 1 / (rawDistance >= 1 ? rawDistance : 1);//prevent acellerating to infinity at impossible distances
				
				x *= distanceFactor;//normallized x component of force vector2d
				y *= distanceFactor;//normallized y component of force vector2d

				//process of normalizing and changing mass creates a 1 / distance ^ (3/2)
				dx -= actingBody->item->mass * distanceFactor * distanceFactor * x;
				dy -= actingBody->item->mass * distanceFactor * distanceFactor * y;
				currentBody->item->accelleration.add(dx, dy);
			}
			actingBody = actingBody->next;//move to next
		}
		// / currentBody->item->mass
		currentBody->item->accelleration.multiply(stepFactor);		
		currentBody = currentBody->next;//move to next
	}

	//update all velocities, doing this seperate seems to solve an energy leak in the system
	currentBody = allBodies->head;
	while (currentBody != nullptr) {
		currentBody->item->velocity.add(currentBody->item->accelleration);//change existing velocity
		currentBody->item->position.add(currentBody->item->velocity);//add velocity to position
		currentBody = currentBody->next;
	}
}

	  //-------------------------< USING DISTANCE MATRIX >------------------------------
public: void matrixStep(double stepFactor = 1) {
	shared_ptr<body> currentBody = allBodies->head;
	//create the matrix for this step
	distMatrix.generateMatrix();

	//loop through each body in the body list and calculate the forces of all other bodies from the distanceFactor matrix
	//update all accellerations first
	while (currentBody != nullptr) {//for each body in the global list
		shared_ptr<matrixItem> currentItem = distMatrix.head;
		shared_ptr<body> actingBody = allBodies->head;
		double dx = 0;
		double dy = 0;
		double linkMass = 0;
		currentBody->item->accelleration = vector2d();
		int currentId = currentBody->item->id;
		bool hasLink = false;
		//check all links inside the matrix, invert direction as needed
		while (currentItem != nullptr) {

			vector2d forceDirection = currentItem->direction;//may need to invert depending on how link was formed
			if (currentBody->item->id == currentItem->aID) {
				hasLink = true;
				linkMass = currentItem->bMass;				
			}
			else if (currentBody->item->id == currentItem->bID) {
				hasLink = true;
				linkMass = currentItem->aMass;
				forceDirection.multiply(-1);
			}
			if (hasLink) {//if a link exist apply its mass * distance * direction as acceleration
				double distanceFactor = 1 / (currentItem->distance * currentItem->distance * currentItem->distance);
				dx -= linkMass * forceDirection.x * distanceFactor;
				dy -= linkMass * forceDirection.y * distanceFactor;
				currentBody->item->accelleration.add(dx, dy);
			}
			currentItem = currentItem->next;//move to next
			hasLink = false;
		}
		// / currentBody->item->mass
		currentItem = distMatrix.head;
		currentBody->item->accelleration.multiply(stepFactor);
		currentBody = currentBody->next;//move to next
	}

	//update all velocities, doing this seperate seems to solve an energy leak in the system
	currentBody = allBodies->head;
	while (currentBody != nullptr) {
		currentBody->item->velocity.add(currentBody->item->accelleration);//change existing velocity
		currentBody->item->position.add(currentBody->item->velocity);//add velocity to position
		currentBody = currentBody->next;
	}
}
};

