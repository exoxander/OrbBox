#pragma once
#include <fstream>
using std::fstream;
using std::shared_ptr;
using std::make_shared;
using std::string;

#define PI 3.14159265//used in trig functions
//https://www.cprogramming.com/tutorial/function-pointers.html

//general list to replace the horrendous random implimentations
//"bin" == each link in the list
template <typename T> struct bin {
	shared_ptr<T> item;
	int item_id;
	shared_ptr<bin> next;
	shared_ptr<bin> prev;

	bin<T>(shared_ptr<T> _item = nullptr, int _id = -1) {
		item = _item;
		next = nullptr;
		prev = nullptr;
		item_id = _id;
	}
};

//the list itself and its functions
template <typename T> struct list {
	shared_ptr<bin<T>> head;
	shared_ptr<bin<T>> tail;
	int uniqueCount;

	list<T>() {
		head = nullptr;
		tail = nullptr;
		uniqueCount = 1;
	}
	void clear() {
		head = nullptr;
		tail = nullptr;
		uniqueCount = 1;
	}
	//adds shared pointer of the specified type to the list

	template <typename T> void add(shared_ptr<T> _item) {
		shared_ptr<bin<T>> currentItem = head;
		shared_ptr<bin<T>> nextItem = make_shared<bin<T>>(_item, uniqueCount);
		uniqueCount++;

		if (tail == nullptr) {
			//if the list is empty
			head = nextItem;
			tail = nextItem;
		}
		else {
			//if list has > 1 entries
			tail->next = nextItem;
			nextItem->prev = tail;
			tail = nextItem;
		}

	}

	//removes an item of the matching shared pointer from the list (if it exists)
	template <typename T> void removeByItem(shared_ptr<T> _item) {
		shared_ptr<bin<T>> currentBin = head;
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
					else if (currentBin == tail) {
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
	template <typename T> void removeByBinID(int _id) {
		//loop through until bin->ID == _id and call remove() on bin->item
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->item_id == _id) {
				remove(currentBin->item);
				break;
			}
			currentBin = currentBin->next;
		}

	}
	template <typename T> shared_ptr<T> getByBinID(int _id) {
		//return pointer to bin->item if bin->id == _id
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->item_id == _id) {
				return currentBin->item;
			}
		}
		return nullptr;
	}

	template <typename T> int getBinID(shared_ptr<T> _item) {
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->item == _item) {
				return currentBin->item_id;
			}
			currentBin = currentBin->next;
		}
		return -1;
	}
};

//queue
template <typename T> struct queue {
	int currentlength;
	int maxLength;
	list<T> itemList;

	template <typename T> queue(int _len = 5) {
		currentlength = 0;
		maxLength = _len;
		itemList = list<T>();
	}

	//add to queue, if length too large remove tail
	template <typename T> add(shared_ptr<T> _item) {
		currentLength++;
		if (currentLength > maxlength) {
			currentLength--;
			itemList.removeByItem(itemList.tail->item);
		}
		itemList.add(_item);
	}

	//return tail item
	template <typename T> shared_ptr<T> remove() {
		length--;
		shared_ptr<T> returnItem = itemList.tail->item;
		itemList.removeByItem(returnItem);
		return returnitem;
	}

	//add to queue and always return tail
	template <typename T> shared_pr<T> swap(shared_ptr<T> _item) {
		shared_ptr<T> returnItem = itemList.tail->item;
		itemList.removeByItem(returnItem);
		itemList.add(_item);
		return returnitem;
	}
};

//game states
enum struct GAME_STATE {
	edit = 0,
	pause,
	play

};
enum struct MESH_DRAW_MODE {
	wireframe,
	triangle,
	solid,
	texture
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
	void add(vector2i _value) {
		x += _value.x;
		y += _value.y;
	}
	void multiply(int _value) {
		x *= _value;
		y *= _value;
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
	void normalize() {
		double magnitude = getMagnitude();
		if (magnitude != 0) {
			x /= magnitude;
			y /= magnitude;
		}
	}
	vector2d returnNormalized() {
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
	}

	vertex(double _a, double _b, int _id = -1) {
		position.x = _a;
		position.y = _b;
		id = _id;
	}
	vertex(vector2d _position, int _id = -1) {
		position = _position;
		id = _id;
	}
};

//---------------< POLYGON >--------------
struct polygon {

	int id;
	shared_ptr<vertex> a;
	shared_ptr<vertex> b;
	shared_ptr<vertex> c;

	polygon() {
		a = nullptr;
		b = nullptr;
		c = nullptr;
		id = -1;
	}
	polygon(shared_ptr<vertex> _a, shared_ptr<vertex> _b, shared_ptr<vertex> _c, int _id = -1) {
		a = _a;
		b = _b;
		c = _c;
		id = _id;
	}
};

struct polygonLink {
	int polygonID;
	int aID;
	int bID;
	int cID;

	polygonLink(int _p, int _a, int _b, int _c) {
		polygonID = _p;
		aID = _a;
		bID = _b;
		cID = _c;
	}

	polygonLink(shared_ptr<bin<polygon>> _polygonBin, list<vertex> _vertexList) {
		polygonID = _polygonBin->item_id;
		aID = _vertexList.getBinID(_polygonBin->item->a);
		bID = _vertexList.getBinID(_polygonBin->item->b);
		cID = _vertexList.getBinID(_polygonBin->item->c);
	}
};

//----------------< MESH >------------------
struct mesh {
	list<vertex> vertexList;
	list<polygon> polygonList;
	list<polygonLink> polygonLinks;

	mesh(double radius = 6) {//create default box mesh
	   //create all vertecies
		shared_ptr<vertex> va = make_shared<vertex>(-radius / 2, -radius / 2, 0);
		shared_ptr<vertex> vb = make_shared<vertex>(radius / 2, -radius / 2, 1);
		shared_ptr<vertex> vc = make_shared<vertex>(radius / 2, radius / 2, 2);
		shared_ptr<vertex> vd = make_shared<vertex>(-radius / 2, radius / 2, 3);
		//add to list
		vertexList.add(va);
		vertexList.add(vb);
		vertexList.add(vc);
		vertexList.add(vd);

		//make polygons from vertecies
		shared_ptr<polygon> pa = make_shared<polygon>(va, vb, vc);
		shared_ptr<polygon> pb = make_shared<polygon>(vc, vd, va);
		//add to list
		polygonList.add(pa);
		polygonList.add(pb);//hehe, lead
	}

	mesh(list<vertex> _vertList, list<polygon> _polyList) {//create from existing lists
		vertexList = _vertList;
		polygonList = _polyList;
	}

	void rebuildMesh(list<polygonLink> _links) {

	}

};



//-------------------< UTILITY CLASS >---------------------------
//dev console and debug utilities
class Utility {

public:
	MESH_DRAW_MODE draw_mode;
	bool draw_velocity;
	bool draw_acceleration;
	int draw_body_info;
	bool show_user_interface;
	bool virtual_list_changed;
	GAME_STATE game_state;

public:
	Utility() {
		draw_mode = MESH_DRAW_MODE::solid;
		draw_velocity = false;
		draw_acceleration = false;
		draw_body_info = 0;
		show_user_interface = true;
		virtual_list_changed = false;
		game_state = GAME_STATE::edit;
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
		vertexList.add(origin);

		//fill until at final vertex
		for (int i = 0; i < vertecies; i++) {
			next = vector2d(radius * cos(angle * i), radius * sin(angle * i));//coordinant of next vertex
			vertexList.add(make_shared<vertex>(next, i + 1));

			//creates first -> almost last polygons
			if (i >= 1) {
				shared_ptr<polygon> p = make_shared<polygon>(vertexList.head->item, vertexList.tail->prev->item, vertexList.tail->item, i - 1);//something here is causing a crash
				polygonList.add(p);
			}
		}

		//finally create last polygon to link front and back
		shared_ptr<polygon> newPolygon = make_shared<polygon>(vertexList.head->next->item, vertexList.head->item, vertexList.tail->item, i - 1);
		polygonList.add(newPolygon);

		//aaaaand return
		return mesh(vertexList, polygonList);
	}
	double getStableSpeed(double _mass, double _radius, double _gravity) {
		return sqrt((_gravity * _mass) / _radius);
	}
};
