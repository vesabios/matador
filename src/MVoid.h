//
//  MVoid.h
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef MVoid_h
#define MVoid_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"
#include "Material.h"

class MVoid : public Material {
    
public:

    MVoid(MaterialType mt) : Material(mt) {}
    string getName() override { return "Void"; }
    bool isOpaque() override { return true; }
    DEBT traversable() override { return TRAVERSE_BLOCKED; }
    Pixel operator()(ofVec2i pos, float luma) override {
        
        Pixel p;

        p.fg = 0;
        p.bg = 0;
        p.c = 0;
        
        return p;
        
    };

};

#endif /* MVoid_h */
