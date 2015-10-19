//
//  MStoneWall.hpp
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef MStoneFloor_h
#define MStoneFloor_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"
#include "Material.h"


class MStoneFloor : public Material {
    
public:
    
    int cLUT[8] ={
        CHAR_DITHER3,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2,
        CHAR_DITHER2
    };
    
    int fgLUT[8][3]= {
        {1,  1,  2}, // memory
        {1,  1,  1},
        {1,  1,  2},
        {1,  2,  2},
        {1,  2,  2},
        {2,  2,  2},
        {2,  2,  2},
        {2,  2,  2}
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
    
    MStoneFloor(Material::MaterialType mt) : Material(mt) {}
    string getName() override { return "Stone Floor"; }
    float isOpaque() override { return 0.0f; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    Pixel operator()(ofVec2i pos, float luma) override {
        
        Pixel p;
        char l = 0;
        
        if (luma>0) {
            float noise = (ofNoise(pos.x, pos.y)*0.6f) + 0.7f;
            char lm = dither(pos.x, pos.y, luma * noise * 9.0f);

            l = MAX(MIN(lm-1,7),0);
        }

        p.fg = makeColor(fgLUT[l][0],fgLUT[l][1],fgLUT[l][2]);
        p.bg = makeColor(bgLUT[l][0],bgLUT[l][1],bgLUT[l][2]);
        p.c = cLUT[l];
    
        return p;
        
    };
    
    
};

#endif /* MStoneFloor_h */
