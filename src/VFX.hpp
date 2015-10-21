//
//  VFX.hpp
//  matador
//
//  Created by vesabios on 10/19/15.
//
//

#ifndef VFX_hpp
#define VFX_hpp

#include <stdio.h>
#include "Defines.h"
#include "Console.h"
#include "CombatEvent.h"


class VFXBase {
public:
    
    virtual ~VFXBase() {};
    
    ofVec2i p;
    bool done = false;
    float elapsed = 0;
    bool blocking = false;
    
    
    virtual void update(float dt) = 0;
    
};


class VFXArrow : public VFXBase {
    
public:
    
    ofVec2i pa;
    ofVec2i pb;
    
    
    bool inited = false;
    
    int frame = 0;
    
    
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;

    float angle;
    
    ofPtr<CombatEvent> nextEvent;
    
    void init() {
        
        
        
        blocking = true;
        
        
        dx = abs(pb.x-pa.x);
        sx = pa.x<pb.x ? 1 : -1;
        dy = abs(pb.y-pa.y);
        sy = pa.y<pb.y? 1 : -1;
        err = (dx>dy ? dx : -dy)/2;
        

        inited = true;
        
        ofVec2f va = ofVec2f(pa.x, pa.y);
        ofVec2f vb = ofVec2f(pb.x, pb.y);
       
        angle = atan2(va.y - vb.y, va.x - vb.x) + M_PI;
        angle = angle * 180.0f / M_PI;
        
        ofLog() << "ANGLE OF ATTACK: " << angle;
        
    }
    
    ~VFXArrow() override {}
 
    
    
    virtual void update(float dt) override {
        if (!inited) { init(); }
        
        frame++;
        
        Pixel pixel;
        pixel.fg = makeColor(5,5,5);
        pixel.bg = makeColor(0,0,0);
        
        
        int cr = 0;
        for (int i=0; i<9; i++) {
            if (angle < (i*45)+22.5) {
                cr = i;
                break;
            }
        }
        
        BYTE arr[9] = {133,253,134,252,133,253,134,252,133};

        pixel.c = arr[cr];
        pixel.a = 0;
        
        ofVec2i p = ofVec2i(pa.x, pa.y);
        
        console.setPixel(p, pixel);
        
        if (!done) {
            if (pa.x==pb.x && pa.y==pb.y) {
                done = true;
                
                if (nextEvent) {
                    ofNotifyEvent(CombatEvent::combatEvent, *nextEvent);
                }

            }
            e2 = err;
            if (e2 >-dx) { err -= dy; pa.x += sx; }
            if (e2 < dy) { err += dx; pa.y += sy; }
        }
 

    }
    
};



class VFXHit : public VFXBase {
public:
    
    ~VFXHit() override {}
    
    bool flip = false;
    int frame = 0;

    virtual void update(float dt) override {
        elapsed += dt;
        
        flip = !flip;
        
        frame++;
        
        //ofLog() << "rendering vfx";
        
        if (elapsed>0.15f) {
            done = true;
        }
        
        Pixel pixel;
        
        if (frame<3) {
            pixel.bg = makeColor(5,5,5);
            pixel.fg = makeColor(5,5,5);
            pixel.c = toascii(' ');
            pixel.a = 255;
            
        } else {
            if (flip) {
                pixel.fg = makeColor(5,4,3);
                pixel.bg = makeColor(1,frame/5,(frame/3));
            } else {
                pixel.fg = makeColor(1,0,0);
                pixel.bg = makeColor(MAX(0,5-(frame/3)),0,2);
            }
            
            double i;
            int f = (frame/2) % 4;
            if (f==3) {
                pixel.c = 225;
            } else if (f==2) {
                pixel.c = 226;
            } else if (f==1) {
                pixel.c = 176;
            } else {
                pixel.c = 227;
                
            }
        }
        
        if (frame>8) {
            pixel.a = 0;
        }
        
        

        
        //pixel.a = 0;
        
        console.setPixel(p, pixel);
        
        /*
        if (frame<2) {
            pixel.a = 255;
            pixel.c = toascii(' ');
            console.setPixel(p+ofVec2i(1,0), pixel);
            console.setPixel(p+ofVec2i(-1,0), pixel);
            console.setPixel(p+ofVec2i(0,1), pixel);
            console.setPixel(p+ofVec2i(0,-1), pixel);
            
        }
         */

    
    }
    
};




class VFXFloater : public VFXBase {
public:
    
    ~VFXFloater() override {}
    
    bool flip = false;
    int frame = 0;
    int dmg = 0;
    
    virtual void update(float dt) override {
        
        elapsed += dt;
        frame++;

        if (elapsed>0.45f) {
            done = true;
        }
        
        Pixel pixel;
        pixel.fg = makeColor(5,1,0);
        pixel.bg = 0;
        pixel.a = 0;
        
        int y = p.y - (elapsed*5.0f);
        
        console.writeString(p.x, y, makeColor(5,1,0), ofToString(dmg));
        
    }
    
};

class VFX {
    
public:
    
    void init();
    
    vector<VFXBase*> items;
    void combatEvent(CombatEvent &e);
    
    
    bool busy() {
        for (int i=0; i<items.size(); i++) {
            if (items[i]->blocking) return true;
        }
        return false;
    }

    void update(float deltaTime) {
        
        for (int i=0; i<items.size(); i++) {
            items[i]->update(deltaTime);
            //ofLog() << "VFX updating item: " <<i;
        }
        
        for (int i=0; i<items.size(); i++) {
            if (items[i]->done) {
                delete items[i];
                items.erase (items.begin()+i);
            }
            //i--;
        }
        
    }
    
    
};

extern VFX vfx;

#endif /* VFX_hpp */
