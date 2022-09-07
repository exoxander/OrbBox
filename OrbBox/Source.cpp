#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "PhysicsBody.h"
#include "Utility.h"
#include "Camera.h"
#include "PhysicsSolver.h"
#include "SubWindow.h"

//const float TickRate = 16;
//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki

class Engine : public olc::PixelGameEngine {

public:
    Engine() {
        sAppName = "Orb_Box";
    }

private:
    olc::Pixel colorList[7] = { olc::WHITE, olc::BLUE, olc::GREEN, olc::RED, olc::YELLOW, olc::GREY, olc::Pixel(255,145,0) };
    int colorListLength = 7;
    Utility u;
    Camera viewport;
    shared_ptr<bodyList> bodies = make_shared<bodyList>();
    PhysicsSolver solver = PhysicsSolver(bodies);
    SubWindow UI = SubWindow(vector2d(0, double(ScreenHeight())*2 + 100), vector2d(double(ScreenWidth())*4, double(ScreenHeight())*2));


public:
    bool OnUserCreate() override {
        //initialize camera
        viewport.location = vector2d(double(ScreenWidth() / 2), double(ScreenHeight()) / 2);
        viewport.zoom = 3;

        bodies->createBody(vector2d(), vector2d(), 50000);//central star
        
        bodies->createBody(vector2d(150, 0), vector2d(-.1, -.8), 3500);//planet 1

        bodies->createBody(vector2d(-300, 40), vector2d(.1, .6), 2000);//planet 2

        bodies->createBody(vector2d(-290, 45), vector2d(-.1, 1.15), 300);//moon of planet 2

        bodies->createBody(vector2d(-30, -170), vector2d(-1.2,-.1), 600);//planet 3
        
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

        //utility settings
        if (GetKey(olc::Key::P).bPressed) { u.game_paused = (u.game_paused ? false : true); }
        if (GetKey(olc::Key::F1).bPressed) { u.polygon_debug_draw = (u.polygon_debug_draw ? false : true); }
        if (GetKey(olc::Key::F2).bPressed) { u.velocity_debug_draw = (u.velocity_debug_draw ? false : true); }
        if (GetKey(olc::Key::F3).bPressed) { u.accelleration_debug_draw = (u.accelleration_debug_draw ? false : true); }


        //fill screen with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        //drawMesh(bodies.head,true);
        shared_ptr<body> currentBody = bodies->head;
        while (currentBody != nullptr) {
            drawMesh(currentBody, u.polygon_debug_draw, u.velocity_debug_draw, u.accelleration_debug_draw);
            currentBody = currentBody->next;
        }
        //finished

        //draw interface to view
        drawInterface(UI);

        //do physics
        if (!u.game_paused) {
            solver.step(.001);
        }
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

    //----------------------< DRAW USER INTERFACE >-----------------------
    void drawInterface(SubWindow _window) {
        //use 2d loop to fill a rectancle specified by window position and dimensions
        for (int i = 1; i <= _window.dimensions.x; i++) {
            for (int u = 1; u <= _window.dimensions.y; u++) {
                //draw a pixel at position + i x u
                Draw(int(_window.position.x) + i, int(_window.position.y) + u, _window.secondary);
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
