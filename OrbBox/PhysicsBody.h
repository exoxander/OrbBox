#pragma once
#include <math.h>
#include "Utility.h"
using std::shared_ptr;
using std::make_shared;



//===================-< PhysicsBody >-==================
class PhysicsBody
{
public:
	int id;
	double mass;
	coord velocity;
	coord position;
private:
	mesh body;

	//default
public: PhysicsBody() {
	position = coord();
	velocity = coord();
	mass = 10;
	id = -1;
	body = mesh();
}
	  //complete
public: PhysicsBody(coord _posistion, coord _velocity, double _mass = 10) {
	position = _posistion;
	velocity = _velocity;
	mass = _mass;
	id = -1;
	body = mesh();
}

	  //create mesh from vertex list | impliment later
public:mesh createMesh() {
	//origin vertex	

	mesh m = mesh();
	return m;
}

public:mesh getMesh() {
	return body;
}
public: void setMesh(mesh _m) {
	body = _m;
}
public:shared_ptr<polygon> getPolygonList() {
	return body.polygonList;
}
};

