//
//  MAlgae.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef MAlgae_h
#define MAlgae_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"
#include "Material.h"


class MAlgae : public Material {
    
public:
    
    int cLUT[8] ={
        243,
        243,
        CHAR_PLANTS0,
        CHAR_PLANTS1,
        CHAR_PLANTS2,
        CHAR_DITHER1,
        CHAR_PLANTS3,
        242
    };
    
    int fgLUT[8][3]= {
        {0,  1,  0}, // memory
        {0,  2,  0},
        {0,  2,  0},
        {0,  2,  1},
        {0,  2,  1},
        {0,  3,  1},
        {0,  3,  1},
        {0,  4,  2}
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
    
    
    MAlgae(MaterialType mt) : Material(mt) {}
    string getName() override { return "Algae"; }
    
    bool isOpaque() override { return false; }
    DEBT traversable() override { return TRAVERSE_SLIPPERY; }
    Pixel operator()(ofVec2i pos, float luma) override {
        
        Pixel p;
        char l = 0;
        
        char gc = 0;
        
        char lm;
        
        if (luma>0) {
            float noise = (ofNoise(pos.x, pos.y)*0.2f) + 0.8f;
            lm = dither(pos.x, pos.y, luma * noise * 9.0f);
            l = MAX(MIN(lm-1,7),0);
            
            if (l>1) {

                float grassNoise = ofNoise(pos.x, pos.y);
                l = MIN(int (grassNoise * 7.0f), 6);
            }
            
            p.fg = makeColor(lm*0.3,lm*0.6,lm*0.2);
            p.bg = makeColor(lm*0.1,lm*0.4,lm*0.2);
            
        } else {
            p.fg = makeColor(fgLUT[l][0],fgLUT[l][1],fgLUT[l][2]);
            p.bg = makeColor(bgLUT[l][0],bgLUT[l][1],bgLUT[l][2]);
            
        }
        


        p.c = cLUT[l];
        
        return p;
        
    };
    
    /*

    
    if (v<=0) {
        c = 243;//toascii(' ');
    } else if (v<=1) {
        c = 243;
    } else {
        
        float grassNoise = ofNoise(mx*1.0f, my*1.0f);
        
        if (grassNoise>0.9) {
            c = CHAR_PLANTS0;
        } else if (grassNoise>0.8) {
            c = CHAR_PLANTS1;
        } else if (grassNoise>0.7) {
            c = CHAR_PLANTS2;
        } else if (grassNoise>0.6) {
            c = CHAR_PLANTS3;
        } else if (grassNoise>0.5) {
            c = 242;
        } else {
            c = 243;
        }
        
        
    }
    
    v = MIN(2,ditherValue(mx, my, att*flickerValue*aa * 6))+2;
    console.setPixel(x,y, makeColor(v*0.3,v*0.6,v*0.2),makeColor(v*0.1,v*0.4,v*0.2), c);
     
     */
    
    
};

#endif