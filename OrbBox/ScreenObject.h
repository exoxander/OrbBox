#pragma once
#include "Body.h"
#include"Utility.h"

enum struct SO_TYPE {
	none,
	physicsObject,
	visualObject,
	buttonObject
};
class ScreenObject {
public:
	shared_ptr<body> physicsBody;
	shared_ptr<mesh> collisionMesh;
	shared_ptr<mesh> visualMesh;

	vector2i screenPosition;
	double scale;
	bool hasPhysics;
	bool hasCollision;
	bool show;

public:
	//default
	ScreenObject() {
		physicsBody = nullptr;
		collisionMesh = nullptr;
		visualMesh = make_shared<mesh>(10);
		screenPosition = vector2i();
		scale = 1;
		hasPhysics = false;
		hasCollision = false;
		show = true;
	}
	
	//visual only object
	ScreenObject(vector2i _pos, shared_ptr<mesh> _visualMesh, double _scale = 1) {
		physicsBody = nullptr;
		collisionMesh = nullptr;//make getRect(visualMesh) later?
		hasPhysics = false;
		hasCollision = false;
		show = true;

		screenPosition = _pos;
		visualMesh = _visualMesh;
		scale = _scale;
	}

	//physics object by partial defnition
	ScreenObject(shared_ptr<body> _physBody, shared_ptr<mesh> _visMesh, shared_ptr<mesh> _colMesh = nullptr) {
		physicsBody = _physBody;
		visualMesh = _visMesh;
		collisionMesh = _colMesh;
		hasPhysics = true;
		show = true;
		hasCollision = (_colMesh == nullptr ? false : true);

		screenPosition = vector2i();
		scale = 1;
	}
	//physics object by complete defnition
	ScreenObject(vector2d _pos, vector2d _vel, vector2d _rot,
		shared_ptr<mesh> _visMesh, shared_ptr<mesh> _colMesh = nullptr, double _mass = 1) {
		physicsBody = make_shared<body>(_pos, _vel, _rot, _mass);
		visualMesh = _visMesh;
		collisionMesh = _colMesh;
		hasPhysics = true;
		show = true;
		hasCollision = (_colMesh == nullptr ? false : true);

		screenPosition = vector2i();
		scale = 1;
	}


};