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
    
        
    fontsheet.loadImage("vesa.bmp");
    fontsheet.getTextureReference().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    fontsheet.getTextureReference().setTextureWrap(GL_ZERO, GL_ZERO);
    
    tex.allocate(80,50,GL_RGBA);
    
    shader.load("shaders/s.vert", "shaders/s.frag");
    shader.setUniformTexture("colorMap", fontsheet, 1 );
    shader.setUniformTexture("font", fontsheet, 2 );
}

//--------------------------------------------------------------
void Console::setPixel(ofVec2f pp, Pixel p) {
    setPixel(pp.x, pp.y, p.fg, p.bg, p.c);
    
}

//--------------------------------------------------------------
void Console::setPixel(BYTE x, BYTE y, BYTE f, BYTE b, BYTE c) {
    
    if (x>=0 && y>=0 && x<80 && y<50) {
        int idx = ((y*80)+x)*4;
        
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
void Console::writeString(int x, int y, string s) {
    
    int sz = s.length();
    
    
    for (int i=0; i<sz; i++) {
        console.setPixel(x+i, y, makeColor(5,5,2), makeColor(0,2,2), s.at(i));
    }
    
    
}

//--------------------------------------------------------------
void Console::writeString(int x, int y, string s, BYTE fg, BYTE bg) {
    
    int sz = s.length();
    
    
    for (int i=0; i<sz; i++) {
        console.setPixel(x+i, y, fg, bg, s.at(i));
    }
    
}


//--------------------------------------------------------------
void Console::drawBox(ofRectangle r, BYTE bg) {
    
    for (int y=r.getMinY(); y<r.getMaxY(); y++) {
        for (int x=r.getMinX(); x<r.getMaxX(); x++) {
            setPixel(x, y, 0, bg, 0);
        }
    }
    
}



//--------------------------------------------------------------
void Console::render() {
    
    tex.loadData(pixels, 80, 50, GL_RGBA);
    
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
    glTexCoord2f(80.f, 0.f);      glVertex3i(0.f + dim.x, 0.f, 0);
    glTexCoord2f(80.f, 50.f);     glVertex3i(0.f + dim.x, 0.f + dim.y, 0);
    glTexCoord2f(0.f,  50.f);     glVertex3i(0.f,        0.f + dim.y, 0);
    glEnd();
    shader.end();
    
    glDisable(tex.texData.textureTarget);
    
}