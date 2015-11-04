//
//  Console.cpp
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#include "Console.h"

Console console;


//--------------------------------------------------------------
void Console::init() {
    
        
    fontsheet.loadImage("vesa.png");
    fontsheet.getTextureReference().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    fontsheet.getTextureReference().setTextureWrap(GL_ZERO, GL_ZERO);
    
    tex.allocate(CONSOLE_WIDTH,CONSOLE_HEIGHT,GL_RGBA);
    
    shader.load("shaders/s.vert", "shaders/s.frag");
    shader.setUniformTexture("colorMap", fontsheet, 1 );
    shader.setUniformTexture("font", fontsheet, 2 );
}

//--------------------------------------------------------------
Pixel Console::getPixel(const BYTE x, const BYTE y) {
    Pixel p;

    if (x>=0 && y>=0 && x<CONSOLE_WIDTH && y<CONSOLE_HEIGHT) {
        
        int idx = ((y*CONSOLE_WIDTH)+x)*4;
        
        p.fg = pixels[idx];
        
        // background color
        p.bg = pixels[idx+1];
        
        // character
        p.c = pixels[idx+2];
        
    }
    
    return p;
}

//--------------------------------------------------------------
void Console::setPixel(const ofVec2i pp, const Pixel p) {
    setPixel(pp.x, pp.y, p);
    
}


//--------------------------------------------------------------
void Console::setPixel(const BYTE x, const BYTE y, const Pixel p) {
    
    if (x>=0 && y>=0 && x<CONSOLE_WIDTH && y<CONSOLE_HEIGHT) {
        int idx = ((y*CONSOLE_WIDTH)+x)*4;
        
        if (p.a!=1) {
            pixels[idx] = p.fg;
        }
        

        if (p.a>0) {
            pixels[idx+1] = p.bg;
        }
        
        // character
        
        
        if (p.a!=1) {
            pixels[idx+2] = p.c;
        }
        
        // not used
        pixels[idx+3] = p.a;
        
    }
    
}
//--------------------------------------------------------------
void Console::setPixel(const BYTE x, const BYTE y, const BYTE f, const BYTE b, const BYTE c) {
    
    if (x>=0 && y>=0 && x<CONSOLE_WIDTH && y<CONSOLE_HEIGHT) {
        int idx = ((y*CONSOLE_WIDTH)+x)*4;
        
        pixels[idx] = f;
        
        // background color
        pixels[idx+1] = b;
        
        // character
        pixels[idx+2] = c;
        
        // not used
        pixels[idx+3] = 0;
        
    }
    
}

//--------------------------------------------------------------
void Console::writeString(const int x, const int y, const string s, const BYTE fg) {
    writeString(x,y,fg,s);
}

void Console::writeString(const int x, const int y, const BYTE fg, const string s) {
    
    int sz = s.length();
    
    
    for (int i=0; i<sz; i++) {
        Pixel p;
        p.fg = fg;
        p.bg = 0;
        p.a = 0;
        p.c = s.at(i);
        console.setPixel(x+i, y, p);
        
        // console.setPixel(x+i, y, makeColor(5,5,2), makeColor(0,2,2), s.at(i));
    }
    
    
}



//--------------------------------------------------------------

void Console::writeString(const int x, const int y, const string s) {
    
    int sz = s.length();
    
    
    for (int i=0; i<sz; i++) {
        Pixel p;
        p.fg =makeColor(3,3,2);
        p.bg = 0;
        p.a = 0;
        p.c = s.at(i);
        console.setPixel(x+i, y, p);
    
       // console.setPixel(x+i, y, makeColor(5,5,2), makeColor(0,2,2), s.at(i));
    }
    
    
}



//--------------------------------------------------------------
void Console::writeString(const int x, const int y, const string s, const BYTE fg, const BYTE bg) {
    
    int sz = s.length();
    
    
    for (int i=0; i<sz; i++) {
        console.setPixel(x+i, y, fg, bg, s.at(i));
    }
    
}


//--------------------------------------------------------------
void Console::drawBox(const ofRectangle r, const BYTE bg) {
    
    for (int y=r.getMinY(); y<r.getMaxY(); y++) {
        for (int x=r.getMinX(); x<r.getMaxX(); x++) {
            setPixel(x, y, 0, bg, 0);
        }
    }
    
}



//--------------------------------------------------------------
void Console::render() {
    
    tex.loadData(pixels, CONSOLE_WIDTH, CONSOLE_HEIGHT, GL_RGBA);
    
    glEnable(tex.texData.textureTarget);
    glTexParameterf(tex.texData.textureTarget, GL_TEXTURE_WRAP_S, GL_ZERO);
    glTexParameterf(tex.texData.textureTarget, GL_TEXTURE_WRAP_T, GL_ZERO);
    glTexParameteri(tex.texData.textureTarget, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(tex.texData.textureTarget, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    
    shader.begin();
    
    shader.setUniformTexture("colorMap", tex, 1 );
    shader.setUniformTexture("font", fontsheet.getTextureReference(), 2 );
    
    glBegin( GL_QUADS );
    
    
    ofPoint dim = ofGetWindowSize();

    
    glTexCoord2f(0.f,  0.f);      glVertex3i(0.f,        0.f, 0);
    glTexCoord2f(CONSOLE_WIDTH, 0.f);      glVertex3i(0.f + dim.x, 0.f, 0);
    glTexCoord2f(CONSOLE_WIDTH, CONSOLE_HEIGHT);     glVertex3i(0.f + dim.x, 0.f + dim.y, 0);
    glTexCoord2f(0.f,  CONSOLE_HEIGHT);     glVertex3i(0.f,        0.f + dim.y, 0);
    glEnd();
    shader.end();
    
    glDisable(tex.texData.textureTarget);
    
}