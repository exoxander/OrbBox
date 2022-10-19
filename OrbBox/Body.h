#pragma once
#include <math.h>
#include "Utility.h"
using std::shared_ptr;
using std::make_shared;

/*
old code i dont want to lose
\public: void createDefaultSystem() {
		  createBody(vector2d(), vector2d(), 50000);//central star
		  createBody(vector2d(100, 0), vector2d(-.1, -1.5), 5500);//planet 1
		  createBody(vector2d(-260, 40), vector2d(.2, .6), 2000);//planet 2
		  createBody(vector2d(-250, 45), vector2d(-.25, 1.2), 300);//moon of planet 2
		  createBody(vector2d(60, 350), vector2d(.75, -.22), 600);//planet 3
	  }*/
//----------------------< BODY >-------------------------
struct body {
public:
	vector2d position;
	vector2d velocity;
	vector2d acceleration;
	vector2d rotation;
	double mass;

public:
	//default
	body() {
		position = vector2d();
		velocity = vector2d();
		acceleration = vector2d();
		rotation = vector2d();
		mass = 1;
}
	//with default args
	body(vector2d _pos, vector2d _vel = vector2d(), vector2d _rot = vector2d(), double _mass = 1) {
		position = _pos;
		velocity = _vel;
		acceleration = vector2d();
		rotation = _rot;
		mass = _mass;
	}
}
;



