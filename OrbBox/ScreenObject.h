#pragma once
#include "Body.h"
#include"Utility.h";

class ScreenObject {
public:
	shared_ptr<body> physicsBody;
	shared_ptr<mesh> collisionMesh;
	shared_ptr<mesh> visualMesh;

	vector2i screenPosition;
	double scale;
	int ID;
	bool hasPhysics;
	bool hasCollision;

public:
	//default
	ScreenObject() {
		physicsBody = nullptr;
		collisionMesh = nullptr;
		visualMesh = make_shared<mesh>(10);
		screenPosition = vector2i();
		scale = 1;
		ID = -1;
		hasPhysics = false;
		hasCollision = false;
	}
	
	//visual only object
	ScreenObject(vector2i _pos, shared_ptr<mesh> _visualMesh, double _scale = 1, int _ID = -1) {
		physicsBody = nullptr;
		collisionMesh = nullptr;//make getRect(visualMesh) later?
		hasPhysics = false;
		hasCollision = false;

		screenPosition = _pos;
		visualMesh = _visualMesh;
		scale = _scale;
		ID = _ID;
	}

};