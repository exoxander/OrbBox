#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "PhysicsBody.h"
#include "Utility.h"
#include "Camera.h"

//const float TickRate = 16;
//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki

class Engine : public olc::PixelGameEngine {

public:
    Engine() {
        sAppName = "Orb_Box";
    }

private:
    int vertexDrawScale = 4;
    olc::Pixel colorList[4] = { olc::WHITE, olc::BLUE, olc::GREEN, olc::RED };
    Utility u;
    Camera viewport;
    bodyList bodies;


public:
    bool OnUserCreate() override {
        //initialize camera
        viewport.location = coord(double(ScreenWidth()/2), double(ScreenHeight())/2);
        PhysicsBody p = PhysicsBody();
        p.setMesh(u.generateCircle(30));
        p.position.add(coord(200,0));
        bodies.addBody(p);
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //fill screen with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        drawMesh(bodies,true);
        //finished

        return true;
    }

    void drawMesh(bodyList _list, bool filled = false) {
        shared_ptr<body> current = _list.head;
            do {
                shared_ptr<PhysicsBody> p = current->item;
                mesh m = current->item->getMesh();
                if (filled) {

                    //loop through all polygons and draw to screen
                    shared_ptr<polygon> current = m.polygonList;
                    for (int i = 0; i < m.pgonLength; i++) {
                        coord a = current->a->position;
                        coord b = current->b->position;
                        coord c = current->c->position;
                        coord parent = p->position;

                        //make color result of % between polygon id and color list
                        olc::Pixel polygonColor = colorList[current->id % 4];
                        FillTriangle(
                            int((viewport.translate(parent, a)).x), int((viewport.translate(parent, a)).y),//A
                            int((viewport.translate(parent, b)).x), int((viewport.translate(parent, b)).y),//B
                            int((viewport.translate(parent, c)).x), int((viewport.translate(parent, c)).y),//C
                            polygonColor);
                        //int((viewport.translate(parent, a)).x), int((viewport.translate(parent, b)).y)
                        if (current->next != nullptr) {
                            current = current->next;
                        }
                    }
                }
                else {
                    //draw circles on vertecies
                    shared_ptr<vertex> current = p->getMesh().vertexList;
                    for (int i = 0; i < p->getMesh().vexLength; i++) {
                        if (current != nullptr) {
                            //make circle
                            olc::Pixel vertexColor = colorList[current->id % 4];
                            FillCircle(
                                int((viewport.translate(p->position, current->position)).x),
                                int((viewport.translate(p->position, current->position)).y),
                                vertexDrawScale, vertexColor);
                            current = current->next;
                        }
                        else {
                            //throw an error or something idk
                            std::cout << "vexlength exceeded actual" << std::endl;
                        }
                    }
                }
                if (current->next) {
                    current = current->next;
                }
            } while (current->next);
    }
        

};

int main()
{
    Engine window;
    if (window.Construct(1024, 640, 1, 1)) {
        window.Start();
    }
    return 0;
}
