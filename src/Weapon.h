//
//  Weapon.h
//  matador
//
//  Created by vesabios on 10/15/15.
//
//

#ifndef Weapons_h
#define Weapons_h
#pragma once

#include "Item.h"
#include "Actor.h"

class Item;



class Weapon : public Item {
public:
    
    enum WeaponType {
        REACH_WEAPON,
        DOUBLE_WEAPON,
        THROWN_WEAPON,
        PROJECTILE_WEAPON,
        AMMUNITION_WEAPON,
        LIGHT_WEAPON,
        ONE_HANDED_WEAPON,
        TWO_HANDED_WEAPON
    };
    
    struct data_t {
        BYTE numberOfDice = 1;
        BYTE die = 4; // type of die, 1d4 in this case
        BYTE toHit = 0;
        BYTE damageBonus = 0;
        BYTE cricitalMultiplier = 1; // standard 1x crit
        BYTE criticalThreat = 20; // standard natural 20 crit roll
        BYTE range = 5; // normal range, 1 square or 5 ft
        DEBT attackDebt = 100; // how much action debt does usage incur
        int weaponType = (int)ONE_HANDED_WEAPON;
        int weight = 1; // 1 pound
        BYTE cost = 1; // 1 gp
    };
    
    data_t data;
    
    void initReflectors() override {
        REFLECT(numberOfDice)
        REFLECT(die)
    }
    
    InteractionType getInteractionType(Object *) override { return Take; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    bool isPortable() override { return true; }
    float update(DEBT d) override { return 0.0f; }
    DEBT use(Actor * a) override {
        return 0;
    }
    BYTE rollAttack() {
        BYTE dmg = 0;
        for (int i=0; i<data.numberOfDice; i++) {
            dmg += (int)ofRandom(data.die)+1;
            dmg += data.damageBonus;
        }
        printf("%dd%d rollAttack: %d\n", data.numberOfDice, data.die, dmg);
        return dmg;
    }
};


#endif /* Header_h */
