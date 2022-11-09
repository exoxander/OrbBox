#pragma once
#include "Body.h"
#include"Utility.h"

enum struct SO_TYPE {
	none,
	special,
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

	~ScreenObject() {
		physicsBody = nullptr;
		visualMesh = nullptr;
		collisionMesh = nullptr;
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
		physicsBody = (_object.physicsBody ? make_shared<body>(_object.physicsBody->copy()) : nullptr);
		visualMesh = (_object.visualMesh ? make_shared<mesh>(_object.visualMesh->copy()) : nullptr);
		collisionMesh = (_object.collisionMesh ? make_shared<mesh>(_object.collisionMesh->copy()) : nullptr);

		hasPhysics = _object.hasPhysics;
		show = _object.show;
		hasCollision = _object.hasCollision;

	}

	void computeRect() {

	}

	void returnRect(shared_ptr<iVector> a, shared_ptr<iVector> b, bool useVisual = false) {
		//return the position and length / width
		//if using a mesh loop through mesh (visual or collision?) find highest, leftest, lowest and rightest points
		//and form a rectangle from that
	}
};