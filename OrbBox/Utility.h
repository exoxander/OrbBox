#pragma once
#include <fstream>
using std::fstream;
using std::shared_ptr;
using std::make_shared;
using std::string;

#define PI 3.14159265//used in trig functions

//general list to replace the horrendous random implimentations
//struct List<Type t> {
//};

//game states
enum struct GAME_STATE {
	edit=0,
	pause,
	play

};
//===================-< vector2d >-==================
struct vector2d {

	double x;
	double y;

public: vector2d(double _x = 0, double _y = 0) {
	set(_x, _y);
}
public: void set(double _x, double _y) {
	x = _x;
	y = _y;
}
	  //math on self
public: void add(double _x, double _y) {
	x += _x;
	y += _y;
}
public: void add(vector2d _value) {
	x += _value.x;
	y += _value.y;
}
public:void subtract(vector2d _value) {
	x -= _value.x;
	y -= _value.y;
}
public:void multiply(vector2d _value) {
	x *= _value.x;
	y *= _value.y;
}
public:void multiply(double _value) {
	x *= _value;
	y *= _value;
}
public:double getMagnitude() {
	return sqrt(x * x + y * y);
}
public:vector2d normalize() {
	double magnitude = getMagnitude();
	if (magnitude != 0) {
		return vector2d(x / magnitude, y / magnitude);
	}
	else {
		return vector2d();
	}
}
};


//---------------< VERTEX > --------------
struct vertex {
public:
	vector2d position;
	int id;
	shared_ptr<vertex> next;
	shared_ptr<vertex> prev;

public: vertex() {
	position = vector2d();
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
public: vertex(vector2d _position, int _id = -1) {
	position.x = _position.x;
	position.y = _position.y;
	id = _id;
	next = nullptr;
	prev = nullptr;
}
};

//---------------< POLYGON >--------------
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

//----------------< MESH >------------------
struct mesh {
	shared_ptr<vertex> vertexList;
	int vexLength;

	shared_ptr<polygon> polygonList;
	int pgonLength;

public: mesh() {//create default triangle mesh
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
public: mesh(double mass, int _vertecies) {
	shared_ptr<vertex> origin = make_shared<vertex>(vertex(0, 0, 0));
	shared_ptr<vertex> currentVex = origin;
	shared_ptr<polygon> currentPgon;
	vertexList = origin;
	vexLength = 1;
	int vertecies = _vertecies;

	double angle = (2 * PI) / double(vertecies);//angle needs to be in radians

	for (int i = 0; i < vertecies; i++) {
		//generate vertecies and add to vertexList
		//x1 = x0*cos() - y0*sin()
		//y1 = x0*sin() + y0*cos()
		//create vertex by angle * i starting at vector2d (radius, 0)
		vector2d next = vector2d(mass * cos(angle * i), mass * sin(angle * i));//coordinant of next vertex
		shared_ptr<vertex> vert = make_shared<vertex>(vertex(next, i + 1));

		//add to vertex chain and increase vexlength
		currentVex->next = vert;
		vert->prev = currentVex;
		vert->id = i + 1;
		vexLength++;
		currentVex = vert;

		//make polygons
		if (vexLength == 3) {//first polygon
			polygonList = make_shared<polygon>(origin, currentVex->prev, currentVex, 1);
			pgonLength = 1;
			currentPgon = polygonList;
		}
		else if (vexLength > 3 && i < vertecies) {//middle polygons
			shared_ptr<polygon> pgon = make_shared<polygon>(origin, currentPgon->c, currentVex, pgonLength + 1);//origin, last vertex of previous pgon, most recent vertex
			currentPgon->next = pgon;
			pgon->prev = currentPgon;
			pgonLength++;
			currentPgon = pgon;

		}
		//if near end, create final polygon
		if (vexLength == vertecies + 1) {//final polygon
		//origin, most recent vertex, first polygon.b
			shared_ptr<polygon> pgon = make_shared<polygon>(origin, currentVex, polygonList->b, pgonLength + 1);//origin, last vertex of previous pgon, most recent vertex
			currentPgon->next = pgon;
			pgon->prev = currentPgon;
			pgonLength++;
			currentPgon = pgon;
		}

	}
}

public: mesh(shared_ptr<vertex> _vertexList, shared_ptr<polygon> _polygonList, int _vexLength, int _pgonLength) {//create from existing lists
	vertexList = _vertexList;
	polygonList = _polygonList;
	vexLength = _vexLength;
	pgonLength = _pgonLength;
}
	  
};



//-------------------< UTILITY CLASS >---------------------------
//dev console and debug utilities
class Utility {
public:

	bool polygon_debug_draw;
	bool velocity_debug_draw;
	bool accelleration_debug_draw;
	int body_debug_draw;
	bool show_user_interface;
	bool virtual_list_changed;
	GAME_STATE game_state;
	/*game_state codes
	0: edit mode
	1: paused simulation
	2: active simulation
	*/

public:Utility() {
	//game data

	polygon_debug_draw = false;
	velocity_debug_draw = false;
	accelleration_debug_draw = false;
	body_debug_draw = 0;
	show_user_interface = true;
	virtual_list_changed = false;
	game_state = GAME_STATE::edit;
	//default constructor
}
public:mesh generateBox(double _radius) {
	mesh m;
	shared_ptr<vertex> origin = make_shared<vertex>(0, 0, 0);
	shared_ptr<vertex> currentVex = origin;
	shared_ptr<vertex> nextVex;
	shared_ptr<polygon> currentPgon;
	shared_ptr<polygon> nextPgon;
	m.vertexList = origin;
	m.vexLength = 1;

	//top left
	nextVex = make_shared<vertex>(-_radius, -_radius, 1);
	currentVex->next = nextVex;
	currentVex = nextVex;
	//top right
	nextVex = make_shared<vertex>(_radius, -_radius, 2);
	currentVex->next = nextVex;
	currentVex = nextVex;
	//bottom right
	nextVex = make_shared<vertex>(_radius, _radius, 3);
	currentVex->next = nextVex;
	currentVex = nextVex;
	//bottom left
	nextVex = make_shared<vertex>(-_radius, _radius, 4);
	currentVex->next = nextVex;
	currentVex = nextVex;
	m.vexLength = 5;

	//polygons
	currentPgon = make_shared<polygon>(m.vertexList, m.vertexList->next, m.vertexList->next->next, 0);
	m.polygonList = currentPgon;
	nextPgon = make_shared<polygon>(m.vertexList, m.vertexList->next->next, m.vertexList->next->next->next, 1);
	currentPgon->next = nextPgon;
	currentPgon = nextPgon;
	nextPgon = make_shared<polygon>(m.vertexList, m.vertexList->next->next->next, m.vertexList->next->next->next->next, 2);
	currentPgon->next = nextPgon;
	currentPgon = nextPgon;
	nextPgon = make_shared<polygon>(m.vertexList, m.vertexList->next->next->next->next, m.vertexList->next, 3);
	currentPgon->next = nextPgon;
	m.pgonLength = 4;

	return m;
}
public:mesh generateCircle(double mass, int _vertecies) {
	mesh m;
	shared_ptr<vertex> origin = make_shared<vertex>(vertex(0, 0, 0));
	shared_ptr<vertex> currentVex = origin;
	shared_ptr<polygon> currentPgon;
	m.vertexList = origin;
	m.vexLength = 1;
	int vertecies = _vertecies;

	double angle = (2 * PI) / double(vertecies);//angle needs to be in radians

	for (int i = 0; i < vertecies; i++) {
		//generate vertecies and add to vertexList
		//x1 = x0*cos() - y0*sin()
		//y1 = x0*sin() + y0*cos()
		//create vertex by angle * i starting at vector2d (radius, 0)
		vector2d next = vector2d(mass * cos(angle * i), mass * sin(angle * i));//coordinant of next vertex
		shared_ptr<vertex> vert = make_shared<vertex>(vertex(next, i + 1));

		//add to vertex chain and increase vexlength
		currentVex->next = vert;
		vert->prev = currentVex;
		vert->id = i + 1;
		m.vexLength++;
		currentVex = vert;

		//make polygons
		if (m.vexLength == 3) {//first polygon
			m.polygonList = make_shared<polygon>(origin, currentVex->prev, currentVex, 1);
			m.pgonLength = 1;
			currentPgon = m.polygonList;
		}
		else if (m.vexLength > 3 && i < vertecies) {//middle polygons
			shared_ptr<polygon> pgon = make_shared<polygon>(origin, currentPgon->c, currentVex, m.pgonLength + 1);//origin, last vertex of previous pgon, most recent vertex
			currentPgon->next = pgon;
			pgon->prev = currentPgon;
			m.pgonLength++;
			currentPgon = pgon;

		}
		//if near end, create final polygon
		if (m.vexLength == vertecies + 1) {//final polygon
		//origin, most recent vertex, first polygon.b
			shared_ptr<polygon> pgon = make_shared<polygon>(origin, currentVex, m.polygonList->b, m.pgonLength + 1);//origin, last vertex of previous pgon, most recent vertex
			currentPgon->next = pgon;
			pgon->prev = currentPgon;
			m.pgonLength++;
			currentPgon = pgon;
		}

	}
}
	  double getStableSpeed(double _mass, double _radius, double _gravity) {
		  return sqrt((_gravity * _mass) / _radius);
	  }
};
