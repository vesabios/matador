#pragma once

#include "ofMain.h"
#include "ofVec2i.h"
#include "Serializer.hpp"
#include "Map.h"
#include "Material.h"
#include "ObjectFactory.h"
#include "Items.h"
#include "Player.h"
#include "Defines.h"
#include "Engine.h"
#include "Paint.h"
#include "Inspect.h"







class Actor;
class Engine;
class Console;

class Core : public ofBaseApp {
    
    


public:
 
    struct ctrls {

        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        bool t = false;
        bool rest = false;
        bool space = false;
        
        public: bool active() {
            return up || down || left || right || t || rest || space;
        }

    
    };

    void setup();
    void update();
    void draw();


    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    float ditherValue(int x, int y, float c0);


    
    GameState state;

    Map *map;

    vector<ofVec2i> trail;

    bool torch = true;

    float torchBrightness = 0.0f;
    float visionRadius = 0.0f;
    bool isVisible(const ofVec2i pos);

    DEBT traversable(int dx, int dy);
    
    bool isOpaque(int x, int y);

    void raytrace(int x0, int y0, int x1, int y1);

    void adjustWindow();
    ofVec2i worldToWindow(const ofVec2i p);
    ofVec2i windowToWorld(const ofVec2i p);
    void writeString(int x, int y, string s, BYTE fg, BYTE bg);
    void writeString(int x, int y, string s);
    void createRoom(int x, int y, int w, int h);
    
    void populateMap();
    
    Object * placeObject(int x, int y, int mapNumber, Object::ObjectType it);
    
    void renderWorld();
    
    ofFmodSoundPlayer sound;

    uint32_t world[80*50];

    BYTE vis[80*50];
    
    Player * player;
    
    
    MaterialType currentMaterial = (MaterialType)0;
    
    

    ofVec2i cursorPos;
    ofVec2i mousePos;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    ofVec2i flickerOffset;
    float flickerValue;

    ctrls controls;

    //vector<Actor> actors;

    DWORD currentObject = 0;

    int playerMvt = 0;





};

extern Core * core;



