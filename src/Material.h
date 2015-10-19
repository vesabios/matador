//
//  Material.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef Material_h
#define Material_h

#include <stdio.h>
#include "Console.h"
#include "ofxGui.h"


static int BAYER8X8[8][8] = {
    { 0, 32, 8, 40, 2, 34, 10, 42},
    {48, 16, 56, 24, 50, 18, 58, 26},
    {12, 44, 4, 36, 14, 46, 6, 38},
    {60, 28, 52, 20, 62, 30, 54, 22},
    { 3, 35, 11, 43, 1, 33, 9, 41}, 
    {51, 19, 59, 27, 49, 17, 57, 25},
    {15, 47, 7, 39, 13, 45, 5, 37},
    {63, 31, 55, 23, 61, 29, 53, 21} };


class Material {
    
public:
    
    enum MaterialType {
        Void,
        StoneWall,
        StoneFloor,
        Algae,
        GreenGrass,
        DenseForest
    };
    
    ofxFloatSlider * aFreq;
    ofxFloatSlider * aAmp;
    ofxFloatSlider * aOffset;
    ofxFloatSlider * bFreq;
    ofxFloatSlider * bAmp;
    ofxFloatSlider * bOffset;
    ofxFloatSlider * cFreq;
    ofxFloatSlider * cAmp;
    ofxFloatSlider * cOffset;
    
    
    static char dither(const int x, const int y, const float c0) {
        
        float v = c0;
        char a = 0;
        while(v>1.0f) {
            v-=1.0f;
            a+=1;
        }
        
        float limit = float(BAYER8X8[x%8][y%8]+1) / 64.0f;
        
        if (v<limit) {
            return a;
        } else {
            return a+1;
        }
        
    }
    
    
    Material(Material::MaterialType t) : mt(t) {};
    Material::MaterialType mt;
    
    virtual string getName() = 0;
    virtual float isOpaque() = 0;
    virtual DEBT traversable() = 0;
    virtual Pixel operator()(ofVec2i pos, float luma) = 0;
    
    
};
#endif /* Material_h */




