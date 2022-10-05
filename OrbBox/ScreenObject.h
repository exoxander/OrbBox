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

	//physics object by partial defnition
	ScreenObject(shared_ptr<body> _physBody, shared_ptr<mesh> _visMesh, shared_ptr<mesh> _colMesh = nullptr, int _ID = -1) {
		physicsBody = _physBody;
		visualMesh = _visMesh;
		collisionMesh = _colMesh;
		hasPhysics = true;
		hasCollision = (_colMesh == nullptr ? false : true);
		ID = _ID;

		screenPosition = vector2i();
		scale = 1;
	}
	//physics object by complete defnition
	ScreenObject(vector2d _pos, vector2d _vel, vector2d _rot,
		shared_ptr<mesh> _visMesh, shared_ptr<mesh> _colMesh = nullptr, double _mass = 1, int _ID = -1) {
		physicsBody = make_shared<body>(_pos, _vel, _rot, _ID, _mass);
		visualMesh = _visMesh;
		collisionMesh = _colMesh;
		hasPhysics = true;
		hasCollision = (_colMesh == nullptr ? false : true);
		ID = _ID;

		screenPosition = vector2i();
		scale = 1;
	}


};