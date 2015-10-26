//
//  MLightDirt.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef MLightDirt_h
#define MLightDirt_h

#include <stdio.h>

#include "Material.h"

class MLightDirt : public Material {
    
public:

    
    MLightDirt(Material::MaterialType mt) : Material(mt) {}
    string getName() override { return "Light Dirt"; }
    float isOpaque() override { return 0.0f; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
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
            
            float aF = 0.042f;
            float aA = 1.92f;
            float aO = 1.2;
            
            float bF = 0.23f;
            float bA = 2.35f;
            float bO = 1.1;
            
            float cF = 0.534;
            float cA = 3.34f;
            float cO = 3.1;
            
            float dF = 0.49;
            float dA = 3.35f;
            float dO = 3.1;
            
            float rn = (ofNoise(pos.x*(aF), pos.y*(aF))*(aA)) - (aO);
            rn += (ofNoise(pos.x*(bF), pos.y*(bF))*(bA)) - (bO);


            
            // base grass
            
            if (rn>0) {
                
                fg_r = 3;
                fg_g = 2;
                fg_b = 1;
                
                bg_r = 2;
                bg_g = 2;
                bg_b = 1;
                
                p.c = 243;

            } else {
                
                fg_r = 3;
                fg_g = 2;
                fg_b = 1;
                
                bg_r = 2;
                bg_g = 2;
                bg_b = 1;
                p.c = 242;
            }
           
            
            // splothces
            float gn = (ofNoise(pos.x*(cF), pos.y*(cF))*(cA)) - (cO);

            if (gn>0) {
                
                fg_r = 2;
                fg_g = 2;
                fg_b = 1;
                
                bg_r = 3;
                bg_g = 2;
                bg_b = 1;

                p.c = 242;

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
            
            p.fg = makeColor(0,1,0);
            p.bg = makeColor(0,0,0);
            p.c= 242;
            
        }
        
        return p;

        
    };
    
    
};

#endif /* MLightDirt_h */
