#pragma once
using std::shared_ptr;
using std::make_shared;
using std::string;

#define PI 3.14159265//used in trig functions

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
	  //------------< GENERATE CIRCLE FUNCTION >------------------
public:mesh generateCircle(int _radius = 10, int _vertecies = 12) {//generates a circle with the desired radius and vertex count
	mesh m;
	//origin vertex
	shared_ptr<vertex> origin = make_shared<vertex>(vertex(0, 0, 0));
	shared_ptr<vertex> currentVex = origin;
	shared_ptr<polygon> currentPgon;
	m.vertexList = origin;
	m.vexLength = 1;

	double angle = (2 * PI) / double(_vertecies);//angle needs to be in radians

	for (int i = 0; i < _vertecies; i++) {
		//generate vertecies and add to vertexList
		//x1 = x0*cos() - y0*sin()
		//y1 = x0*sin() + y0*cos()
		//create vertex by angle * i starting at vector2d (radius, 0)
		vector2d next = vector2d(_radius * cos(angle * i), _radius * sin(angle * i));//coordinant of next vertex
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
		else if (m.vexLength > 3 && i < _vertecies) {//middle polygons
			shared_ptr<polygon> pgon = make_shared<polygon>(origin, currentPgon->c, currentVex, m.pgonLength + 1);//origin, last vertex of previous pgon, most recent vertex
			currentPgon->next = pgon;
			pgon->prev = currentPgon;
			m.pgonLength++;
			currentPgon = pgon;

		}
		//if near end, create final polygon
		if (m.vexLength == _vertecies + 1) {//final polygon
		//origin, most recent vertex, first polygon.b
			shared_ptr<polygon> pgon = make_shared<polygon>(origin, currentVex, m.polygonList->b, m.pgonLength + 1);//origin, last vertex of previous pgon, most recent vertex
			currentPgon->next = pgon;
			pgon->prev = currentPgon;
			m.pgonLength++;
			currentPgon = pgon;
		}

	}
	return m;
}
};

//-----------------< BUTTON >-----------------
struct button {
	bool active;
	vector2d location;
	vector2d dimensions;
	string info;
	//shared_ptr<action> activate;
};



//-------------------< UTILITY CLASS >---------------------------
//dev console and debug utilities
class Utility {
public:
	//shared_ptr<bodyList> startingBodies;//virtual bodies at start / load / save
	//shared_ptr<bodyList> existingBodies;//all bodies in the sim currently

	bool polygon_debug_draw;
	bool velocity_debug_draw;
	bool accelleration_debug_draw;
	bool show_user_interface;
	int game_state;
	/*game_state codes
	0: edit mode
	1: paused simulation
	2: active simulation
	*/

public:Utility() {
	//game data
	//startingBodies = nullptr;
	//existingBodies = nullptr;

	polygon_debug_draw = false;
	velocity_debug_draw = false;
	accelleration_debug_draw = false;
	show_user_interface = true;
	game_state = 0;
	//default constructor
}
public:void markdownReader() {
	
}
public: void markdownWriter() {

}
};
/* SIMPLE MARKDOWN FORMAT SMF
	<name>			| the start of a field
	</name>			| the end of the most recent field of that type
	{name:data}		| the name and value of a piece of data in a field
	//text			| a comment

	//EXAMPLE
	<physicsbody>
	{position_x:123}
	{position_y:456}
	{mass:500}
	{id:42}
	</physicsbody>
	*/