#pragma once
#include <memory>
#include "PhysicsBody.h"
using std::shared_ptr;


//--------------< MATRIX ITEM >---------------
struct matrixItem {
	int a;
	int b;
	double y;
	double x;
	double distanceFactor;
	shared_ptr<matrixItem> next;
public:matrixItem() {
	a = -1;
	b = -2;
	x = 0;
	y = 0;
	distanceFactor = 0;
	next = nullptr;
}
public:matrixItem(int _a, int _b, double _x, double _y) {//make sure distanceFactors are absolute values
	a = _a;
	b = _b;
	//gravitational force falloff is 1 / distanceFactor squared
	x = _x;
	y = _y;
	distanceFactor = 1 / (x*x + y*y);

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
public:void generateMatrix(bool regen = false) {
	
	//destroy all exiting items if regenerating
	if (regen) {
		//requires a deconstructor to work properly
		//for now just delete the head and leak memory 
		head = nullptr;
	}
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
				bool xNegative = false;
				bool yNegative = false;

				double xDist = activePosition.x - linkingBody->item->position.x;
				double yDist = activePosition.y - linkingBody->item->position.y;

				currentItem = make_shared<matrixItem>(a, b,xDist,yDist);
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
public:void step(double stepFactor = 1) {
	shared_ptr<body> currentBody = allBodies->head;
	//create the matrix for this step	
	distMatrix.generateMatrix(true);

	//velocity to add  = sum of all (other objects mass * distanceFactor factor)
	//loop through each body in the body list and calculate the forces of all other bodies from the distanceFactor matrix
	while (currentBody != nullptr) {//for each body in the global list
		shared_ptr<matrixItem> currentItem = distMatrix.head;
		double dx = 0;
		double dy = 0;
		double linkMass = 0;
		int currentId = currentBody->item->id;
		while(currentItem != nullptr){
			//if there is a link
			if (currentId == currentItem->b) {//need to know which one is the other object and get *its* mass for hte link mass
				if (allBodies->exists(currentItem->a)) {
					linkMass = allBodies->getBody(currentItem->a)->item->mass;
					//adding velocities
					dx += linkMass * currentItem->x * currentItem->distanceFactor * currentBody->item->mass;
					dy += linkMass * currentItem->y * currentItem->distanceFactor * currentBody->item->mass;
				}
				
			}
			else if (currentId == currentItem->a) {
				if (allBodies->exists(currentItem->b)) {
					linkMass = allBodies->getBody(currentItem->b)->item->mass;
					//adding velocities
					dx -= linkMass * currentItem->x * currentItem->distanceFactor * currentBody->item->mass;
					dy -= linkMass * currentItem->y * currentItem->distanceFactor * currentBody->item->mass;
				}
			}			

			currentItem = currentItem->next;//move to next
		}

		// / currentBody->item->mass
		currentBody->item->velocity.add(coord((dx) * stepFactor, (dy) * stepFactor));//change existing velocity
		currentBody->item->position.add(currentBody->item->velocity);//add velocity to position
		currentBody = currentBody->next;//move to next
	}
}
};

