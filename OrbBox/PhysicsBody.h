#pragma once
#include <math.h>
#include "Utility.h"
using std::shared_ptr;
using std::make_shared;
#define PI 3.14159265//used in trig functions



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
public:mesh generateCircle(int _radius = 10, int _vertecies = 12) {//generates a circle with the desired radius and vertex count
	mesh m;
	//origin vertex
	shared_ptr<vertex> origin = make_shared<vertex>(vertex(0, 0, 0));
	shared_ptr<vertex> current = origin;
	m.vertexList = origin;
	m.vexLength = 1;

	double angle = (2 * PI) / double(_vertecies);

	for (int i = 0; i < _vertecies; i++) {
		//generate vertecies and add to vertexList
		//x1 = x0*cos() - y0*sin()
		//y1 = x0*sin() + y0*cos()
		//create vertex by angle * i starting at coord (radius, 0)
		coord next = coord(_radius * cos(angle * i), _radius * sin(angle * i));//coordinant of next vertex
		shared_ptr<vertex> vert = make_shared<vertex>(vertex(next, i + 1));

		//add to vertex chain and increase vexlength
		current->next = vert;
		vert->prev = current;
		vert->id = i + 1;
		m.vexLength++;
		current = vert;
	}

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

