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
    
    static char dither(int x, int y, float c0) {
        
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
    
    
    Material(MaterialType t) : mt(t) {};
    MaterialType mt;
    
    virtual string getName() = 0;
    virtual bool isOpaque() = 0;
    virtual DEBT traversable() = 0;
    virtual Pixel operator()(ofVec2i pos, float luma) = 0;
    
    
};
#endif /* Material_h */


/*

} else {
    
    
    unsigned char c;
    
    float gNoise = ofNoise(mx * 0.05f, my * 0.05f) + ofNoise(mx * 0.4f, my * 0.4f) *0.4f;
    float bNoise = ofNoise(mx * 0.07f, my * 0.07f) + ofNoise(mx * 0.3f, my * 0.3f) *0.3f;
    
    if (gNoise>0.872) {
        
        int v = MIN(4,ditherValue(mx, my, att*flickerValue * aa * 15));
        if (v>0) {
            map->setWindowSeen(x,y);
        }
        
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
        
    } else if (bNoise>1.28) {
        
        int v = int(att*flickerValue * aa * 5);
        
        if (v>0) {
            map->setWindowSeen(x,y);
        }
        if (v<=0) {
            c = toascii(' ');
        } else {
            
            float water = ofNoise((mx*0.06f)+ofGetElapsedTimef()*0.1f, (my*0.06f)+ofGetElapsedTimef()*0.2f,ofGetElapsedTimef()*0.1f);
            
            if (water>0.9) {
                c = toascii('~');
            } else if (water>0.8) {
                c = toascii('-');
            } else if (water>0.7) {
                c = 247;
            } else if (water>0.6) {
                c = 176;
            } else {
                c = ' ';
            }
            
            
        }
        
        v = MIN(5,ditherValue(mx, my, att*flickerValue*aa * 4));
        console.setPixel(x,y, makeColor(v*0.0,v*0.6,v),makeColor(v*0.0,v*0.2,v*0.5), c);
        
    } else {
        
        float a = (att*flickerValue* aa * 8.9f) -0.2f;
        int v =  MIN(2,ditherValue(mx,my,a));
        if (v>0) {
            map->setWindowSeen(x,y);
        }
        unsigned char c;
        if (v<=0) {
            c = toascii(' ');
            console.setPixel(x,y, makeColor(1,1,2),0, 243);
            
        } else {
            v = MIN(3,ditherValue(mx, my, att*flickerValue*aa * 4.0f));
            
            if (v==0) {
                console.setPixel(x,y, makeColor(1,1,2),makeColor(0,0,0), 242);
            } else if (v==1) {
                console.setPixel(x,y, makeColor(1,2,2),makeColor(0,0,0), 242);
            } else {
                console.setPixel(x,y, makeColor(2,2,2),makeColor(0,0,0), 242);
                
            }
            
        }
        
        
    }
    
    
    
    
    
}


*/


