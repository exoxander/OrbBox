#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "Body.h"
#include "ScreenObject.h"
#include "Utility.h"
#include "Camera.h"
#include "PhysicsSolver.h"
#include "InterfaceManager.h"
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
    double GRAVITY_CONSTANT = 0.001;
    shared_ptr<Utility> util = make_shared<Utility>();
    shared_ptr<Camera> viewport = make_shared<Camera>();
    
    PhysicsSolver solver = PhysicsSolver();
    InterfaceManager UI = InterfaceManager();


public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        //util->game_state = 0;
        util->virtual_list_changed = true;
        viewport = make_shared<Camera>(vector2d(), vector2d(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));
        
        return true;
    }

    //-------------------------< ON USER UPDATE >----------------------------
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        //camera control
        if (GetKey(olc::Key::LEFT).bHeld) viewport->location.x += (1 / viewport->zoom) * viewport->panSpeed;
        if (GetKey(olc::Key::RIGHT).bHeld) viewport->location.x -= (1 / viewport->zoom) * viewport->panSpeed;
        if (GetKey(olc::Key::UP).bHeld) viewport->location.y += (1 / viewport->zoom) * viewport->panSpeed;
        if (GetKey(olc::Key::DOWN).bHeld) viewport->location.y -= (1 / viewport->zoom) * viewport->panSpeed;
        if (GetKey(olc::Key::NP_ADD).bHeld) viewport->zoomIn();
        if (GetKey(olc::Key::NP_SUB).bHeld) viewport->zoomOut();

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
        //-------------------< DRAW BACKGROUND >------------------------
        
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(20,20,40));

        //----------------------< DRAW VIRTUAL BODIES >------------------------------
        if (util->game_state == GAME_STATE::edit) {//edit mode
            
        }

        //----------------------< DRAW PHYSICS BODIES >------------------------------
        if (util->game_state == GAME_STATE::pause || util->game_state == GAME_STATE::play) {//only show during paused or active physics modes
            
        }
        //finished

        //---------------------< DRAW INTERFACE >------------------------
        if (util->show_user_interface) {
        }

        //-------------------------< DO PHYSICS STEP >---------------------
        if (util->game_state == GAME_STATE::play) {
        }

        return true;
    }

    //---------------------< DRAW VIRTUALS >------------------------------
    
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
    void drawInterface() {
      
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
