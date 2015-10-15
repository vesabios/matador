//
//  Console.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef Console_h
#define Console_h

#include "ofMain.h"
#include "Defines.h"
#include <stdio.h>


class Console {
    
    ofTexture tex;
    ofShader shader;
    ofImage fontsheet;
    
public:
    BYTE pixels[80*50*4];

    void init();
    
    void drawBox(ofRectangle r, BYTE bg);
    
    void setPixel(ofVec2f pp, Pixel p);

    void setPixel(BYTE x, BYTE y, BYTE f, BYTE b, BYTE c);
    void writeString(int x, int y, string s, BYTE fg, BYTE bg);
    void writeString(int x, int y, string s);
    
    
    
    void render();
    
};

extern Console console;


#endif /* Console_h */
