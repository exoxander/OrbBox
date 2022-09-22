#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "PhysicsBody.h"
#include "Utility.h"
#include "Camera.h"
#include "PhysicsSolver.h"
#include "InterfaceRegion.h"
#include "MarkupCore.h"
using std::to_string;

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
    shared_ptr<MarkupCore> readWriter = make_shared<MarkupCore>(virtualBodies, physicsBodies);

    string frameTime = "Frame Time:";
    string bodyCount = "Scene Bodies:";
    Pixel BGC = Pixel(10, 10, 10);//background color
    Pixel textColor = Pixel(140, 140, 140);

    PhysicsSolver solver = PhysicsSolver(physicsBodies);
    InterfaceRegion UI = InterfaceRegion(vector2d(0, .9), vector2d(1, 1), physicsBodies, virtualBodies, util, viewport, readWriter);


public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        viewport = make_shared<Camera>(vector2d(), vector2d(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));
        util->game_state = 0;
        virtualBodies->createBody(vector2d(), vector2d(), 50000);//central star
        virtualBodies->createBody(vector2d(100, 0), vector2d(-.1, -1.5), 3500);//planet 1
        virtualBodies->createBody(vector2d(-260, 40), vector2d(.2, .6), 2000);//planet 2
        virtualBodies->createBody(vector2d(-250, 45), vector2d(-.25, 1.2), 300);//moon of planet 2
        virtualBodies->createBody(vector2d(60, 350), vector2d(.75,-.22), 600);//planet 3
        //physicsBodies->makeActual(virtualBodies);
        
        return true;
    }

    //-------------------------< ON USER UPDATE >----------------------------
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //camera control
        if (viewport->target != nullptr) {
            vector2d temp = viewport->target->item->position;
            temp.multiply(-1);
            viewport->location = temp;//force camera to follow a body
            //may act unusually if body removed from list but not deleted
        }
        else {
            if (GetKey(olc::Key::LEFT).bHeld) viewport->location.x += (1 / viewport->zoom) * viewport->panSpeed;
            if (GetKey(olc::Key::RIGHT).bHeld) viewport->location.x -= (1 / viewport->zoom) * viewport->panSpeed;
            if (GetKey(olc::Key::UP).bHeld) viewport->location.y += (1 / viewport->zoom) * viewport->panSpeed;
            if (GetKey(olc::Key::DOWN).bHeld) viewport->location.y -= (1 / viewport->zoom) * viewport->panSpeed;
        }

        if (GetKey(olc::Key::J).bPressed) {//(J)ump to next body as camera target
            if (viewport->target == nullptr || viewport->target->next == nullptr) {
                viewport->target = physicsBodies->head;
            }
            else {
                viewport->target = viewport->target->next;
            }
        }
        if (GetKey(olc::Key::H).bPressed) viewport->target = nullptr;
        if (GetKey(olc::Key::NP_ADD).bHeld) viewport->zoomIn();
        if (GetKey(olc::Key::NP_SUB).bHeld) viewport->zoomOut();

        //utility settings
        if (GetKey(olc::Key::R).bPressed) { UI.takeAction(5); }//any->edit
        if (GetKey(olc::Key::SPACE).bPressed) {
            if (util->game_state == 0) {
                UI.takeAction(6);//edit->paused
            }
            else {
                UI.takeAction(0);//toggle play/paused
            }
        }

        //----------------< DEBUG >--------------------
        if (GetKey(olc::Key::F1).bPressed) { util->polygon_debug_draw = (util->polygon_debug_draw ? false : true); }
        if (GetKey(olc::Key::F2).bPressed) { util->velocity_debug_draw = (util->velocity_debug_draw ? false : true); }
        if (GetKey(olc::Key::F3).bPressed) { util->accelleration_debug_draw = (util->accelleration_debug_draw ? false : true); }
        if (GetKey(olc::Key::F4).bPressed) { 
            if (util->body_debug_draw < 3) {
                util->body_debug_draw++;
            }
            else {
                util->body_debug_draw = 0;
            }
        }
        if (GetKey(olc::Key::S).bPressed && GetKey(olc::Key::CTRL).bHeld) { UI.takeAction(4); }

        //-------------------< DRAW BACKGROUND >------------------------
        if (util->game_state == 0) {
            BGC = Pixel(10,10,200);
        }
        else if (util->game_state == 1) {
            BGC = Pixel(60,60,100);
        }
        else {
            BGC = Pixel(10, 10, 20);
        }
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, BGC);

        //----------------------< DRAW VIRTUAL BODIES >------------------------------
        if (util->game_state == 0) {//edit mode
            drawVirtuals(virtualBodies, viewport->zoom);
        }

        //----------------------< DRAW PHYSICS BODIES >------------------------------
        if (util->game_state == 1 || util->game_state == 2) {//only show during paused or active physics modes
            shared_ptr<body> currentBody = physicsBodies->head;
            while (currentBody != nullptr) {//do culling check later
                drawMesh(currentBody);
                currentBody = currentBody->next;
            }
        }
        //finished

        //---------------------< DRAW INTERFACE >------------------------
        if (util->show_user_interface) {
            drawInterface(UI);
            string temp = frameTime;
            temp.append(to_string(fElapsedTime));
            DrawString(0, 5, temp, olc::RED, 1);

            temp = bodyCount;
            temp.append(to_string(physicsBodies->length));
            DrawString(0, 15, temp, olc::RED, 1);
        }

        //-------------------------< DO PHYSICS STEP >---------------------
        if (util->game_state == 2) {
            //solver.matrixStep(.01);
            solver.step(physicsBodies, .001);
        }

        return true;
    }

    //---------------------< DRAW VIRTUALS >------------------------------
    void drawVirtuals(shared_ptr<bodyList> _bodies, double _scale = 1){
        shared_ptr<body> currentBody = _bodies->head;
        shared_ptr<bodyList> pathList = make_shared<bodyList>();
        pathList->makeActual(virtualBodies);

        while (currentBody != nullptr) {//virtuals
            vector2d tPos = viewport->translate(currentBody->item->position, vector2d());
            FillCircle(int(tPos.x), int(tPos.y), int(currentBody->item->radius * _scale));
            currentBody = currentBody->next;
        }

        for (int n = 0; n < 15; n++) {
            for (int i = 0; i < 10; i++) {
                solver.step(pathList, .001);
            }
            currentBody = pathList->head;
            while (currentBody != nullptr) {//virtuals path
                vector2d tPos = viewport->translate(currentBody->item->position, vector2d());
                FillCircle(int(tPos.x), int(tPos.y), 3, olc::Pixel(60,60,120));
                currentBody = currentBody->next;
            }
        }
    }
    void drawPath(shared_ptr<body> _body ,int _iterations = 5) {

    }
    //-----------------------------< DRAWMESH >-----------------------------
    void drawMesh(shared_ptr<body> _entity) {
        shared_ptr<body> currentBody = _entity;
        vector2d tpp = viewport->translate(_entity->item->position, vector2d());
        shared_ptr<PhysicsBody> p = currentBody->item;
        mesh m = currentBody->item->getMesh();   

        //loop through all polygons and draw to screen
        shared_ptr<polygon> currentPgon = m.polygonList;
        for (int i = 0; i < m.pgonLength; i++) {
            vector2d parent = p->position;
            vector2d a = viewport->translate(parent, currentPgon->a->position);
            vector2d b = viewport->translate(parent, currentPgon->b->position);
            vector2d c = viewport->translate(parent, currentPgon->c->position);
            olc::Pixel polygonColor;

            if (util->polygon_debug_draw) {
                //make color result of % between polygon id and color list
                 polygonColor = colorList[currentPgon->id % colorListLength];
            }
            else {//for now color based on body ID, give actual texture later?
                polygonColor = colorList[currentBody->item->id % colorListLength];
            }
            FillTriangle(
                int(a.x), int(a.y),//A
                int(b.x), int(b.y),//B
                int(c.x), int(c.y),//C
                polygonColor);

            //int((viewport->translate(parent, a)).x), int((viewport->translate(parent, b)).y)
            if (currentPgon->next != nullptr) {
                currentPgon = currentPgon->next;
            }
        }

        //int((viewport->translate(p->position, vector2d())).x)
        if (util->velocity_debug_draw) {
            //draw normalized vector for velocity
            vector2d v = p->velocity;
            v = v.normalize();
            v.multiply(25 * (1/viewport->zoom));
            DrawLine(int(tpp.x),
                int(tpp.y),
                int((viewport->translate(p->position, v)).x),
                int((viewport->translate(p->position, v)).y),
                olc::MAGENTA);
        }
        if (util->accelleration_debug_draw) {
            //draw normalized vector for accelleration
            vector2d a = p->accelleration;
            a = a.normalize();
            a.multiply(30 * (1/viewport->zoom));
            DrawLine(int(tpp.x),
                int(tpp.y),
                int((viewport->translate(p->position, a)).x),
                int((viewport->translate(p->position, a)).y),
                olc::CYAN);            
        }
        if (util->body_debug_draw != 0) {
            string temp = "ID:";
            temp.append(to_string(currentBody->item->id));
            DrawString(int(tpp.x), int(tpp.y), temp, textColor);//id
            if (util->body_debug_draw > 1) {
                temp = "Mass:";
                temp.append(to_string(currentBody->item->mass));
                DrawString(int(tpp.x), int(tpp.y) + 15, temp, textColor);//mass
                temp = "Velocity:(";
                temp.append(to_string(currentBody->item->velocity.x));
                temp.append(",");
                temp.append(to_string(currentBody->item->velocity.y));
                temp.append(")");
                DrawString(int(tpp.x), int(tpp.y) + 30, temp, textColor);//velocity?
            }
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
