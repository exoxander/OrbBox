#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "PhysicsBody.h"
#include "Utility.h"
#include "Camera.h"
#include "PhysicsSolver.h"
#include "InterfaceRegion.h"

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
    shared_ptr<Utility> util = make_shared<Utility>();
    shared_ptr<Camera> viewport = make_shared<Camera>();
    shared_ptr<bodyList> physicsBodies = make_shared<bodyList>();
    shared_ptr<bodyList> virtualBodies = make_shared<bodyList>();
    PhysicsSolver solver = PhysicsSolver(physicsBodies);
    InterfaceRegion UI = InterfaceRegion(vector2d(0, .9), vector2d(1, 1), physicsBodies, util, viewport);


public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        viewport = make_shared<Camera>(vector2d(), vector2d(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));
        util->game_state = 1;
        physicsBodies->createBody(vector2d(), vector2d(), 50000);//central star
        physicsBodies->createBody(vector2d(100, 0), vector2d(-.1, -1.5), 3500);//planet 1
        physicsBodies->createBody(vector2d(-260, 40), vector2d(.2, .6), 2000);//planet 2
        physicsBodies->createBody(vector2d(-250, 45), vector2d(-.25, 1.15), 300);//moon of planet 2
        physicsBodies->createBody(vector2d(60, 350), vector2d(.75,-.22), 600);//planet 3
        
        return true;
    }

    //-------------------------< ON USER UPDATE >----------------------------
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //camera control
        if (GetKey(olc::Key::LEFT).bHeld) viewport->location.x += 1 * viewport->panSpeed;
        if (GetKey(olc::Key::RIGHT).bHeld) viewport->location.x -= 1 * viewport->panSpeed;
        if (GetKey(olc::Key::UP).bHeld) viewport->location.y += 1 * viewport->panSpeed;
        if (GetKey(olc::Key::DOWN).bHeld) viewport->location.y -= 1 * viewport->panSpeed;
        if (GetKey(olc::Key::NP_ADD).bHeld) viewport->zoom += viewport->zoomSpeed;
        if (GetKey(olc::Key::NP_SUB).bHeld) viewport->zoom -= viewport->zoomSpeed;

        //utility settings
        if (GetKey(olc::Key::SPACE).bPressed) { UI.takeAction(0); }
        if (GetKey(olc::Key::F1).bPressed) { util->polygon_debug_draw = (util->polygon_debug_draw ? false : true); }
        if (GetKey(olc::Key::F2).bPressed) { util->velocity_debug_draw = (util->velocity_debug_draw ? false : true); }
        if (GetKey(olc::Key::F3).bPressed) { util->accelleration_debug_draw = (util->accelleration_debug_draw ? false : true); }

        //-------------------< DRAW BACKGROUND >------------------------
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));

        //----------------------< DRAW VIRTUAL BODIES >------------------------------

        //----------------------< DRAW PHYSICS BODIES >------------------------------
        if (util->game_state == 1 || util->game_state == 2) {//only show during paused or active physics modes
            shared_ptr<body> currentBody = physicsBodies->head;
            while (currentBody != nullptr) {
                drawMesh(currentBody, util->polygon_debug_draw, util->velocity_debug_draw, util->accelleration_debug_draw);
                currentBody = currentBody->next;
            }
        }
        //finished

        //---------------------< DRAW INTERFACE >------------------------
        if (util->show_user_interface) {
            drawInterface(UI);
        }

        //-------------------------< DO PHYSICS STEP >---------------------
        if (util->game_state == 2) {
            solver.step(.001);
        }
        return true;
    }
    //-----------------------------< DRAWMESH >-----------------------------
    void drawMesh(shared_ptr<body> _entity, bool show_polygons, bool show_velocity, bool show_accelleration) {
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
            else {//for now color based on body ID, give actual texture later?
                polygonColor = colorList[currentBody->item->id % colorListLength];
            }
            FillTriangle(
                int((viewport->translate(parent, a)).x), int((viewport->translate(parent, a)).y),//A
                int((viewport->translate(parent, b)).x), int((viewport->translate(parent, b)).y),//B
                int((viewport->translate(parent, c)).x), int((viewport->translate(parent, c)).y),//C
                polygonColor);

            //int((viewport->translate(parent, a)).x), int((viewport->translate(parent, b)).y)
            if (currentPgon->next != nullptr) {
                currentPgon = currentPgon->next;
            }
        }

        //int((viewport->translate(p->position, vector2d())).x)
        if (show_velocity) {
            //draw normalized vector for velocity
            vector2d v = p->velocity;
            v = v.normalize();
            v.multiply(10);
            DrawLine(int((viewport->translate(p->position, vector2d())).x),
                int((viewport->translate(p->position, vector2d())).y),
                int((viewport->translate(p->position, v)).x),
                int((viewport->translate(p->position, v)).y),
                olc::MAGENTA);
        }
        if (show_accelleration) {
            //draw normalized vector for accelleration
            vector2d a = p->accelleration;
            a = a.normalize();
            a.multiply(10);
            DrawLine(int((viewport->translate(p->position, vector2d())).x),
                int((viewport->translate(p->position, vector2d())).y),
                int((viewport->translate(p->position, a)).x),
                int((viewport->translate(p->position, a)).y),
                olc::CYAN);
            
        }
    }

    //----------------------< DRAW USER INTERFACE >-----------------------
    void drawInterface(InterfaceRegion _window) {
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
