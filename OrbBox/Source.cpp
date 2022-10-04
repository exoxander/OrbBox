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
    int colorOffset = 3;
    shared_ptr<Utility> util = make_shared<Utility>();
    shared_ptr<Camera> viewport = make_shared<Camera>();
    shared_ptr<bodyList> physicsBodies = make_shared<bodyList>();
    shared_ptr<bodyList> virtualBodies = make_shared<bodyList>();
    shared_ptr<MarkupCore> readWriter = make_shared<MarkupCore>(virtualBodies, physicsBodies);

    shared_ptr<vertex> virtualPath = nullptr;
    string frameTime = "Frame Time:";
    string bodyCount = "Scene Bodies:";
    Pixel BGC = Pixel(10, 10, 10);//background color
    Pixel textColor = Pixel(140, 140, 140);
    double GRAVITY_CONSTANT = 0.001;
    PhysicsSolver solver = PhysicsSolver(physicsBodies);
    InterfaceRegion UI = InterfaceRegion(vector2d(0, 0), vector2d(80, 12), physicsBodies, virtualBodies, util, viewport, readWriter);


public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        //util->game_state = 0;
        util->virtual_list_changed = true;
        viewport = make_shared<Camera>(vector2d(), vector2d(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));

        //default system
        virtualBodies->createDefaultSystem();
        virtualBodies->getBody(3)->item->bodyMesh = util->generateBox(6);
        //virtualBodies->createBody(vector2d(), vector2d(), 100000);
        //virtualBodies->createBody(vector2d(-80,0), vector2d(0,2), 1000);//mercury :D
        //virtualBodies->createBody(vector2d(190, 0), vector2d(0, -util->getStableSpeed(100000, 190, GRAVITY_CONSTANT)), 67000);
        
        return true;
    }

    //-------------------------< ON USER UPDATE >----------------------------
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //camera control
        if (viewport->target != nullptr && viewport->target->item->id != -1) {
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
        if (GetKey(olc::Key::R).bPressed) { UI.takeAction(ACTION::forceEdit); }//any->edit
        if (GetKey(olc::Key::SPACE).bPressed) {
            if (util->game_state == GAME_STATE::edit) {
                UI.takeAction(ACTION::saveSim);//edit->paused
            }
            else {
                UI.takeAction(ACTION::togglePlay);//toggle play/paused
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
        if (GetKey(olc::Key::S).bPressed && GetKey(olc::Key::CTRL).bHeld && (util->game_state == GAME_STATE::pause || util->game_state == GAME_STATE::play)) { UI.takeAction(ACTION::saveSim); }

        //-------------------< DRAW BACKGROUND >------------------------
        if (util->game_state == GAME_STATE::edit) {
            BGC = Pixel(10,10,200);
        }
        else if (util->game_state == GAME_STATE::pause) {
            BGC = Pixel(60,60,100);
        }
        else {
            BGC = Pixel(10, 10, 20);
        }
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, BGC);

        //----------------------< DRAW VIRTUAL BODIES >------------------------------
        if (util->game_state == GAME_STATE::edit) {//edit mode
            virtualPath = drawVirtuals(virtualBodies, virtualPath, viewport->zoom);
        }

        //----------------------< DRAW PHYSICS BODIES >------------------------------
        if (util->game_state == GAME_STATE::pause || util->game_state == GAME_STATE::play) {//only show during paused or active physics modes
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
        if (util->game_state == GAME_STATE::play) {
            //solver.matrixStep(.01);
            solver.step(physicsBodies, GRAVITY_CONSTANT);
        }

        return true;
    }

    //---------------------< DRAW VIRTUALS >------------------------------
    shared_ptr<vertex> drawVirtuals(shared_ptr<bodyList> _bodies, shared_ptr<vertex> _vPath, double _scale = 1){
        shared_ptr<body> currentBody = _bodies->head;
        shared_ptr<vertex> nextHead = nullptr;
        shared_ptr<vertex> currentVertex;
        shared_ptr<vertex> nextVertex;
        shared_ptr<bodyList> pathList = make_shared<bodyList>();
        pathList->makeActual(virtualBodies);

        while (currentBody != nullptr) {//virtuals
            vector2d tPos = viewport->translate(currentBody->item->position );
            FillCircle(int(tPos.x), int(tPos.y), int(currentBody->item->radius * _scale));
            currentBody = currentBody->next;
        }
        currentBody = pathList->head;
        if (util->virtual_list_changed) {//use chain of vertecies for positions, vertex ID = body ID
            for (int n = 0; n < 15; n++) {
                for (int i = 0; i < 10; i++) {
                    solver.step(pathList, .001);
                }
                while (currentBody != nullptr) {//virtuals path
                    vector2d tPos = currentBody->item->position;
                    if (nextHead != nullptr) {
                        nextVertex = make_shared<vertex>(tPos, currentBody->item->id);
                        currentVertex->next = nextVertex;
                        currentVertex = nextVertex;
                        //DrawString(0, 60, "drawing to vPath");
                    }
                    else {
                        nextHead = make_shared<vertex>(tPos, currentBody->item->id);
                        currentVertex = nextHead;
                    }
                    currentBody = currentBody->next;
                }
                currentBody = pathList->head;
            }
            
        }
        else {
            currentVertex = _vPath;
            while (currentVertex != nullptr) {
                vector2d vertPos = viewport->translate(currentVertex->position,vector2d());
                DrawCircle(int(vertPos.x), int(vertPos.y), 4, olc::RED);
                currentVertex = currentVertex->next;
            }
        }
        util->virtual_list_changed = false;       
        if (nextHead != nullptr) {
            return nextHead;
        }
        else {
            return _vPath;
        }
    }
    void drawPath(shared_ptr<body> _body ,int _iterations = 5) {

    }
    //-----------------------------< DRAWMESH >-----------------------------
    void drawMesh(shared_ptr<body> _entity) {
        shared_ptr<body> currentBody = _entity;
        vector2d tpp = viewport->translate(_entity->item->position );
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
                polygonColor = colorList[(currentBody->item->id + colorOffset) % colorListLength];
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

        //int((viewport->translate(p->position )).x)
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
        int x = int(_window.position.x);
        int y = int(_window.position.y);
        int w = int(_window.dimensions.x);
        int h = int(_window.dimensions.y);
        FillRect(x,y,w,h,_window.primary);
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
