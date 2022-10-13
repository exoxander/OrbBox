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
    list<body> activeBodies = list<body>();
    PhysicsSolver solver = PhysicsSolver();
    InterfaceManager UI = InterfaceManager();

public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        util->draw_mode = MESH_DRAW_MODE::wireframe;
        util->game_state = GAME_STATE::pause;
        viewport = make_shared<Camera>(vector2d(), vector2d(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));
        //viewport->panSpeed *= .25;

        //render and whatnot test
        mesh m1 = util->generateCircle(50, 12);
        mesh m2 = util->generateCircle(10, 12);
        mesh m3 = util->generateCircle(3, 12);
        shared_ptr<Page> p = make_shared<Page>();
        UI.pages.add(p);
        UI.currentPage = UI.pages.head;

        //test bodies
        shared_ptr<body> b1 = make_shared<body>(vector2d(), vector2d(), vector2d(), 1, 50000);
        shared_ptr<body> b2 = make_shared<body>(vector2d(100,10), vector2d(0,-1.5), vector2d(), 2, 1000);
        shared_ptr<body> b3 = make_shared<body>(vector2d(-200,0), vector2d(0,1), vector2d(), 2, 100);

        //test objects
        shared_ptr<ScreenObject> s1 = make_shared<ScreenObject>(vector2i(15, 15), make_shared<mesh>(), 3, 1);
        shared_ptr<ScreenObject> p1 = make_shared<ScreenObject>(b1, make_shared<mesh>(m1));
        shared_ptr<ScreenObject> p2 = make_shared<ScreenObject>(b2, make_shared<mesh>(m2));
        shared_ptr<ScreenObject> p3 = make_shared<ScreenObject>(b3, make_shared<mesh>(m3));
        shared_ptr<ScreenObject> s2 = make_shared<ScreenObject>(vector2i(50, 24), make_shared<mesh>(), 6, 4);

        UI.addToCurrentPage(s1);
        UI.addToCurrentPage(s2);
        UI.addToCurrentPage(p1);
        UI.addToCurrentPage(p2);
        UI.addToCurrentPage(p3);
        
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
        if (GetKey(olc::Key::SPACE).bPressed) { util->game_state = (util->game_state == GAME_STATE::pause ? GAME_STATE::play : GAME_STATE::pause); }
        if (GetKey(olc::Key::F2).bPressed) { util->draw_velocity = (util->draw_velocity ? false : true); }
        if (GetKey(olc::Key::F3).bPressed) { util->draw_acceleration = (util->draw_acceleration ? false : true); }
        if (GetKey(olc::Key::F4).bPressed) { 
            if (util->draw_body_info < 3) {
                util->draw_body_info++;
            }
            else {
                util->draw_body_info = 0;
            }
        }
        //-------------------< DRAW BACKGROUND >------------------------
        
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(20,20,40));
        drawScreenObjects(UI.currentPage->item->pageObjects);
      
        //-------------------------< DO PHYSICS STEP >---------------------
        if (util->game_state == GAME_STATE::play) {
            solver.step(UI.currentPage->item->pageBodies);
        }
        
        return true;
    }
    
    //-----------------------------< DRAWMESH >-----------------------------
    void drawScreenObjects(list<ScreenObject> _objects) {
        shared_ptr<bin<ScreenObject>> currentObject = _objects.head;
        //loop through objects
        while (currentObject != nullptr) {
            if (currentObject->item->show) {
                if (true) {//do viewport culling later
                    shared_ptr<mesh> m = currentObject->item->visualMesh;
                    vector2d p;
                    vector2i a;
                    vector2i b;
                    vector2i c;
                    double scale = currentObject->item->scale;
                    bool hasBody = (currentObject->item->physicsBody == nullptr ? false: true);

                    if (hasBody) {
                        p = currentObject->item->physicsBody->position;
                    }

                    int vertexScale = 3;
                    //wireframe
                    if (util->draw_mode == MESH_DRAW_MODE::wireframe) {
                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;

                        while (currentPolygon != nullptr) {
                            if(hasBody){
                                a = viewport->translate(p, currentPolygon->item->a->position);
                                b = viewport->translate(p, currentPolygon->item->b->position);
                                c = viewport->translate(p, currentPolygon->item->c->position);
                            }
                            else {
                                a = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->a->position, scale);
                                b = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->b->position, scale);
                                c = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->c->position, scale);
                            }

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
                        Pixel color = olc::WHITE;

                        while (currentPolygon != nullptr) {
                            color = colorList[(currentPolygon->item_id) % colorListLength];

                            if (hasBody) {
                                a = viewport->translate(p, currentPolygon->item->a->position);
                                b = viewport->translate(p, currentPolygon->item->b->position);
                                c = viewport->translate(p, currentPolygon->item->c->position);
                            }
                            else {
                                a = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->a->position, scale);
                                b = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->b->position, scale);
                                c = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->c->position, scale);
                            }

                            //draw each polygon with a color from the list
                            FillTriangle(
                                a.x, a.y,//A
                                b.x, b.y,//B
                                c.x, c.y,//C
                                color);
                            currentPolygon = currentPolygon->next;
                        }
                    }

                    //solid
                    if (util->draw_mode == MESH_DRAW_MODE::solid) {
                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;
                        Pixel color = colorList[(currentObject->item_id) % colorListLength];

                        while (currentPolygon != nullptr) {

                            if (hasBody) {
                                a = viewport->translate(p, currentPolygon->item->a->position);
                                b = viewport->translate(p, currentPolygon->item->b->position);
                                c = viewport->translate(p, currentPolygon->item->c->position);
                            }
                            else {
                                a = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->a->position, scale);
                                b = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->b->position, scale);
                                c = viewport->translate(currentObject->item->screenPosition, currentPolygon->item->c->position, scale);
                            }

                            //draw each polygon with a color from the list
                            FillTriangle(
                                a.x, a.y,//A
                                b.x, b.y,//B
                                c.x, c.y,//C
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
            if (currentObject->item->physicsBody != nullptr) {
                drawDebugInfo(currentObject->item->physicsBody);
            }
            currentObject = currentObject->next;
        }
    }

    void drawDebugInfo(shared_ptr<body> _body) {
        vector2i start = viewport->translate(_body->position, vector2d());
        vector2d b;

        if (util->draw_acceleration) {
            b = _body->acceleration;
            b.normalize();
            b.multiply(50 / viewport->zoom);
            vector2i end = viewport->translate(_body->position, b);
            DrawLine(start.x, start.y, end.x, end.y, Pixel(255, 255, 0));
        }
        if (util->draw_velocity) {
            b = _body->velocity;
            b.normalize();
            b.multiply(50 / viewport->zoom);
            vector2i end = viewport->translate(_body->position, b);
            DrawLine(start.x, start.y, end.x, end.y, Pixel(0, 255, 255));
        }
        if (util->draw_body_info > 0) {
            //stuff
            if (util->draw_body_info > 1) {
                //more stuff
                if (util->draw_body_info > 2) {
                    //final level of stuff
                }
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
