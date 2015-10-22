//
//  Dijkstra.cpp
//  matador
//
//  Created by vesabios on 10/21/15.
//
//

#include "Dijkstra.hpp"
#include "Core.h"


ofVec2i Dijkstra::getMoveVector() {
    
    int v = graph[(size/2)+(size/2) * size];
    
    int lvn = 9999;
    
    int x = size/2;
    int y = size/2;
    
    
    ofVec2i moveVector;
    if (lvn > graph[(x-1)+(y+1)*size]) {
        moveVector = ofVec2i(-1,1);
        lvn = graph[(x-1)+(y+1)*size];
        
    }
    if (lvn > graph[(x+1)+(y+1)*size]) {
        moveVector = ofVec2i(1,1);
        lvn = graph[(x+1)+(y+1)*size];
        
    }
    if (lvn > graph[(x-1)+(y-1)*size]) {
        moveVector = ofVec2i(-1,-1);
        lvn = graph[(x-1)+(y-1)*size];
        
    }
    if (lvn > graph[(x+1)+(y-1)*size]) {
        moveVector = ofVec2i(1,-1);
        lvn = graph[(x+1)+(y-1)*size];
        
    }
    
    
    if (lvn >= graph[(x-1)+y*size]) {
        moveVector = ofVec2i(-1,0);
        lvn = graph[(x-1)+y*size];
        
    }
    if (lvn >= graph[(x+1)+y*size]) {
        moveVector = ofVec2i(1,0);
        lvn = graph[(x+1)+y*size];
        
    }

    if (lvn >= graph[x+(y-1)*size]) {
        moveVector = ofVec2i(0,-1);
        lvn = graph[x+(y-1)*size];
        
    }
    if (lvn >= graph[x+(y+1)*size]) {
        moveVector = ofVec2i(0,1);
        lvn = graph[x+(y+1)*size];
        
    }

    return moveVector;
    
}

void Dijkstra::process() {
    
    // init
    for (int y = 0; y<size; y++) {
        for (int x = 0; x<size; x++) {
            int mx = x + tl.x;
            int my = y + tl.y;
            
            int debt = (int)core->map->traversable(mx, my);
            
            if (debt==0) {
                graph[x+y*size] = 10000;
            } else {
                graph[x+y*size] = debt;
            }
            
        }
        
    }
    
    
    
    // place goals
    ofLog() << "goal: " << goal.x << " " << goal.y;
    
    placeWorldGoal(goal.x, goal.y);
    
    graph[(size/2)+(size/2) * size] = 1000;
    ofLog() << "tl: " << tl.x << " " << tl.y;

    
    // iterate through graph
    bool cont = true;
    bool changed = false;
    int failsafe = 0;
    while (cont && failsafe<400) {
        changed = false;
        failsafe++;
        for (int y = 1; y<size-1; y++) {
            for (int x = 1; x<size-1; x++) {
                
                int v =  graph[x+y*size];
                
                if ( v < 10000) {
                    
                    int lvn = 9999;
                    
                    lvn = MIN(lvn, graph[(x-1)+y*size]);
                    lvn = MIN(lvn, graph[(x+1)+y*size]);
                    lvn = MIN(lvn, graph[(x-1)+(y+1)*size]);
                    lvn = MIN(lvn, graph[(x+1)+(y+1)*size]);
                    lvn = MIN(lvn, graph[(x-1)+(y-1)*size]);
                    lvn = MIN(lvn, graph[(x+1)+(y-1)*size]);
                    lvn = MIN(lvn, graph[x+(y-1)*size]);
                    lvn = MIN(lvn, graph[x+(y+1)*size]);
                    
                    if (v-lvn>=2) {
                        graph[x+y*size]= lvn+1;
                        changed = true;
                    }
                }
            }
        }
        if (!changed) {
            cont = false;
        }
    }
 
 
     
     
}


void Dijkstra::render() {
    
    for (int y = 0; y<size; y++) {
        for (int x = 0; x<size; x++) {
            
            int wx = tl.x+x;
            int wy = tl.y+y;
            
            ofVec2i s = core->worldToWindow(ofVec2i(wx,wy));
            
            Pixel p;
            
            BYTE r = (graph[x+y*size])%6;
            BYTE g = (graph[x+y*size]/6)%6;
            BYTE b = (graph[x+y*size]/36)%6;
            
            p.fg = makeColor(5,5,5);
            p.bg = makeColor(r,g,b);
            p.c = ' ';
            
            if (graph[x+y*size] == 10000) {
                p.c = toascii('X');
            };
            
            console.setPixel(s, p);
            
        }
    }
}