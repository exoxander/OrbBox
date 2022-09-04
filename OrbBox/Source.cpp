#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "PhysicsBody.h"
#include "Utility.h"
#include "Camera.h"
#include "PhysicsSolver.h"

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
    shared_ptr<bodyList> bodies = make_shared<bodyList>();
    PhysicsSolver solver = PhysicsSolver(bodies);


public:
    bool OnUserCreate() override {
        //initialize camera
        viewport.location = coord(double(ScreenWidth() / 2), double(ScreenHeight()) / 2);
        PhysicsBody planet = PhysicsBody(coord(200, 0), coord(0, -5), 100);
        PhysicsBody star = PhysicsBody(coord(0, 0), coord(), 2000, 32);

        PhysicsBody mewn = PhysicsBody(coord(-200, -200), coord(-.5, .2), 30);
        PhysicsBody mewn_2 = PhysicsBody(coord(150, 70), coord(.3, 0), 30);
        PhysicsBody mewn_3 = PhysicsBody(coord(190, 10), coord(-.3, .5), 30);
        PhysicsBody mewn_4 = PhysicsBody(coord(250, -90), coord(0, -.5), 30);

        PhysicsBody planet_2 = PhysicsBody(coord(-200, -10), coord(-.5, .2), 256, 18);
        PhysicsBody planet_3 = PhysicsBody(coord(300, 10), coord(.1,-.2), 350, 24);


        planet.id = 1;
        star.id = 2;
        mewn.id = 3;
        planet_2.id = 4;
        planet_3.id = 5;
        mewn_2.id = 6;
        mewn_3.id = 7;
        mewn_4.id = 8;


        bodies->addBody(planet);
        bodies->addBody(star);
        bodies->addBody(mewn);
        bodies->addBody(planet_2);
        bodies->addBody(mewn_2);
        bodies->addBody(planet_3);
        bodies->addBody(mewn_3);
        bodies->addBody(mewn_4);
        return true;
    }

    //-------------------------< ON USER UPDATE >----------------------------
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //fill screen with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        //drawMesh(bodies.head,true);
        shared_ptr<body> currentBody = bodies->head;
        while (currentBody != nullptr) {
            drawMesh(currentBody,true);
            currentBody = currentBody->next;
        }
        //finished

        //do physics
        solver.step(.01);
        return true;
    }
    //-----------------------------< DRAWMESH >-----------------------------
    void drawMesh(shared_ptr<body> _entity, bool filled = false) {
        shared_ptr<body> current = _entity;
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
    }
        

};

int main()
{
    Engine window;
    if (window.Construct(1024, 640, 1, 1, false,true)) {//V-SYNCH ON
        window.Start();
    }
    return 0;
}
