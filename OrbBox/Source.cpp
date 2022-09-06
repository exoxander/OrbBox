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
        viewport.location = vector2d(double(ScreenWidth() / 2), double(ScreenHeight()) / 2);
        viewport.zoom = 3;

        PhysicsBody planet = PhysicsBody(vector2d(70, -4), vector2d(0,.8), 3000,24);
        PhysicsBody star = PhysicsBody(vector2d(-10, 14), vector2d(0.01,0), 50000, 32);

        //PhysicsBody mewn = PhysicsBody(vector2d(-50, -200), vector2d(.4,.1), 300);
        PhysicsBody mewn_2 = PhysicsBody(vector2d(80, 35), vector2d(.3, 1), 300);
        PhysicsBody mewn_3 = PhysicsBody(vector2d(-20, 100), vector2d(-.5, .3), 300);
        PhysicsBody mewn_4 = PhysicsBody(vector2d(120, 75), vector2d(-1, .3), 300);

        PhysicsBody planet_2 = PhysicsBody(vector2d(-100, -12), vector2d(0, -.8), 2500, 18);
        PhysicsBody planet_3 = PhysicsBody(vector2d(180, -50), vector2d(-.1,.5), 4500, 24);


        planet.id = 1;
        star.id = 2;
        //mewn.id = 3;
        planet_2.id = 4;
        planet_3.id = 5;
        mewn_2.id = 6;
        mewn_3.id = 7;
        mewn_4.id = 8;

        //adding mewn crashes the program 
        //bodies->addBody(mewn);

        bodies->addBody(planet_2);
        bodies->addBody(mewn_2);
        bodies->addBody(planet_3);
        bodies->addBody(mewn_3);
        bodies->addBody(mewn_4);

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
        if (GetKey(olc::Key::NP_ADD).bHeld) viewport.zoom += viewport.zoomSpeed;
        if (GetKey(olc::Key::NP_SUB).bHeld) viewport.zoom -= viewport.zoomSpeed;
        //fill screen with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        //drawMesh(bodies.head,true);
        shared_ptr<body> currentBody = bodies->head;
        while (currentBody != nullptr) {
            drawMesh(currentBody, false, true, true);
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
            vector2d a = currentPgon->a->position;
            vector2d b = currentPgon->b->position;
            vector2d c = currentPgon->c->position;
            vector2d parent = p->position;
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

        //int((viewport.translate(p->position, vector2d())).x)
        if (show_velocity) {
            //draw normalized vector for velocity
            vector2d v = p->velocity;
            v = v.normalize();
            v.multiply(10);
            DrawLine(int((viewport.translate(p->position, vector2d())).x),
                int((viewport.translate(p->position, vector2d())).y),
                int((viewport.translate(p->position, v)).x),
                int((viewport.translate(p->position, v)).y),
                olc::MAGENTA);
        }
        if (show_accelleration) {
            //draw normalized vector for accelleration
            vector2d a = p->accelleration;
            a = a.normalize();
            a.multiply(10);
            DrawLine(int((viewport.translate(p->position, vector2d())).x),
                int((viewport.translate(p->position, vector2d())).y),
                int((viewport.translate(p->position, a)).x),
                int((viewport.translate(p->position, a)).y),
                olc::CYAN);
            
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
