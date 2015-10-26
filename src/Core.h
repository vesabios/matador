#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofVec2i.h"
#include "Serializer.hpp"
#include "Map.h"
#include "Material.h"
#include "ObjectFactory.h"
#include "MapData.h"
#include "Player.h"
#include "Defines.h"
#include "Engine.h"
#include "Paint.h"
#include "Inspect.h"
#include "Equip.h"
#include "EventLog.h"
#include "VFX.hpp"
#include "Dijkstra.hpp"
#include "Dialog.hpp"



class Actor;
class Engine;
class Console;
class Player;

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
        bool fire = false;
        bool tab = false;
        
        public: bool active() {
            return up || down || left || right || t || rest || space || fire;
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
    
    void actorEvent(ActorEvent &e);


    
    GameState state;

    Map *map;
    

    vector<ofVec2i> trail;

    bool torch = true;
    
    bool debugGraph = false;
    
    
    bool resolvingTurn = false;

    float torchBrightness = 0.0f;
    float visionRadius = 0.0f;
    BYTE isVisible(const ofVec2i pos);

    DEBT traversable(int dx, int dy);
    
    float isOpaque(int x, int y, float currentOpaque);

    void raytrace(int x0, int y0, int x1, int y1);
    bool losCheck(Object * a, Object * b);

    void transitActor(Actor* a, int x, int y, int z);

    void adjustWindow();
    ofVec2i worldToWindow(const ofVec2i p);
    ofVec2i windowToWorld(const ofVec2i p);
    void writeString(int x, int y, string s, BYTE fg, BYTE bg);
    void writeString(int x, int y, string s);
    void createRoom(int x, int y, int w, int h);
    
    void reset();
    
    vector<Object*> updateList;
    int updateDebt = 0;
    int updateIndex = 0;
    float resolveDelay = 0;
    
    void resolveTurn();
    
    void populateMap();
    
    bool gameplayState();
    
    Object * placeObject(int x, int y, int mapNumber, Object::ObjectType it);
    
    void renderWorld();
    
    ofFmodSoundPlayer sound;

    uint32_t world[CONSOLE_WIDTH*CONSOLE_HEIGHT];

    BYTE vis[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    
    Player * player;
    
    
    Material::MaterialType currentMaterial = (Material::MaterialType)0;
    
    MapData * mapData[256];
    
    
    std::vector<Actor *> firingList;
    int firingIndex = 0;
    void toggleFiring();
    bool fireTargeting = false;
    
    

    ofVec2i cursorPos;
    ofVec2i mousePos;

    float reticleTime = 0.0f;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    ofVec2i flickerOffset;
    float flickerValue;

    ctrls controls;

    //vector<Actor> actors;

    DWORD currentObject = 0;

    int actionDebt = 0;
    DEBT newPlayerDebt = 0;
    
    ofxPanel gui;

    bool controlsHaveBeenActive = false;
    
    ofxFloatSlider  aFreq;
    ofxFloatSlider  aAmp;
    ofxFloatSlider  aOffset;
    ofxFloatSlider  bFreq;
    ofxFloatSlider  bAmp;
    ofxFloatSlider  bOffset;
    ofxFloatSlider  cFreq;
    ofxFloatSlider  cAmp;
    ofxFloatSlider  cOffset;
    


};


extern Core * core;



