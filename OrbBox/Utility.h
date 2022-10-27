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

	template <typename T> void copyBin(shared_ptr<bin<T>> _bin) {
		T newBin = *_bin->item;
		add(make_shared<T>(newBin));
	}

	template <typename T> void deepCopyBin(shared_ptr<bin<T>> _bin) {
		int temp = _bin->itemID;
		T newItem = *_bin->item;

		add(make_shared<T>(newItem));
		tail->itemID = temp;
		
		//ensures each ID is always unique in the list if adding more after copying
		if (uniqueCount < temp) {
			uniqueCount = temp + 1;
		}
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
	template <typename T> shared_ptr<T> getByBinID(T type, int _id) {
		//return bin->item if bin->itemID == _id
		//need a way of determining type without passing as arg
		shared_ptr<bin<T>> currentBin = head;
		while (currentBin != nullptr) {
			if (currentBin->itemID == _id) {
				return currentBin->item;
			}
			currentBin = currentBin->next;
		}
		return nullptr;
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

//queue
template <typename T> struct queue {
public:
	int currentLength;
	int maxLength;
	list<T> itemList;

public:
	template <typename T> queue(int _len = 5) {
		currentLength = 0;
		maxLength = _len;
		itemList = list<T>();
	}

	//add to queue, if length too large remove tail
	template <typename T> void add(shared_ptr<T> _item) {
		currentLength++;
		if (currentLength > maxLength) {
			currentLength--;
			itemList.removeByItem(itemList.tail->item);
		}
		itemList.add(_item);
	}

	//return tail item
	template <typename T> shared_ptr<T> remove() {
		currentLength--;
		shared_ptr<T> returnItem = itemList.tail->item;
		itemList.removeByItem(returnItem);
		return returnItem;
	}

	//add to queue and always return tail
	template <typename T> shared_ptr<T> swap(shared_ptr<T> _item) {
		shared_ptr<T> returnItem = itemList.tail->item;
		itemList.removeByItem(returnItem);
		itemList.add(_item);
		return returnItem;
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

	vertex() {
		position = vector2d();
	}

	vertex(double _a, double _b) {
		position.x = _a;
		position.y = _b;
	}
	vertex(vector2d _position) {
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
			currentPolygon->item->a = vertexList.getByBinID(vertex(), currentPolygon->item->aID);
			currentPolygon->item->b = vertexList.getByBinID(vertex(), currentPolygon->item->bID);
			currentPolygon->item->c = vertexList.getByBinID(vertex(), currentPolygon->item->cID);

			if (currentPolygon->item->a == nullptr || currentPolygon->item->b == nullptr || currentPolygon->item->c == nullptr) {
				currentPolygon->itemID = -1;
				//if something went wrong mark polygon as problem
			}
			currentPolygon = currentPolygon->next;
		}
	}
	void copyMesh(shared_ptr<mesh> _mesh) {
		buildMesh(_mesh->vertexList, _mesh->polygonList);
	}
	void copyMesh(mesh _mesh) {
		buildMesh(_mesh.vertexList, _mesh.polygonList);
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
		//create vertex by angle * i starting at vector2d (radius, 0)
		list<vertex> vertexList;
		list<polygon> polygonList;
		vector2d next;
		double angle = (2 * PI) / double(vertecies);//angle of each new vertex in radians

		//create an origin
		int i = 0;
		shared_ptr<vertex> origin = make_shared<vertex>(0, 0);
		vertexList.add(origin);

		//fill until at final vertex
		for (int i = 0; i < vertecies; i++) {
			next = vector2d(radius * cos(angle * i), radius * sin(angle * i));//coordinant of next vertex
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
