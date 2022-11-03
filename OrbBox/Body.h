#pragma once
#include <math.h>
#include "Utility.h"
using std::shared_ptr;
using std::make_shared;

/*
old code i dont want to lose
\public: void createDefaultSystem() {
		  createBody(dVector(), dVector(), 50000);//central star
		  createBody(dVector(100, 0), dVector(-.1, -1.5), 5500);//planet 1
		  createBody(dVector(-260, 40), dVector(.2, .6), 2000);//planet 2
		  createBody(dVector(-250, 45), dVector(-.25, 1.2), 300);//moon of planet 2
		  createBody(dVector(60, 350), dVector(.75, -.22), 600);//planet 3
	  }*/
//----------------------< BODY >-------------------------
struct body {
public:
	dVector position;
	dVector velocity;
	dVector acceleration;
	dVector rotation;
	double mass;
	int linkedObject;

public:
	//default
	body() {
		position = dVector();
		velocity = dVector();
		acceleration = dVector();
		rotation = dVector();
		mass = 1;
		linkedObject = -1;
}
	//with default args
	body(dVector _pos, dVector _vel = dVector(), dVector _rot = dVector(), double _mass = 1, int _linkedObject = -1) {
		position = _pos;
		velocity = _vel;
		acceleration = dVector();
		rotation = _rot;
		mass = _mass;
		linkedObject = _linkedObject;
	}

	body copy() {
		body newBody = body(position, velocity, rotation, mass, linkedObject);
		return newBody;
	}

	void copy(body _item) {
		position = _item.position;
		velocity = _item.velocity;
		acceleration = dVector();
		rotation = _item.rotation;
		mass = _item.mass;
	}
}
;



