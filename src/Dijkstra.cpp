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
    
    int lvn = OMIT_VALUE-1;
    
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




void Dijkstra::iterate() {
    
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
                
                if ( v < OMIT_VALUE) {
                    
                    int lvn = OMIT_VALUE-1;
                    
                    lvn = MIN(lvn, graph[(x-1)+y*size]);
                    lvn = MIN(lvn, graph[(x+1)+y*size]);
                    lvn = MIN(lvn, graph[(x-1)+(y+1)*size]);
                    lvn = MIN(lvn, graph[(x+1)+(y+1)*size]);
                    lvn = MIN(lvn, graph[(x-1)+(y-1)*size]);
                    lvn = MIN(lvn, graph[(x+1)+(y-1)*size]);
                    lvn = MIN(lvn, graph[x+(y-1)*size]);
                    lvn = MIN(lvn, graph[x+(y+1)*size]);
                    
                    if (abs(v-lvn)>=2) {
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

void Dijkstra::setWorldCenter(int x, int y) {
    tl.x = MIN(511-size,MAX(0,x-size/2));
    tl.y = MIN(511-size,MAX(0,y-size/2));

}

void Dijkstra::placeWorldGoal(int x, int y, int value) {
    
    int wx = x - tl.x;
    int wy = y - tl.y;
    
    if (wx>=0 && wx<size && wy>=0 && wy<size) {
        graph[wx+wy*size] = value;
    }
}

void Dijkstra::init() {
    for (int y = 0; y<size; y++) {
        for (int x = 0; x<size; x++) {
            int mx = x + tl.x;
            int my = y + tl.y;
            
            int debt = (int)engine.map->traversable(mx, my);
            
            if (debt==0) {
                graph[x+y*size] = OMIT_VALUE;
            } else {
                graph[x+y*size] = debt;
            }
            
        }
    }
}


void Dijkstra::process() {
    
    init();
    setBasis();
    iterate();
    
}

void Dijkstra::setBasis() {
    placeWorldGoal(goal.x, goal.y);
    //graph[(size/2)+(size/2) * size] = 1000;
}


void Dijkstra::retreat() {

    for (int y = 0; y<size; y++) {
        for (int x = 0; x<size; x++) {
            if ((y==0)||(y==size-1)||(x==0)||(x==size-1)) {
                graph[x+y*size] = OMIT_VALUE;
            } else {
                int v = graph[x+y*size];
                if ( v < OMIT_VALUE) {
                    graph[x+y*size] = ((v*-6)/5);
                }
            }
        }
    }


    iterate();

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
            
            if (graph[x+y*size] == OMIT_VALUE) {
                p.c = toascii('X');
            };
            
            console.setPixel(s, p);
            
        }
    }
}


int Dijkstra::graph[128*128];

ofVec2i Dijkstra::tl;
ofVec2i Dijkstra::goal;

