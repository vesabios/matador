//
//  Engine.hpp
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef Engine_h
#define Engine_h

#include <stdio.h>
#include "Serializer.hpp"
#include "Map.h"
#include "MapData.h"
#include "Player.h"

class Engine {
    
public:
    void init();
    void actorEvent(ActorEvent &e);
    void reset();
    void resolveTurn();
    void toggleFiring();
    void transitActor(Actor* a, int x, int y, int z);
    void raytrace(int x0, int y0, int x1, int y1);
    BYTE isVisible(const ofVec2i pos);
    float isOpaque(int x, int y, float currentOpaque);
    bool losCheck(Object * a, Object * b);
    float calcLuma(int x, int y);
    DEBT traversable(int dx, int dy);
    void preRender();
    void setFullVisibility();
    void renderWorld();
    void update(float deltaTime);
    
/*
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
 */
    
    

    // major pointers
    Player * player;
    Map *map;
    MapData * mapData[256];
    

    // game turns
    int actionDebt = 0;
    DEBT newPlayerDebt = 0;
    vector<Object*> updateList;
    int updateDebt = 0;
    int updateIndex = 0;
    float resolveDelay = 0;
    bool resolvingTurn = false;
   
    
    // firing
    bool fireTargeting = false;
    std::vector<Actor *> firingList;
    int firingIndex = 0;
    float reticleTime = 0.0f;
    
    
    // light & rendering
    
    BYTE vis[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    
    float ambient = 1.0f;
    bool torch = true;
    float torchBrightness = 0.0f;
    float visionRadius = 0.0f;
    ofVec2i flickerOffset;
    float flickerValue;
    
    
    
};


extern Engine engine;


#endif /* Engine_h */
