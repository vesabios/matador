//
//  Dijkstra.hpp
//  matador
//
//  Created by vesabios on 10/21/15.
//
//

#ifndef Dijkstra_hpp
#define Dijkstra_hpp

#include "ofTypes.h"
#include "ofVec2i.h"
#include "ofLog.h"


class Dijkstra {
public:
    
    int size = 128;
    
    static int graph[128*128];
    
    static ofVec2i tl;
    static ofVec2i goal;
    
    void setWorldCenter(int x, int y);
    void placeWorldGoal(int x, int y, int value = 0);
    void retreat();
    void iterate();

    ofVec2i getMoveVector();
    
    void init();
    void setBasis();
    void process();
    void render();
    
    
};
#endif /* Dijkstra_hpp */
