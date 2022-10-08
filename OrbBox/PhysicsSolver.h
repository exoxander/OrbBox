#pragma once
#include <memory>
#include "Body.h"
using std::shared_ptr;
double collision_factor = 1.2;
//Ethan approved integration method
//https://lpsa.swarthmore.edu/NumInt/NumIntFourth.html

enum struct eventType {
	none,
	gravity,
	meshCollision,
	bubbleCollision,
};

struct event {
	eventType type;
	shared_ptr<body> first;
	shared_ptr<body> second;
	double alpha;
	double beta;

	event() {
		type = eventType::none;
		first = nullptr;
		second = nullptr;
		alpha = 0;
		beta = 0;
	}
	event(eventType _event, shared_ptr<body> _first, shared_ptr<body> _second, double _a = 0, double _b = 0) {
		type = _event;
		first = _first;
		second = _second;
		alpha = _a;
		beta = _b;
	}
};




//--------------------< PHYSICS SOLVER >------------------------
class PhysicsSolver {
	list<event> eventMatrix;
	list<body> physicsBodies;

public:
	PhysicsSolver() {
	}

	//run through the event matrix and compute everything
	void step(double _speed = 1) {

	}

	void buildEventMatrix() {
		eventMatrix.clear();
		shared_ptr<bin<body>> currentBin = physicsBodies.head;
		shared_ptr<bin<body>> linkBin;
		shared_ptr<bin<event>> currentEvent;
		bool hasLink;

		//find all distances for gravity
		//check if a link exists between the two items, if not add link to eventMatrix
		//loop through all bodies in the body list
		while (currentBin != nullptr) {
			linkBin = currentBin->next;
			while (linkBin != nullptr) {				
				currentEvent = eventMatrix.head;
				//check if link between currentBin and linkBin exists in eventMatrix
				hasLink = false;
				while(currentEvent != nullptr){

					if (currentEvent->item->first == currentBin->item && currentEvent->item->second == linkBin->item
						|| currentEvent->item->second == currentBin->item && currentEvent->item->first == linkBin->item) {
						//only generate this part for gravity
						if (currentEvent->item->type != eventType::gravity) {
							hasLink = true;
						}
					}
					currentEvent = currentEvent->next;
				}
				if (!hasLink && currentBin->item != linkBin->item) {
					eventMatrix.add(make_shared<event>(eventType::gravity, currentBin->item, linkBin->item));
					//set event->alpha to the distance between the two bodies
				}
				linkBin = linkBin->next;
			}
			currentBin = currentBin->next;
		}
		//check if two objects are close and mark for collision check
	}
};

