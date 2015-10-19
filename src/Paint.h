//
//  Paint.h
//  matador
//
//  Created by vesabios on 10/8/15.
//
//

#ifndef Paint_h
#define Paint_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"

class Paint {
    

    Axis orthoAxis = AXIS_NONE;
    
    bool visible = false;
    float menuPos = 100.0f;
    
    unsigned char pixels[CONSOLE_WIDTH*CONSOLE_HEIGHT*4];
    
    bool mouseDown = false;
    bool ortho = false;
    ofPoint startMousePoint;
    char brush = 0;

    
    
public:
    
    BYTE fg;
    BYTE bg;
    BYTE c;
    
    void show();
    void hide();
    
    BYTE fg_r = 0;
    BYTE fg_g = 0;
    BYTE fg_b = 0;
    BYTE bg_r = 0;
    BYTE bg_g = 0;
    BYTE bg_b = 0;
    
    
    void toggle();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);

    void brushPixels(ofVec2i p);
    void placePixel(ofVec2i p);

    void save();
    void load();
    
    void update(const float deltaTime);
    void render();
    
};



extern Paint paint;


#endif /* Paint_h */
