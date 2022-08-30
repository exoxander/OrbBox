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

public:
    bool OnUserCreate() override {
        PhysicsBody* p = new PhysicsBody();
        p->position = coord(100, 100);
        bodyArray[0] = p;
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override {///something keeps deleting the fucking pointers to data
        // called once per frame
        //for (int x = 0; x < ScreenWidth(); x++)
            //for (int y = 0; y < ScreenHeight(); y++)
                //Draw(x, y, olc::Pixel(60,60,60));
        //drawing mesh to view
        drawMesh(bodyArray[0]);
        //finished

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
                int x = p->getPolygonList()->b->position.x;
                int y = p->getPolygonList()->b->position.y;
                //A
                FillCircle(int((p->translate(x, m.polygonList->a->position.x)).x), int((p->translate(y, m.polygonList->a->position.y)).y), 10);
                //B
                FillCircle(int((p->translate(x, m.polygonList->b->position.x)).x), int((p->translate(y, m.polygonList->b->position.y)).y), 10);
                //C
                FillCircle(int((p->translate(x, m.polygonList->c->position.x)).x), int((p->translate(y, m.polygonList->c->position.y)).y), 10);
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
