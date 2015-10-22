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

/*
 
 To get a Dijkstra map, you start with an integer array representing your map, with some set of goal cells set to zero and all the rest set to a very high number. Iterate through the map's "floor" cells -- skip the impassable wall cells. If any floor tile has a value that is at least 2 greater than its lowest-value floor neighbor, set it to be exactly 1 greater than its lowest value neighbor. Repeat until no changes are made. The resulting grid of numbers represents the number of steps that it will take to get from any given tile to the nearest goal.
 
 */


class Dijkstra {
public:
    
    int size = 128;
    
    int graph[128*128];
    
    ofVec2i tl;
    ofVec2i goal;
    
    
    void setWorldCenter(int x, int y) {
        tl.x = MIN(511-size,MAX(0,x-size/2));
        tl.y = MIN(511-size,MAX(0,y-size/2));
    }
    
    void placeWorldGoal(int x, int y) {
        
        int wx = x - tl.x;
        int wy = y - tl.y;
       
        if (wx>=0 && wx<size && wy>=0 && wy<size) {
            graph[wx+wy*size] = 0;
        }
    }
    
    ofVec2i getMoveVector();
    
    void process();
    void render();
    
    
};
#endif /* Dijkstra_hpp */
