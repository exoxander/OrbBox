#pragma once
#include <memory>
#include "Body.h"
using std::shared_ptr;
double collision_factor = 1.2;
//Ethan approved integration method
//https://lpsa.swarthmore.edu/NumInt/NumIntFourth.html

const double GRAVITY = 0.001;
const double TIMESTEP = 0.01;

enum struct EVENT_TYPE {
	none,
	gravity,
	collisionCheck,
	meshCollision,
	bubbleCollision,
};

struct event {
	EVENT_TYPE type;
	shared_ptr<body> first;
	shared_ptr<body> second;
	double alpha;
	double beta;

	event() {
		type = EVENT_TYPE::none;
		first = nullptr;
		second = nullptr;
		alpha = 0;
		beta = 0;
	}
	event(EVENT_TYPE _event, shared_ptr<body> _first, shared_ptr<body> _second, double _a = 0, double _b = 0) {
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
	list<body> pageBodies;

public:
	PhysicsSolver() {
		eventMatrix = list<event>();
		pageBodies = list<body>();
	}

	vector2d applyGravity(shared_ptr<body> _first, shared_ptr<body> _second, bool flipped = false) {//euclidian integration for now
		//(a) = g = GM / d^2

		//the acceleration of first as a result of second
		vector2d result = _first->position;
		result.subtract(_second->position);
		double mass;
		double distance = result.getMagnitude();
		distance *= distance;//d^2
		result.normalize();
		if (flipped) {
			mass = _first->mass;
		}
		else {
			result.multiply(-1);
			mass = _second->mass;
		}

		mass *= GRAVITY;//GM
		mass /= distance;//GM / d^2
		mass *= TIMESTEP;//new magnitude of acceleration

		result.multiply(mass);//final acceleration vector from gravity
		return result;
	}

	void buildEventMatrix() {
		eventMatrix.clear();
		shared_ptr<bin<body>> currentBin = pageBodies.head;
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
						if (currentEvent->item->type == EVENT_TYPE::gravity) {
							hasLink = true;
							break;
						}
					}
					currentEvent = currentEvent->next;
				}
				if (!hasLink && currentBin->item != linkBin->item) {
					eventMatrix.add(make_shared<event>(EVENT_TYPE::gravity, currentBin->item, linkBin->item));
					//set event->alpha to the distance between the two bodies
				}
				linkBin = linkBin->next;
			}
			currentBin = currentBin->next;
		}
		//check if two objects are close and mark for collision check
	}

	//run through the event matrix and compute everything
	void step(list<body> _pageBodies, double _speed = 1) {
		pageBodies = _pageBodies;
		shared_ptr<bin<body>> currentBody = _pageBodies.head;
		buildEventMatrix();
		shared_ptr<bin<event>> currentEvent = eventMatrix.head;
		while (currentEvent != nullptr) {
			if (currentEvent->item->type == EVENT_TYPE::gravity) {
				//apply for both bodies in the event link
				currentEvent->item->first->acceleration.add(applyGravity(currentEvent->item->first, currentEvent->item->second));
				currentEvent->item->second->acceleration.add(applyGravity(currentEvent->item->second, currentEvent->item->first, true));
			}
			currentEvent = currentEvent->next;
		}

		//loop through all bodies and apply + clear acceleration
		while (currentBody != nullptr) {
			currentBody->item->velocity.add(currentBody->item->acceleration);
			currentBody->item->acceleration = vector2d();
			currentBody = currentBody->next;
		}
	}
};

