//
//  Player.hpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
#include "Actor.h"

class Player : public Actor {
public:
    
 
    InteractionType getInteractionType(Object *) override;

    void tryInteracting(ofVec2i moveVector);
    void interact(Object * o);
    
    string getName() override;
    DEBT traversable() override;
    bool isPortable() override;
    float update(DEBT d) override;
    Pixel render(float luma) override;
    void die() override;

    void init() override;
    int armorBonus() override;
    
    void cycleWeapons();

    Weapon * findWeaponByGuid(DWORD guid);
    bool hasWeaponTypeEquipped(Weapon::WeaponType t);

    OBJTYPE(Player);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};
#endif /* Player_h */
