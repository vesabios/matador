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
    
    enum Axis {
        AXIS_NONE,
        AXIS_HORIZONTAL,
        AXIS_VERTICAL
    };
    
    Axis orthoAxis = AXIS_NONE;
    
    bool visible = false;
    float menuPos = 100.0f;
    
    unsigned char pixels[80*50*4];
    
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
