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
        data.attackDebt = 150;


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
        data.range = 50; // 50 ft range
        data.attackDebt = 400;

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



class Falchion : public Weapon {
public:
    
    void init() override {
        data.die = 4;
        data.numberOfDice = 2;
        data.criticalThreat = 18; // standard natural 20 crit roll
        data.toHit = 4;
        data.damageBonus = 4;
        data.weaponType = TWO_HANDED_WEAPON;
        data.attackDebt = 300;
        
    }
    
    
    string getName() override  { return "Falchion"; }
    
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii('/');
        return p;
    }
    
    OBJTYPE(Falchion);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Falchion);
