#pragma once
#include <fstream>
using std::fstream;
using std::shared_ptr;
using std::make_shared;
using std::string;

#define PI 3.14159265//used in trig functions

//general list to replace the horrendous random implimentations
//"bin" == each link in the list
template <typename T> struct bin {
	shared_ptr<T> item;
	int item_id;
	shared_ptr<bin> next;
	shared_ptr<bin> prev;
bin<T>(shared_ptr<T> _item = nullptr, shared_ptr<bin> _prev = nullptr, int _id = -1) {
	item = _item;
	next = nullptr;
	prev = _prev;
	id = _id;
}
};

//the list itself and its functions
template <typename T> struct list {
	shared_ptr<bin> head;
	shared_ptr<bin> tail;


	list<T>() {
	head = nullptr;
	tail = nullptr;
}

	//adds shared pointer of the specified type to the list
	void add(shared_ptr<T> _item) {
		shared_ptr<bin> currentItem = head;
		shared_ptr<bin> nextItem = make_shared<bin>(_item);

		if (tail == nullptr) {
			//if the list is empty
			head = _item;
			tail = _item;
		}
		else {
			//if list has > 1 entries
			tail->next = nextItem;
			nextItem->prev = tail;
			tail = nextItem;
		}

	}

	//removes an item of the matching shared pointer from the list (if it exists)
	void remove(shared_ptr<T> _item) {
		shared_ptr<bin> currentBin = head;
		if (tail == nullptr) {
			//list empty, warn user?
		}
		else {
			while (currentBin != nullptr) {
				if (currentBin->item == _item) {
					//if pointer matches
					if (currentBin == head && currentBin == tail) {
						//if only item in list
						head = nullptr;
						tail = nullptr;
					}
					else if (currentBin == head) {
						//if the head of the list
						head = currentBin->next;
					}
					else if (currentbin == tail) {
						//if the tail of the list
						tail = currentBin->prev;
					}
					else {
						//if in the middle somewhere
						currentBin->prev->next = currentBin->next;
						currentBin->next->prev = currentBin->prev;
					}

					break;
				}
				currentBin = currentBin->next;
			}
		}

	}
	void removeByBinID(int _id) {
		//loop through until bin->ID == _id and call remove() on bin->item
		shared_ptr<bin> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->id == _id) {
				remove(currentBin->item);
				break;
			}
			currentBin = currentBin->next;
		}

	}
	shared_ptr<T> getByBinID(int _id) {
		//return pointer to bin->item if bin->id == _id
		shared_ptr<bin> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->id == _id) {
				return currentBin->item;
			}
		}
		return nullptr;
	}
};

//game states
enum struct GAME_STATE {
	edit=0,
	pause,
	play

};
struct vector2i {
	int x;
	int y;

	vector2i(int _x = 0, int _y = 0) {
		x = _x;
		y = _y;
	}
	void add(int _x, int _y) {
		x += _x;
		y += _y;
	}
};
//===================-< vector2d >-==================
struct vector2d {

	double x;
	double y;
public:
 vector2d(double _x = 0, double _y = 0) {
	set(_x, _y);
}
 vector2i convertTo2i() {
	 return vector2i(int(x), int(y));
 }
 void set(double _x, double _y) {
	x = _x;
	y = _y;
}
	  //math on self
 void add(double _x, double _y) {
	x += _x;
	y += _y;
}
 void add(vector2d _value) {
	x += _value.x;
	y += _value.y;
}
void subtract(vector2d _value) {
	x -= _value.x;
	y -= _value.y;
}
void multiply(vector2d _value) {
	x *= _value.x;
	y *= _value.y;
}
void multiply(double _value) {
	x *= _value;
	y *= _value;
}
double getMagnitude() {
	return sqrt(x * x + y * y);
}
vector2d normalize() {
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

	vector2d position;
	int id;
	shared_ptr<vertex> next;
	shared_ptr<vertex> prev;

 vertex() {
	position = vector2d();
	id = -1;
	next = nullptr;
	prev = nullptr;
}

 vertex(double _a, double _b, int _id = -1) {
	position.x = _a;
	position.y = _b;
	id = _id;
	next = nullptr;
	prev = nullptr;
}
 vertex(vector2d _position, int _id = -1) {
	position.x = _position.x;
	position.y = _position.y;
	id = _id;
	next = nullptr;
	prev = nullptr;
}
};

//---------------< POLYGON >--------------
struct polygon {

	int id;
	shared_ptr<vertex> a;
	shared_ptr<vertex> b;
	shared_ptr<vertex> c;
	shared_ptr<polygon> next;
	shared_ptr<polygon> prev;

polygon() {
	a = nullptr;
	b = nullptr;
	c = nullptr;
	id = -1;
	next = nullptr;
	prev = nullptr;
}
 polygon(shared_ptr<vertex> _a, shared_ptr<vertex> _b, shared_ptr<vertex> _c, int _id = -1) {
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

 mesh(double size = 6) {//create default triangle mesh
	//initialize vertex list
	vertex a = vertex(0, size/2, 0);
	vertexList = make_shared<vertex>(a);

	vertex b = vertex(size/2, 0, 1);
	vertexList->next = make_shared<vertex>(b);
	b.prev = vertexList;

	vertex c = vertex(-size/2, 0, 2);
	c.prev = make_shared<vertex>(b);
	vertexList->next->next = make_shared<vertex>(c);

	vexLength = 3;

	//create polygon
	polygon p = polygon(vertexList, vertexList->next, vertexList->next->next, 0);
	polygonList = make_shared<polygon>(p);

	pgonLength = 1;
}
 mesh(double mass, int _vertecies) {
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

 mesh(shared_ptr<vertex> _vertexList, shared_ptr<polygon> _polygonList, int _vexLength, int _pgonLength) {//create from existing lists
	vertexList = _vertexList;
	polygonList = _polygonList;
	vexLength = _vexLength;
	pgonLength = _pgonLength;
}
	  
};



//-------------------< UTILITY CLASS >---------------------------
//dev console and debug utilities
class Utility {


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


	Utility() {
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
	mesh generateBox(double _radius) {
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
	mesh generateCircle(double mass, int _vertecies) {
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
	  template <typename T> list<T> copyList(){
		  //return deepcopy of a list
		  //impliment later
	  }
};
