#define OLC_PGE_APPLICATION //lol, dont forget this one, lots of awful linker build errors
#include <iostream>
#include "olcPixelGameEngine.h"
#include "PhysicsBody.h"
#include "Utility.h"

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
    olc::Pixel colorList[4] = { olc::WHITE, olc::BLUE, olc::GREEN, olc::RED };
    Utility u;

public:
    bool OnUserCreate() override {
        PhysicsBody* p = new PhysicsBody();
        p->position = coord(200, 200);
        p->setMesh(p->generateCircle(30));
        bodyArray[0] = p;
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame

        //fill screen with color
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(10,10,20));
        //drawing mesh to view
        drawMesh(bodyArray[0],false);
        //finished
        //bodyArray[0]->position.add(coord(10, 0));

        return true;
    }

    void drawMesh(PhysicsBody* p, bool filled = false) {
        mesh m = p->getMesh();
        if (filled) {

            //loop through all polygons and draw to screen
            shared_ptr<polygon> current = m.polygonList;
            for (int i = 0; i < m.pgonLength; i++) {
                coord a = current->a->position;
                coord b = current->b->position;
                coord c = current->c->position;
                coord parent = p->position;

                //make color result of % between polygon id and color list
                olc::Pixel polygonColor = colorList[current->id%4];
                FillTriangle(
                    int((u.translate(parent, a)).x), int((u.translate(parent, a)).y),//A
                    int((u.translate(parent, b)).x), int((u.translate(parent, b)).y),//B
                    int((u.translate(parent, c)).x), int((u.translate(parent, c)).y),//C
                    polygonColor);
                //int((u.translate(parent, a)).x), int((u.translate(parent, b)).y)
            }
        }
        else {
            //draw circles on vertecies
            shared_ptr<vertex> current = p->getMesh().vertexList;
            for (int i = 0; i < p->getMesh().vexLength; i++) {
                if (current != nullptr) {
                    //make circle
                    olc::Pixel vertexColor = colorList[current->id%4];
                    FillCircle(int((u.translate(p->position, current->position)).x), int((u.translate(p->position, current->position)).y), vertexDrawScale, vertexColor);
                    current = current->next;
                }
                else {
                    //throw an error or something idk
                    std::cout << "vexlength exceeded actual" << std::endl;
                }    
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
