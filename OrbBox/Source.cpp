#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "Body.h"
#include "ScreenObject.h"
#include "Utility.h"
#include "Camera.h"
#include "PhysicsSolver.h"
#include "InterfaceManager.h"
//#include "MarkupCore.h"
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
    //(id + collorOffset) % colorListLength
    double GRAVITY_CONSTANT = 0.001;
    shared_ptr<Utility> util = make_shared<Utility>();
    shared_ptr<Camera> viewport = make_shared<Camera>();
    
    PhysicsSolver solver = PhysicsSolver();
    InterfaceManager UI = InterfaceManager();

public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        util->draw_mode = MESH_DRAW_MODE::wireframe;
        viewport = make_shared<Camera>(vector2d(), vector2d(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));

        //render and whatnot test
        //mesh m = util->generateCircle(10, 12);
        mesh m = mesh();
        UI.pages.add(shared_ptr<Page>());
        shared_ptr<ScreenObject> s = make_shared<ScreenObject>(vector2i(0, 0), make_shared<mesh>(m), 1, 1);
        UI.pages.head->item->pageObjects.add(s);
        UI.currentPage = UI.pages.head;
        
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
        //if (GetKey(olc::Key::F1).bPressed) { util->polygon_debug_draw = (util->polygon_debug_draw ? false : true); }
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
        drawScreenObjects(UI.currentPage->item->pageObjects);
        /*
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
        */
        return true;
    }

    //---------------------< DRAW VIRTUALS >------------------------------
    
    //-----------------------------< DRAWMESH >-----------------------------
    void drawScreenObjects(list<ScreenObject> _objects) {
        shared_ptr<bin<ScreenObject>> currentObject = _objects.head;
        /*
        vector2d parent = p->position;
            vector2i a = viewport->translate(parent, currentPgon->a->position);
            vector2i b = viewport->translate(parent, currentPgon->b->position);
            vector2i c = viewport->translate(parent, currentPgon->c->position);

            FillTriangle(
                int(a.x), int(a.y),//A
                int(b.x), int(b.y),//B
                int(c.x), int(c.y),//C
                polygonColor);
        */
        //loop through objects
        while (currentObject != nullptr) {
            if (currentObject->item->show) {
                if (true) {//do viewport culling later
                    shared_ptr<mesh> m = currentObject->item->visualMesh;
                    vector2d p = currentObject->item->physicsBody->position;
                    vector2i a;
                    vector2i b;
                    vector2i c;
                    int vertexScale = 3;
                    //wireframe
                    if (util->draw_mode == MESH_DRAW_MODE::wireframe) {
                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;

                        while (currentPolygon != nullptr) {
                            a = viewport->translate(p, currentPolygon->item->a->position);
                            b = viewport->translate(p, currentPolygon->item->b->position);
                            c = viewport->translate(p, currentPolygon->item->c->position);

                            //draw circles on vertecies
                            DrawCircle(a.x, a.y, vertexScale);
                            DrawCircle(b.x, b.y, vertexScale);
                            DrawCircle(c.x, c.y, vertexScale);

                            //draw lines between vertecies
                            DrawLine(a.x, a.y, b.x, b.y);
                            DrawLine(b.x, b.y, c.x, c.y);
                            DrawLine(c.x, c.y, a.x, a.y);

                            currentPolygon = currentPolygon->next;
                        }
                    }

                    //triangle
                    if (util->draw_mode == MESH_DRAW_MODE::triangle) {

                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;
                        Pixel color;

                        while (currentPolygon != nullptr) {
                            color = colorList[(currentPolygon->item_id) % colorListLength];
                            a = viewport->translate(p, currentPolygon->item->a->position);
                            b = viewport->translate(p, currentPolygon->item->b->position);
                            c = viewport->translate(p, currentPolygon->item->c->position);

                            //draw each polygon with a color from the list
                            FillTriangle(
                                int(a.x), int(a.y),//A
                                int(b.x), int(b.y),//B
                                int(c.x), int(c.y),//C
                                color);
                            currentPolygon = currentPolygon->next;
                        }
                    }

                    //solid
                    if (util->draw_mode == MESH_DRAW_MODE::solid) {
                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;
                        Pixel color = colorList[(currentObject->item_id) % colorListLength];

                        while (currentPolygon != nullptr) {
                            a = viewport->translate(p, currentPolygon->item->a->position);
                            b = viewport->translate(p, currentPolygon->item->b->position);
                            c = viewport->translate(p, currentPolygon->item->c->position);

                            //draw each polygon with a color from the list
                            FillTriangle(
                                int(a.x), int(a.y),//A
                                int(b.x), int(b.y),//B
                                int(c.x), int(c.y),//C
                                color);
                            currentPolygon = currentPolygon->next;
                        }
                    }

                    //textured
                    if (util->draw_mode == MESH_DRAW_MODE::texture) {
                        //hell if i know how to impliment this
                    }
                }
            }
            currentObject = currentObject->next;
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
