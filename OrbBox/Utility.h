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
	int itemID;
	shared_ptr<bin> next;
	shared_ptr<bin> prev;

	bin<T>(shared_ptr<T> _item = nullptr, int _id = -1) {
		item = _item;
		next = nullptr;
		prev = nullptr;
		itemID = _id;
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

	template <typename T> void add(shared_ptr<T> _item, int overrideID = -1) {
		shared_ptr<bin<T>> currentItem = head;
		int newID = uniqueCount;
		if (overrideID > -1) {
			newID = overrideID;
			if (newID > uniqueCount) {
				uniqueCount = newID;
			}
		}
		shared_ptr<bin<T>> nextItem = make_shared<bin<T>>(_item, newID);
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

	template <typename T> void add(T _item, int overrideID = -1) {
		add(make_shared<T>(_item), overrideID);
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

	template <typename T> void copyBin(shared_ptr<bin<T>> _bin) {
		T newBin = *_bin->item;
		add(make_shared<T>(newBin));
	}

	template <typename T> void deepCopyBin(shared_ptr<bin<T>> _bin) {
		//int temp = _bin->itemID;
		T newItem = *_bin->item;

		add(make_shared<T>(newItem), _bin->itemID);
	}

	template <typename T> list<T> deepCopy(T type) {
		list<T> newList;
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			newList.deepCopyBin(currentBin);
			currentBin = currentBin->next;
		}
		return newList;
	}

	template <typename T> void removeByBinID(int _id) {
		//loop through until bin->itemID == _id and call remove() on bin->item
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->itemID == _id) {
				removeByItem(currentBin->item);
				break;
			}
			currentBin = currentBin->next;
		}

	}
	template <typename T> shared_ptr<bin<T>> getBinByID(T type, int _id, int index = 1) {
		//return bin->item if bin->itemID == _id
		//need a way of determining type without passing as arg
		shared_ptr<bin<T>> currentBin = head;
		shared_ptr<bin<T>> last = nullptr;
		int count = 0;
		while (currentBin != nullptr && count < index) {
			if (currentBin->itemID == _id) {
				last = currentBin;
				count++;
			}
			currentBin = currentBin->next;
		}
		return last;
	}

	//same as above but works if multiple bins have same value and counts them as the index
	//if index > number in list, returns last one found
	template <typename T> shared_ptr<T> getItemByID(T type, int _id, int index = 1) {
		//return bin->item if bin->itemID == _id
		shared_ptr<bin<T>> currentBin = head;
		shared_ptr<bin<T>> found = nullptr;
		found = getBinByID(type, _id, index);
		if (found != nullptr) {
			return found->item;
		}
		else {
			return nullptr;
		}
	}

	template <typename T> int getBinID(shared_ptr<T> _item) {
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->item == _item) {
				return currentBin->itemID;
			}
			currentBin = currentBin->next;
		}
		return -1;
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
//===================< iVector >====================
struct iVector {
	int x;
	int y;

	iVector(int _x = 0, int _y = 0) {
		x = _x;
		y = _y;
	}
	void add(int _x, int _y) {
		x += _x;
		y += _y;
	}
	void add(iVector _value) {
		x += _value.x;
		y += _value.y;
	}
	void subtract(iVector _value) {
		x -= _value.x;
		y -= _value.y;
	}
	void multiply(int _value) {
		x *= _value;
		y *= _value;
	}
	void divide(int _value) {
		x /= _value;
		y /= _value;
	}
	
};
//===================-< dVector >-==================
struct dVector {

	double x;
	double y;
public:
	dVector(double _x = 0, double _y = 0) {
		set(_x, _y);
	}
	iVector toiVector() {
		return iVector(int(x), int(y));
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
	void add(dVector _value) {
		x += _value.x;
		y += _value.y;
	}
	void subtract(dVector _value) {
		x -= _value.x;
		y -= _value.y;
	}
	void multiply(dVector _value) {
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
	dVector returnNormalized() {
		double magnitude = getMagnitude();
		if (magnitude != 0) {
			return dVector(x / magnitude, y / magnitude);
		}
		else {
			return dVector();
		}
	}
	dVector returnMultiply(double _value) {
		return dVector(x * _value, y * _value);
	}
	void makedVector(iVector _input) {
		x = double(_input.x);
		y = double(_input.y);
	}
};


//---------------< VERTEX > --------------
struct vertex {

	dVector position;

	vertex() {
		position = dVector();
	}

	vertex(double _a, double _b) {
		position.x = _a;
		position.y = _b;
	}
	vertex(dVector _position) {
		position = _position;
	}
};

//---------------< POLYGON >--------------
struct polygon {
	shared_ptr<vertex> a;
	shared_ptr<vertex> b;
	shared_ptr<vertex> c;
	int aID;
	int bID;
	int cID;

	polygon() {
		a = nullptr;
		b = nullptr;
		c = nullptr;
		aID = -1;
		bID = -2;
		cID = -3;
	}
	polygon(shared_ptr<vertex> _a, shared_ptr<vertex> _b, shared_ptr<vertex> _c) {
		a = _a;
		b = _b;
		c = _c;
		aID = -1;
		bID = -2;
		cID = -3;
	}
	polygon(shared_ptr<bin<vertex>> _a, shared_ptr<bin<vertex>> _b, shared_ptr<bin<vertex>> _c) {
		a = _a->item;
		b = _b->item;
		c = _c->item;
		aID = _a->itemID;
		bID = _b->itemID;
		cID = _c->itemID;
	}
	polygon(int _a, int _b, int _c) {//only for use in reading from file
		a = nullptr;
		b = nullptr;
		c = nullptr;
		aID = _a;
		bID = _b;
		cID = _c;
	}
};



//----------------< MESH >------------------
struct mesh {
	list<vertex> vertexList;
	list<polygon> polygonList;

	mesh(double radius = 6) {//create default box mesh
	   //create all vertecies
		shared_ptr<vertex> va = make_shared<vertex>(-radius / 2, -radius / 2);
		shared_ptr<vertex> vb = make_shared<vertex>(radius / 2, -radius / 2);
		shared_ptr<vertex> vc = make_shared<vertex>(radius / 2, radius / 2);
		shared_ptr<vertex> vd = make_shared<vertex>(-radius / 2, radius / 2);
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

		setPolygonIDs();
	}

	mesh(list<vertex> _vertList, list<polygon> _polyList) {//create from existing lists
		vertexList = _vertList;
		polygonList = _polyList;
		setPolygonIDs();
	}
	void setPolygonIDs() {
		shared_ptr<bin<vertex>> currentVertex = vertexList.head;

		while (currentVertex != nullptr) {
			//for every vertex in the mesh find all polygons that have it and set the corrisponding ID
			shared_ptr<bin<polygon>> currentPolygon = polygonList.head;
			while (currentPolygon != nullptr) {
				if (currentVertex->item == currentPolygon->item->a) {//a
					currentPolygon->item->aID = currentVertex->itemID;
				}
				else if (currentVertex->item == currentPolygon->item->b) {//b
					currentPolygon->item->bID = currentVertex->itemID;
				}
				else if (currentVertex->item == currentPolygon->item->c) {//c
					currentPolygon->item->cID = currentVertex->itemID;
				}
				//shared_ptr<bin<polygon>> currentPolygon = polygonList.head;
				currentPolygon = currentPolygon->next;
			}
			currentVertex = currentVertex->next;			
		}
	}

	//build mesh from vertex bins and polygon bins using their IDs
	void buildMesh(list<vertex> _vertexList, list<polygon> _polygonList) {
		vertexList = _vertexList.deepCopy(vertex());
		polygonList = _polygonList.deepCopy(polygon());
		//run through polygons and use getByBinID to set all the pointers
		shared_ptr<bin<polygon>> currentPolygon = polygonList.head;
		while (currentPolygon != nullptr) {
			currentPolygon->item->a = vertexList.getItemByID(vertex(), currentPolygon->item->aID);
			currentPolygon->item->b = vertexList.getItemByID(vertex(), currentPolygon->item->bID);
			currentPolygon->item->c = vertexList.getItemByID(vertex(), currentPolygon->item->cID);

			if (currentPolygon->item->a == nullptr || currentPolygon->item->b == nullptr || currentPolygon->item->c == nullptr) {
				currentPolygon->itemID = -1;
				//if something went wrong mark polygon as problem
			}
			currentPolygon = currentPolygon->next;
		}
	}
	void copy(shared_ptr<mesh> _mesh) {
		buildMesh(_mesh->vertexList, _mesh->polygonList);
	}
	mesh copy() {
		mesh newMesh = mesh();
		newMesh.buildMesh(vertexList, polygonList);
		return newMesh;
	}
	/*
	void rebuildMesh() {

	}
	*/
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
	void iterateDrawMode() {
		if (draw_mode != MESH_DRAW_MODE::texture) {
			draw_mode = (MESH_DRAW_MODE)(((int)draw_mode) + 1);
		}
		else {
			draw_mode = MESH_DRAW_MODE::wireframe;
		}
	}

	mesh generateCircle(double radius = 6, int vertecies = 6) {
		//x1 = x0*cos() - y0*sin()
		//y1 = x0*sin() + y0*cos()
		//create vertex by angle * i starting at dVector (radius, 0)
		list<vertex> vertexList;
		list<polygon> polygonList;
		dVector next;
		double angle = (2 * PI) / double(vertecies);//angle of each new vertex in radians

		//create an origin
		int i = 0;
		shared_ptr<vertex> origin = make_shared<vertex>(0, 0);
		vertexList.add(origin);

		//fill until at final vertex
		for (int i = 0; i < vertecies; i++) {
			next = dVector(radius * cos(angle * i), radius * sin(angle * i));//coordinant of next vertex
			vertexList.add(make_shared<vertex>(next));

			//creates first -> almost last polygons
			if (i >= 1) {
				shared_ptr<polygon> p = make_shared<polygon>(vertexList.head->item, vertexList.tail->prev->item, vertexList.tail->item);//something here is causing a crash
				polygonList.add(p);
			}
		}

		//finally create last polygon to link front and back
		shared_ptr<polygon> newPolygon = make_shared<polygon>(vertexList.head->next->item, vertexList.head->item, vertexList.tail->item);
		polygonList.add(newPolygon);

		//aaaaand return
		return mesh(vertexList, polygonList);
	}
	double getStableSpeed(double _mass, double _radius, double _gravity) {
		return sqrt((_gravity * _mass) / _radius);
	}
};
