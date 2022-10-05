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
	shared_ptr<bin<T>> head;
	shared_ptr<bin<T>> tail;


	list<T>() {
	head = nullptr;
	tail = nullptr;
}
	void clear() {
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
	void removeByItem(shared_ptr<T> _item) {
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
	list<vertex> vertexList;

	list<polygon> polygonList;

 mesh(double radius = 6) {//create default box mesh
	//create all vertecies
	 shared_ptr<vertex> va = make_shared<vertex>(-radius/2, -radius/2, 0);
	 shared_ptr<vertex> vb = make_shared<vertex>(radius/2, -radius/2, 1);
	 shared_ptr<vertex> vc = make_shared<vertex>(radius/2, radius/2, 2);
	 shared_ptr<vertex> vd = make_shared<vertex>(-radius/2, radius/2, 3);
	 //add to list
	 vertexList.add(va);
	 vertexList.add(vb);
	 vertexList.add(vc);
	 vertexList.add(vd);

	//make polygons from vertecies	
	 shared_ptr<polygon> pa = make_shared<polygon>(va, vb, vc);
	 shared_ptr<polygon> pb = make_shared<polygon>(vb, vd, va);
	 //add to list
	 polygonList.add(pa);
	 polygonList.add(pb);//hehe, lead
}

 mesh(list<vertex> _vertList, list<polygon> _polyList) {//create from existing lists
	 vertexList = _vertList;
	 polygonList = _polyList;
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
	
	mesh generateCircle(double radius = 6, int vertecies = 6) {
		//x1 = x0*cos() - y0*sin()
		//y1 = x0*sin() + y0*cos()
		//create vertex by angle * i starting at vector2d (radius, 0)
		list<vertex> vertexList;
		list<polygon> polygonList;
		vector2d next;
		double angle = (2 * PI) / double(vertecies);//angle of each new vertex in radians

		//create an origin
		int i = 0;
		shared_ptr<vertex> origin = make_shared<vertex>(0, 0, 0);

		//fill until at final vertex
		for (int i = 0; i < vertecies; i++) {
			next = vector2d(radius * cos(angle * i), radius * sin(angle * i));//coordinant of next vertex
			vertexList.add(make_shared<vertex>(next, i + 1));

			//creates first -> almost last polygons
			if (i >= 1) {
				polygonList.add(make_shared<polygon>(vertexList.head->item, vertexList.tail->prev->item, vertexList.tail, i - 1));
			}
		}

		//finally create last polygon to link front and back
		polygonList.add(make_shared<polygon>(vertexList.head->item, vertexList.head->prev->item, vertexList.tail, i - 1));

		//aaaaand return
		return mesh(vertexList, polygonList);
	}
	double getStableSpeed(double _mass, double _radius, double _gravity) {
	 return sqrt((_gravity * _mass) / _radius);
	}
	template <typename T> list<T> copyList(){
	 //return deepcopy of a list
	 //impliment later
	}
};
