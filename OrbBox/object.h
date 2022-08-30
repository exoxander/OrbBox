
#pragma once
#include <vector>
using std::shared_ptr;
using std::make_shared;
//===================-< COORDINANT >-==================
struct coord {

	double x;
	double y;

public: coord(double _x = 0, double _y = 0) {
	set(_x, _y);
}
public: void set(double _x, double _y) {
	x = _x;
	y = _y;
}
public: void add(double _x, double _y) {
	x += _x;
	y += _y;
}
public: void add(coord _value) {
	x += _value.x;
	y += _value.y;
}
};

struct vertex {
public:
	coord position;
	int id;
	shared_ptr<vertex> next;
	shared_ptr<vertex> prev;

public: vertex() {
	position = coord();
	id = -1;
	next = nullptr;
	prev = nullptr;
}

public: vertex(double _a, double _b, int _id = -1) {
	position.x = _a;
	position.y = _b;
	id = _id;
	next = nullptr;
	prev = nullptr;
}
};

struct polygon {
public:
	int id;
	shared_ptr<vertex> a;
	shared_ptr<vertex> b;
	shared_ptr<vertex> c;
	shared_ptr<polygon> next;
	shared_ptr<polygon> prev;

public:polygon() {
	a = nullptr;
	b = nullptr;
	c = nullptr;
	id = -1;
	next = nullptr;
	prev = nullptr;
}
public: polygon(shared_ptr<vertex> _a, shared_ptr<vertex> _b, shared_ptr<vertex> _c, int _id = -1) {
	a = _a;
	b = _b;
	c = _c;
	id = _id;
	next = nullptr;
	prev = nullptr;
}
};


struct mesh {
	shared_ptr<vertex> vertexList;
	int vexLength;

	shared_ptr<polygon> polygonList;
	int pgonLength;

public: mesh() {//create default
	//initialize vertex list
	vertex a = vertex(0, 0, 0);
	vertexList = make_shared<vertex>(a);

	vertex b = vertex(20, 0, 1);
	vertexList->next = make_shared<vertex>(b);
	b.prev = vertexList;

	vertex c = vertex(0, 20, 2);
	c.prev = make_shared<vertex>(b);
	vertexList->next->next = make_shared<vertex>(c);

	vexLength = 3;

	//create polygon
	polygon p = polygon(vertexList, vertexList->next, vertexList->next->next, 0);
	polygonList = make_shared<polygon>(p);

	pgonLength = 1;
}

public: mesh(shared_ptr<vertex> _vertexList, shared_ptr<polygon> _polygonList, int _vexLength, int _pgonLength) {//create from existing lists
	vertexList = _vertexList;
	polygonList = _polygonList;
	vexLength = _vexLength;
	pgonLength = _pgonLength;
}
};


//===================-< PhysicsBody >-==================
class PhysicsBody
{
public:
	int id;
	double mass;//xyyy | x = size class?
	coord velocity;
	coord position;
	//coord screenSpace;
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
public:coord translate(coord absolute, coord relative) {//position of an PhysicsBody, position of a vertex
	coord result((relative.x + absolute.x), (relative.y + absolute.x));

	return result;
}
public:mesh getMesh() {
	return body;
}
public:shared_ptr<polygon> getPolygonList() {
	return body.polygonList;
}
};

