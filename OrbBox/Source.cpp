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
    int renderPathDepth = 60;
    int renderPathSkips = 5;

    //UI testing
    //list<Button> buttonList = list<Button>();

public:
    bool OnUserCreate() override {  
        //initialize camera
        util->show_user_interface = true;
        util->draw_mode = MESH_DRAW_MODE::solid;
        //util->game_state = GAME_STATE::edit;
        viewport = make_shared<Camera>(dVector(), dVector(double(ScreenWidth() / 2), double(ScreenHeight() / 2)));
        //viewport->panSpeed *= .25;

        //render and whatnot test
        mesh m3 = util->generateCircle(3, 6);
        mesh m1 = util->generateCircle(50, 16);
        mesh m2 = util->generateCircle(10, 8);
        mesh m4 = mesh();
        mesh m5 = m4.copy();
        shared_ptr<Page> p = make_shared<Page>();
        p->type = PAGE_TYPE::editSim;
        UI.pages.add(p);
        UI.currentPage = UI.pages.head;

        //test bodies
        shared_ptr<body> b1 = make_shared<body>(dVector(), dVector(), dVector(), 50000);
        shared_ptr<body> b2 = make_shared<body>(dVector(-200,0), dVector(0,1.8), dVector(), 1000);
        shared_ptr<body> b3 = make_shared<body>(dVector(-180,-10), dVector(.3,2.4), dVector(), 100);
        shared_ptr<body> b4 = make_shared<body>(dVector(-70, -30), dVector(-.5, 3.2), dVector(), 300);
        shared_ptr<body> b5 = make_shared<body>(dVector(180, -30), dVector(-1.2, -1.2), dVector(), 100);
        shared_ptr<body> b6 = make_shared<body>(dVector(-20, -90), dVector(-2.4, .1), dVector(), 200);
        //shared_ptr<body> b6 = make_shared<body>(b5->copy());
        //b6->position = dVector(-30, -110);
        //b6->velocity = dVector(-2.4, .1);
        //b6->mass = 5000;

        //test objects
        shared_ptr<ScreenObject> s1 = make_shared<ScreenObject>(iVector(15, 15), make_shared<mesh>(m4), SO_TYPE::visualObject, 1);
        shared_ptr<ScreenObject> p1 = make_shared<ScreenObject>(b1, make_shared<mesh>(m1), nullptr, SO_TYPE::physicsObject); 
        shared_ptr<ScreenObject> p2 = make_shared<ScreenObject>(b2, make_shared<mesh>(m2), nullptr, SO_TYPE::physicsObject);
        shared_ptr<ScreenObject> p3 = make_shared<ScreenObject>(b3, make_shared<mesh>(m3), nullptr, SO_TYPE::physicsObject);
        shared_ptr<ScreenObject> p4 = make_shared<ScreenObject>(b4, make_shared<mesh>(m3), nullptr, SO_TYPE::physicsObject);
        shared_ptr<ScreenObject> p5 = make_shared<ScreenObject>(b5, make_shared<mesh>(m5), nullptr, SO_TYPE::physicsObject);
        shared_ptr<ScreenObject> p6 = make_shared<ScreenObject>(b6, make_shared<mesh>(m5), nullptr, SO_TYPE::physicsObject);
        shared_ptr<ScreenObject> s2 = make_shared<ScreenObject>(iVector(50, 24), make_shared<mesh>(), SO_TYPE::visualObject, 4);

        //adding
        UI.addToCurrentPage(s1);
        UI.addToCurrentPage(s2);
        UI.addToCurrentPage(p1);
        UI.addToCurrentPage(p2);
        UI.addToCurrentPage(p3);
        UI.addToCurrentPage(p4);
        UI.addToCurrentPage(p5);
        UI.addToCurrentPage(p6);
        
        UI.buildObjectHandles(viewport, iVector(30,30));
        UI.currentPage->item->copyDynamicObjects(UI.currentPage->item->staticObjects, UI.currentPage->item->staticBodies);
        UI.renderedPaths = renderPaths(renderPathDepth, renderPathSkips);
        UI.currentPage->item->dynamicObjectsVisible(false);
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

        //buttonz
        if (UI.currentPage->item->type == PAGE_TYPE::editSim) {
            //check button clicks
            //run through list
            //if one returns is clicked, do something with its position and mouse while held
            //once released, set linked body position to a reverse-spaceTranslated value / 10 (handle is 10x farther out)
            if (GetMouse(0).bReleased) {
                shared_ptr<Button> currentButton = UI.currentPage->item->pageButtons.getItemByID(Button(), UI.activeButton);
                if (currentButton != nullptr) {
                    if (currentButton->type == BUTTON_TYPE::handle) {
                        shared_ptr<ScreenObject> temp = std::dynamic_pointer_cast<ObjectHandle>(currentButton)->item;
                        if (temp != nullptr) {
                            temp->physicsBody->position = viewport->reversespaceTranslate(currentButton->position);
                            UI.renderedPaths.clear();
                            UI.currentPage->item->dynamicObjects.clear();
                            UI.currentPage->item->copyDynamicObjects(UI.currentPage->item->staticObjects, UI.currentPage->item->staticBodies);
                            UI.renderedPaths = renderPaths(renderPathDepth, renderPathSkips);
                            UI.currentPage->item->dynamicObjectsVisible(false);
                        }
                    }
                }
                //last part
                UI.activeButton = -1;
            }
            if (UI.activeButton == -1 && (GetMouse(0).bHeld)) {//if mouse down and no active button, search buttons
                shared_ptr<bin<Button>> currentButton = UI.currentPage->item->pageButtons.head;
                while (currentButton != nullptr) {
                    //cout << "checking button:  " << currentButton->itemID << endl;
                    if (currentButton->item->mouseIsOn(iVector(GetMouseX(), GetMouseY()))) {//this button is pressed
                        UI.activeButton = currentButton->itemID;
                        //cout << "button found: " << currentButton->itemID << endl;
                        break;
                    }
                    currentButton = currentButton->next;
                }
            }
            //follow on incase its set in previous statement
            if (GetMouse(0).bHeld && UI.activeButton >= 0) {//button is active
                shared_ptr<Button> pressedButton = UI.currentPage->item->pageButtons.getItemByID(Button(), UI.activeButton);
                //check if button is a handle, type cast if so, then bind to mouse position and change object velocity
            }
        }

        //----------------< DEBUG >--------------------
        if (GetKey(olc::Key::F1).bPressed) { util->iterateDrawMode(); }
        if (GetKey(olc::Key::F5).bPressed) { cout << "registered F5 press" << endl; }
        if (GetKey(olc::Key::SPACE).bPressed) { 
            if(UI.currentPage->item->type == PAGE_TYPE::runSim) {
                util->game_state = (util->game_state == GAME_STATE::pause ? GAME_STATE::play : GAME_STATE::pause); 
            } 
        }
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
        drawScreenObjects(UI.currentPage->item->staticObjects);
        drawScreenObjects(UI.currentPage->item->dynamicObjects);
        drawInterface();
      
        //-------------------------< DO PHYSICS STEP >---------------------
        if (util->game_state == GAME_STATE::play) {
            solver.step(UI.currentPage->item->dynamicBodies);
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
                    dVector p;
                    iVector a;
                    iVector b;
                    iVector c;
                    double scale = currentObject->item->scale;
                    bool hasBody = (currentObject->item->physicsBody == nullptr ? false: true);

                    if (hasBody) {
                        p = currentObject->item->physicsBody->position;
                    }

                    int vertexScale = 2;
                    //wireframe
                    if (util->draw_mode == MESH_DRAW_MODE::wireframe) {
                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;

                        while (currentPolygon != nullptr) {
                            //ignore any 'invalid' polygons in the list
                            if (currentPolygon->itemID >= 0) {
                                if (hasBody) {
                                    a = viewport->spaceTranslate(p, currentPolygon->item->a->position);
                                    b = viewport->spaceTranslate(p, currentPolygon->item->b->position);
                                    c = viewport->spaceTranslate(p, currentPolygon->item->c->position);
                                }
                                else {
                                    a = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->a->position, scale);
                                    b = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->b->position, scale);
                                    c = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->c->position, scale);
                                }

                                //draw circles on vertecies
                                DrawCircle(a.x, a.y, vertexScale);
                                DrawCircle(b.x, b.y, vertexScale);
                                DrawCircle(c.x, c.y, vertexScale);

                                //draw lines between vertecies
                                DrawLine(a.x, a.y, b.x, b.y);
                                DrawLine(b.x, b.y, c.x, c.y);
                                DrawLine(c.x, c.y, a.x, a.y);
                            }

                            currentPolygon = currentPolygon->next;
                        }
                    }

                    //triangle
                    if (util->draw_mode == MESH_DRAW_MODE::triangle) {

                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;
                        Pixel color = olc::WHITE;

                        while (currentPolygon != nullptr) {
                            //ignore invalid items
                            if (currentPolygon->itemID >= 0) {
                                color = colorList[(currentPolygon->itemID) % colorListLength];

                                if (hasBody) {
                                    a = viewport->spaceTranslate(p, currentPolygon->item->a->position);
                                    b = viewport->spaceTranslate(p, currentPolygon->item->b->position);
                                    c = viewport->spaceTranslate(p, currentPolygon->item->c->position);
                                }
                                else {
                                    a = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->a->position, scale);
                                    b = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->b->position, scale);
                                    c = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->c->position, scale);
                                }

                                //draw each polygon with a color from the list
                                FillTriangle(
                                    a.x, a.y,//A
                                    b.x, b.y,//B
                                    c.x, c.y,//C
                                    color);
                            }
                            currentPolygon = currentPolygon->next;
                        }
                    }

                    //solid
                    if (util->draw_mode == MESH_DRAW_MODE::solid) {
                        shared_ptr<bin<polygon>> currentPolygon = m->polygonList.head;
                        Pixel color = colorList[(currentObject->itemID) % colorListLength];

                        while (currentPolygon != nullptr) {
                            //ignore invalid items
                            if (currentPolygon->itemID >= 0) {

                                if (hasBody) {
                                    a = viewport->spaceTranslate(p, currentPolygon->item->a->position);
                                    b = viewport->spaceTranslate(p, currentPolygon->item->b->position);
                                    c = viewport->spaceTranslate(p, currentPolygon->item->c->position);
                                }
                                else {
                                    a = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->a->position, scale);
                                    b = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->b->position, scale);
                                    c = viewport->spaceTranslate(currentObject->item->screenPosition, currentPolygon->item->c->position, scale);
                                }

                                //draw each polygon with a color from the list
                                FillTriangle(
                                    a.x, a.y,//A
                                    b.x, b.y,//B
                                    c.x, c.y,//C
                                    color);
                            }
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
        iVector start = viewport->spaceTranslate(_body->position, dVector());
        dVector b;
        int rad = 2;

        if (util->draw_acceleration) {
            b = _body->acceleration;
            b.normalize();
            b.multiply(50 / viewport->zoom);
            iVector end = viewport->spaceTranslate(_body->position, b);
            DrawLine(start.x, start.y, end.x, end.y, Pixel(255, 255, 0));
            FillCircle(end.x, end.y, rad, Pixel(255, 255, 0));
        }
        if (util->draw_velocity) {
            b = _body->velocity;
            b.normalize();
            b.multiply(50 / viewport->zoom);
            iVector end = viewport->spaceTranslate(_body->position, b);
            DrawLine(start.x, start.y, end.x, end.y, Pixel(0, 255, 255));
            FillCircle(end.x, end.y, rad, Pixel(0, 255, 255));
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
      //draw buttons
        shared_ptr<bin<Button>> currentButton = UI.currentPage->item->pageButtons.head;//this is getting set as nullptr
        while (currentButton != nullptr) {

            if (UI.currentPage->item->type == PAGE_TYPE::editSim && currentButton->item->type == BUTTON_TYPE::handle) {//draw handles
                //draw handle on pointed objects velocity (x10?)
                //cast to object handle
                iVector pos;
                iVector wid;
                shared_ptr<ScreenObject> temp = std::dynamic_pointer_cast<ObjectHandle>(currentButton->item)->item;//pointer access violation
                if (temp != nullptr) {
                    currentButton->item->position = viewport->spaceTranslate(temp->physicsBody->position, dVector());

                    if (currentButton->itemID != UI.activeButton) {
                        pos = currentButton->item->position;
                    }
                    else {
                        //lock to mouse
                        currentButton->item->position = iVector(GetMouseX(), GetMouseY());
                        pos = currentButton->item->position;
                    }

                    wid = currentButton->item->size;
                    iVector widOffset = wid;
                    widOffset.divide(2);
                    DrawRect(pos.x - widOffset.x, pos.y - widOffset.y, wid.x, wid.y);
                }
            }
            currentButton = currentButton->next;
        }
        //draw rendered path, relatve to anything?
        shared_ptr<bin<dVector>> currentCoord = UI.renderedPaths.head;
        while (currentCoord != nullptr) {
            Pixel color = colorList[(UI.currentPage->item->staticObjects.getBinByID(ScreenObject(), currentCoord->itemID)->itemID) % colorListLength];
            //Pixel color = olc::WHITE;
            //itemIDs are overwritten to match the object its the path of
            iVector screenCoord = viewport->spaceTranslate(*currentCoord->item);//use offset to make relative to a selected object?
            DrawCircle(screenCoord.x, screenCoord.y, 4, color);
            currentCoord = currentCoord->next;
        }

    }

    list<dVector> renderPaths(int depth = 15, int step = 5, int relativeID = -1) {
        list<dVector> paths = list<dVector>();
        dVector origin = dVector();
        if (relativeID >= 0) {
            origin = UI.currentPage->item->dynamicBodies.getItemByID(body(), relativeID)->position;
        }
        for (int i = 0; i < depth; i++) {
            shared_ptr<bin<body>> currentBody = UI.currentPage->item->dynamicBodies.head;
            dVector offset = dVector();
           
            //do physics step times
            for (int p = 0; p < step; p++) {
                solver.step(UI.currentPage->item->dynamicBodies);
            }
            if (relativeID >= 0) {
                offset = UI.currentPage->item->dynamicBodies.getItemByID(body(), relativeID)->position;

            }
            //run through visible physics objects and pop vertex on that spot
            while (currentBody != nullptr) {
                paths.add(currentBody->item->position, currentBody->item->linkedObject);
                paths.tail->item->subtract(offset);
                paths.tail->item->add(origin);
                //paths.tail->itemID = currentBody->item->linkedObject;//override for color coding on draw
                currentBody = currentBody->next;
            }
        }

        return paths;
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
