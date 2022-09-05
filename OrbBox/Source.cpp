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
    olc::Pixel colorList[7] = { olc::WHITE, olc::BLUE, olc::GREEN, olc::RED, olc::YELLOW, olc::GREY, olc::Pixel(255,145,0) };
    int colorListLength = 7;
    Utility u;
    Camera viewport;
    shared_ptr<bodyList> bodies = make_shared<bodyList>();
    PhysicsSolver solver = PhysicsSolver(bodies);


public:
    bool OnUserCreate() override {
        //initialize camera
        viewport.location = coord(double(ScreenWidth() / 2), double(ScreenHeight()) / 2);
        viewport.zoom = 3;

        PhysicsBody planet = PhysicsBody(coord(70, -4), coord(0,.2), 300,24);
        PhysicsBody star = PhysicsBody(coord(-10, 14), coord(0.01,0), 5000, 32);

        PhysicsBody mewn = PhysicsBody(coord(-70, -20), coord(-.5, .2), 30);
        PhysicsBody mewn_2 = PhysicsBody(coord(80, 35), coord(.3, 0), 30);
        PhysicsBody mewn_3 = PhysicsBody(coord(-20, 100), coord(-.3, .5), 30);
        PhysicsBody mewn_4 = PhysicsBody(coord(40, -90), coord(0, -.5), 30);

        PhysicsBody planet_2 = PhysicsBody(coord(-100, -12), coord(-.5, -.8), 250, 18);
        PhysicsBody planet_3 = PhysicsBody(coord(80, 70), coord(.1,.8), 320, 24);


        planet.id = 1;
        star.id = 2;
        mewn.id = 3;
        planet_2.id = 4;
        planet_3.id = 5;
        mewn_2.id = 6;
        mewn_3.id = 7;
        mewn_4.id = 8;


        //bodies->addBody(mewn);
        //bodies->addBody(planet_2);
        //bodies->addBody(mewn_2);
        //bodies->addBody(planet_3);
        //bodies->addBody(mewn_3);
        //bodies->addBody(mewn_4);

        bodies->addBody(planet);
        bodies->addBody(star);
        return true;
    }

    //-------------------------< ON USER UPDATE >----------------------------
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //camera control
        if (GetKey(olc::Key::LEFT).bHeld) viewport.location.x += 1 * viewport.panSpeed;
        if (GetKey(olc::Key::RIGHT).bHeld) viewport.location.x -= 1 * viewport.panSpeed;
        if (GetKey(olc::Key::UP).bHeld) viewport.location.y += 1 * viewport.panSpeed;
        if (GetKey(olc::Key::DOWN).bHeld) viewport.location.y -= 1 * viewport.panSpeed;
        if (GetKey(olc::Key::NP_ADD).bHeld) viewport.zoom += .05;
        if (GetKey(olc::Key::NP_SUB).bHeld) viewport.zoom -= .05;
        //fill screen with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        //drawMesh(bodies.head,true);
        shared_ptr<body> currentBody = bodies->head;
        while (currentBody != nullptr) {
            drawMesh(currentBody,false, true, true);
            currentBody = currentBody->next;
        }
        //finished

        //do physics
        solver.step(.001);
        return true;
    }
    //-----------------------------< DRAWMESH >-----------------------------
    void drawMesh(shared_ptr<body> _entity, bool show_polygons = false, bool show_velocity = false, bool show_accelleration = false) {
        shared_ptr<body> currentBody = _entity;
        shared_ptr<PhysicsBody> p = currentBody->item;
        mesh m = currentBody->item->getMesh();   

        //loop through all polygons and draw to screen
        shared_ptr<polygon> currentPgon = m.polygonList;
        for (int i = 0; i < m.pgonLength; i++) {
            coord a = currentPgon->a->position;
            coord b = currentPgon->b->position;
            coord c = currentPgon->c->position;
            coord parent = p->position;
            olc::Pixel polygonColor;

            if (show_polygons) {
                //make color result of % between polygon id and color list
                 polygonColor = colorList[currentPgon->id % colorListLength];
            }
            else {
                polygonColor = colorList[currentBody->item->id % colorListLength];
            }
            FillTriangle(
                int((viewport.translate(parent, a)).x), int((viewport.translate(parent, a)).y),//A
                int((viewport.translate(parent, b)).x), int((viewport.translate(parent, b)).y),//B
                int((viewport.translate(parent, c)).x), int((viewport.translate(parent, c)).y),//C
                polygonColor);

            //int((viewport.translate(parent, a)).x), int((viewport.translate(parent, b)).y)
            if (currentPgon->next != nullptr) {
                currentPgon = currentPgon->next;
            }
        }

        //int((viewport.translate(p->position, coord())).x)
        if (show_velocity) {
            //draw circle for velocity
            int x = int((viewport.translate(p->position, coord(p->velocity.x * 10, 0))).x);
            int y = int((viewport.translate(p->position, coord(0, p->velocity.y * 10))).y);
            DrawLine(int((viewport.translate(p->position, coord())).x),
                int((viewport.translate(p->position, coord())).y),
                x, y, olc::MAGENTA);
        }
        if (show_accelleration) {
            //draw circle for accelleration
            int x = int((viewport.translate(p->position, coord(p->accelleration.x * 500, 0))).x);
            int y = int((viewport.translate(p->position, coord(0, p->accelleration.y * 500))).y);
            DrawLine(int((viewport.translate(p->position, coord())).x),
                int((viewport.translate(p->position, coord())).y),
                x, y, olc::CYAN);
            
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
