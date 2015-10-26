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
    string getName() override  { return "Fists"; }
    void init() override {
        data.die = 3;
        data.numberOfDice = 1;
        data.weaponType = LIGHT_WEAPON;
        data.attackDebt = 40;
    }
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
    string getName() override  { return "Club"; }
    void init() override {
        data.die = 4;
        data.numberOfDice = 1;
        data.weaponType = ONE_HANDED_WEAPON;
        data.attackDebt = 150;
    }
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



class Spear : public Weapon {
public:
    string getName() override  { return "Spear"; }
    void init() override {
        data.die = 6;
        data.numberOfDice = 1;
        data.weaponType = ONE_HANDED_WEAPON;
        data.criticalMultiplier = 3;
        data.range = 20;
        data.attackDebt = 200;
    }
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii('/');
        return p;
    }
    OBJTYPE(Spear);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};
REGISTER_OBJTYPE(Spear);




class Sling : public Weapon {
public:
    string getName() override  { return "Sling"; }
  
    void init() override {
        data.die = 3;
        data.numberOfDice = 1;
        data.weaponType = PROJECTILE_WEAPON;
        data.range = 50;
        data.attackDebt = 400;

    }
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
    string getName() override  { return "Falchion"; }
    void init() override {
        data.die = 4;
        data.numberOfDice = 2;
        data.criticalThreat = 18; // standard natural 20 crit roll
        data.toHit = 0;
        data.damageBonus = 0;
        data.weaponType = TWO_HANDED_WEAPON;
        data.attackDebt = 300;
        
    }
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
