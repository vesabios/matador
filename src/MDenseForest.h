//
//  MGreenGrass.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef MDenseForest_h
#define MDenseForest_h

#include <stdio.h>
//#include "ofMain.h"
//#include "Defines.h"
#include "Material.h"

class MDenseForest : public Material {
    
public:

    
    MDenseForest(Material::MaterialType mt) : Material(mt) {}
    string getName() override { return "Dense Forest"; }
    float isOpaque() override { return 0.08f; }
    DEBT traversable() override { return TRAVERSE_BLOCKED; }
    Pixel operator()(ofVec2i pos, float luma) override {
        
        Pixel p;
        char l = 0;
        
        if (luma>0) {
            
            float fg_r;
            float fg_g;
            float fg_b;
            
            float bg_r;
            float bg_g;
            float bg_b;
            
            float aF = 0.15f;
            float aA = 1.6f;
            float aO = 0.56;
            
            float bF = 0.28f;
            float bA = 2.9f;
            float bO = -0.04;
            
            float cF = 0.34;
            float cA = 3.35f;
            float cO = 3.1;
            
            float dF = 0.49;
            float dA = 3.35f;
            float dO = 3.1;
            
            /*
            
            aA = *aAmp;
            bA = *bAmp;
            aF = *aFreq;
            bF = *bFreq;
            aO = *aOffset;
            bO = *bOffset;
             */
            
            float rn = (ofNoise(pos.x*(aF), pos.y*(aF))*(aA)) - (aO);
            rn += (ofNoise(pos.x*(bF), pos.y*(bF))*(bA)) - (bO);


            
            // dense forest!
            
            if (rn<1) {
                
                fg_r = 0;
                fg_g = 0;
                fg_b = 0;
                
                bg_r = 1;
                bg_g = 1;
                bg_b = 0;
                
                p.c = 243;

            } else if (rn<2) {
                
                fg_r = 1;
                fg_g = 2;
                fg_b = 1;
                
                bg_r = 1;
                bg_g = 1;
                bg_b = 0;
                p.c = 243;
            } else if (rn<3) {
                
                fg_r = 1;
                fg_g = 2;
                fg_b = 1;
                
                bg_r = 1;
                bg_g = 1;
                bg_b = 0;
                p.c = 242;
            } else if (rn<4) {
                
                fg_r = 2;
                fg_g = 1;
                fg_b = 0;
                
                bg_r = 1;
                bg_g = 2;
                bg_b = 1;
                p.c = 241;
            }
            
            
            float noise = (ofNoise(pos.x, pos.y)*(0.35)) + (0.97);
            
            char fg_r2 = (dither(pos.x, pos.y, fg_r * luma * noise * 8.0f)-1)/ 7.0f;
            char fg_g2 = (dither(pos.x, pos.y, fg_g * luma * noise * 8.0f)-1)/ 7.0f;
            char fg_b2 = (dither(pos.x, pos.y, fg_b * luma * noise * 8.0f)-1)/ 7.0f;
            char bg_r2 = (dither(pos.x, pos.y, bg_r * luma * noise * 8.0f)-1)/ 7.0f;
            char bg_g2 = (dither(pos.x, pos.y, bg_g * luma * noise * 8.0f)-1)/ 7.0f;
            char bg_b2 = (dither(pos.x, pos.y, bg_b * luma * noise * 8.0f)-1)/ 7.0f;
            
            fg_g2 = MAX(1,fg_g2);
          
            
            p.fg = makeColor(fg_r2,fg_g2,fg_b2);
            p.bg = makeColor(bg_r2,bg_g2,bg_b2);

            
        
        } else {
            
            p.fg = makeColor(0,0,0);
            p.bg = makeColor(0,0,0);
            p.c= 242;
            
        }
        
        return p;

        
    };
    
    
};

#endif /* MDenseForest_h */
