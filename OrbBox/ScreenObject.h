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

	iVector screenPosition;
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
		screenPosition = iVector();
		scale = 1;
		hasPhysics = false;
		hasCollision = false;
		show = true;
	}
	
	//visual only object
	ScreenObject(iVector _pos, shared_ptr<mesh> _visualMesh, double _scale = 1) {
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

		screenPosition = iVector();
		scale = 1;
	}
	//physics object by complete defnition
	ScreenObject(dVector _pos, dVector _vel, dVector _rot,
		shared_ptr<mesh> _visMesh, shared_ptr<mesh> _colMesh = nullptr, double _mass = 1) {
		physicsBody = make_shared<body>(_pos, _vel, _rot, _mass);
		visualMesh = _visMesh;
		collisionMesh = _colMesh;
		hasPhysics = true;
		show = true;
		hasCollision = (_colMesh == nullptr ? false : true);

		screenPosition = iVector();
		scale = 1;
	}

	ScreenObject copy() {
		ScreenObject newObject = ScreenObject();
		if (physicsBody) { newObject.physicsBody = make_shared<body>(physicsBody->copy()); }//repeat for pointers
		if (visualMesh) { newObject.visualMesh = make_shared<mesh>(visualMesh->copy()); }
		if (collisionMesh) { newObject.collisionMesh = make_shared<mesh>(collisionMesh->copy()); }

		newObject.hasPhysics = hasPhysics;
		newObject.show = show;
		newObject.hasCollision = hasCollision;

		return newObject;
	}
	void copy(ScreenObject _object) {

	}


};