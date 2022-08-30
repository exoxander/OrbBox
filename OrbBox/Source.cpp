#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "object.h"

//const float TickRate = 16;

//https://github.com/OneLoneCoder/olcPixelGameEngine/wiki





class Engine : public olc::PixelGameEngine {

public:
    Engine() {
        sAppName = "Orb_Box";
    }

private:
    PhysicsBody* bodyArray[3];
    int vertexDrawScale = 4;

public:
    bool OnUserCreate() override {
        PhysicsBody* p = new PhysicsBody();
        p->position = coord(200, 200);
        bodyArray[0] = p;
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override {///something keeps deleting the fucking pointers to data
        // called once per frame

        //fill with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        drawMesh(bodyArray[0]);
        //finished
        //bodyArray[0]->position.add(coord(10, 0));

        return true;
    }

    void drawMesh(PhysicsBody* p, bool filled = false) {
        mesh m = p->getMesh();
        if (filled) {
            //do nothing for now
        }
        else {
            //draw circles on vertecies
            for (int i = 0; i < p->getMesh().pgonLength; i++) {
                double x = p->position.x;
                double y = p->position.y;
                //A
                FillCircle(int((p->translate(p->position, m.polygonList->a->position)).x), int((p->translate(p->position, m.polygonList->a->position)).y), vertexDrawScale+4, olc::WHITE);
                //B
                FillCircle(int((p->translate(p->position, m.polygonList->b->position)).x), int((p->translate(p->position, m.polygonList->b->position)).y), vertexDrawScale, olc::BLUE);
                //C
                FillCircle(int((p->translate(p->position, m.polygonList->c->position)).x), int((p->translate(p->position, m.polygonList->c->position)).y), vertexDrawScale, olc::RED);
            }
        }
    }

};

int main()
{
    Engine window;
    if (window.Construct(256 * 4, 192 * 4, 1, 1)) {
        window.Start();
    }
    return 0;
}
