//
//  MStoneWall.hpp
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef MStoneWall_h
#define MStoneWall_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"
#include "Material.h"


class MStoneWall : public Material {
    
public:
    
    int cLUT[8] ={
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER1,
        CHAR_DITHER1,
        CHAR_DITHER0,
        CHAR_DITHER0
    };
    
    int fgLUT[8][3]= {
        {1,  1,  2}, // memory
        {2,  2,  2},
        {2,  2,  2},
        {2,  2,  2},
        {2,  2,  2},
        {3,  3,  3},
        {3,  3,  3},
        {4,  4,  4}
    };
    
    int bgLUT[8][3]= {
        {0,  0,  0}, // memory
        {0,  0,  0},
        {0,  0,  0},
        {0,  0,  0},
        {0,  0,  0},
        {0,  0,  0},
        {0,  0,  0},
        {0,  0,  0}
    };
    
    
    MStoneWall(MaterialType mt) : Material(mt) {}
    string getName() override { return "Stone Wall"; }
    
    bool isOpaque() override { return true; }
    DEBT traversable() override { return TRAVERSE_BLOCKED; }
    Pixel operator()(ofVec2i pos, float luma) override {
        
        Pixel p;
        char l = 0;
        
        if (luma>0) {
            float noise = (ofNoise(pos.x, pos.y)*0.2f) + 0.8f;
            char lm = dither(pos.x, pos.y, luma * noise * 9.0f);
            l = MAX(MIN(lm-1,7),0);
        }

        p.fg = makeColor(fgLUT[l][0],fgLUT[l][1],fgLUT[l][2]);
        p.bg = makeColor(bgLUT[l][0],bgLUT[l][1],bgLUT[l][2]);
        p.c = cLUT[l];
        
        return p;
        
    };
    
    
};

#endif /* MStoneWall_h */
