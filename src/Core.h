#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofVec2i.h"
#include "Material.h"
#include "ObjectFactory.h"

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
            return up || down || left || right || t || rest || space || fire || tab;
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

    

    vector<ofVec2i> trail;
    
    bool playing = true;
    bool debugGraph = false;



    void adjustWindow();


    ofVec2i worldToWindow(const ofVec2i p);
    ofVec2i windowToWorld(const ofVec2i p);
    
    
    
    bool gameplayState();
    
    Object * placeObject(int x, int y, int mapNumber, Object::ObjectType it);
    
    
    ofFmodSoundPlayer sound;

    uint32_t world[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    
    Material::MaterialType currentMaterial = (Material::MaterialType)0;
    
    

    ofVec2i cursorPos;
    ofVec2i mousePos;
    bool mouseHide = false;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    ctrls controls;

    DWORD currentObject = 0;

    
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



