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

class Fists : public Weapon {
public:
    
    void init() override {
        data.die = 3;
        data.numberOfDice = 1;
        data.weaponType = LIGHT_WEAPON;
        data.attackDebt = 40;
    }
    
    string getName() override  { return "Fists"; }
    
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii(' ');
        return p;
    }
    
    OBJTYPE(Fists);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Fists);


class Club : public Weapon {
public:
    
    void init() override {
        data.die = 4;
        data.numberOfDice = 1;
        data.weaponType = ONE_HANDED_WEAPON;
        data.attackDebt = 300;


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



class Sling : public Weapon {
public:
    
    void init() override {
        data.die = 3;
        data.numberOfDice = 1;
        data.weaponType = PROJECTILE_WEAPON;
        BYTE rangeIncrememt = 50; // 50 ft range
        data.attackDebt = 200;

    }
    
    
    string getName() override  { return "Sling"; }
    
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii('/');
        return p;
    }
    
    OBJTYPE(Sling);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Sling);