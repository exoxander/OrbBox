
#pragma once
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
	coord position;
	int id;
	vertex* next;
	vertex* prev;

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
	vertex* a;
	vertex* b;
	vertex* c;
	polygon* next;
	polygon* prev;

public: polygon(vertex* _a, vertex* _b, vertex* _c, int _id = -1) {
	a = _a;
	b = _b;
	c = _c;
	id = _id;
	next = nullptr;
	prev = nullptr;
}
};


struct mesh {
	vertex* vertexList;
	int vexLength;

	polygon* polygonList;
	int pgonLength;

public: mesh() {//create default
	//initialize vertex list
	vertex a = vertex(0, 0, 0);
	vertexList = &a;

	vertex b = vertex(20, 0, 1);
	vertexList->next = &b;
	b.prev = vertexList;

	vertex c = vertex(0, 20, 2);
	b.next = &c;

	vexLength = 3;

	//create polygon
	polygon p = polygon(vertexList, vertexList->next, vertexList->next->next, 0);
	polygonList = &p;

	pgonLength = 1;
}

public: mesh(vertex* _vertexList, polygon* _polygonList, int _vexLength, int _pgonLength) {//create from existing lists
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
public:polygon* getPolygonList() {
	return body.polygonList;
}
};

