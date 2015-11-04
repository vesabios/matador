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
    BYTE pixels[CONSOLE_WIDTH*CONSOLE_HEIGHT*4];

    void init();
    
    void drawBox(const ofRectangle r, const BYTE bg);
    
    void setPixel(const ofVec2i pp, const Pixel p);
    void setPixel(const BYTE x, const BYTE y, const BYTE f, const BYTE b, const BYTE c);
    void setPixel(const BYTE x, const BYTE y, const Pixel p);

    Pixel getPixel(const BYTE x, const BYTE y);
    void writeString(const int x, const int y, const string s, const BYTE fg, const BYTE bg);
    void writeString(const int x, const int y, const BYTE fg, const string s);
    void writeString(const int x, const int y, const string s, const BYTE fg);

    void writeString(const int x, const int y, const string s);
    
    void drawSubMenu();

    
    void render();
    
};

extern Console console;


#endif /* Console_h */
