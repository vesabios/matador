//
//  Weapon.cpp
//  matador
//
//  Created by vesabios on 10/17/15.
//
//

#include <stdio.h>
#include "ObjectFactory.h"
#include "Weapon.h"

class Club : public Weapon {
public:
    
    void init() override {
        data.die = 4;
        data.numberOfDice = 1;
    }
    

    
    string getName() override  { return "Club"; }

    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii('/');
        return p;
    }
    
    OBJTYPE(Club);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Club);