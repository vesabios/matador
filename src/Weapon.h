//
//  Weapon.h
//  matador
//
//  Created by vesabios on 10/15/15.
//
//

#ifndef Weapons_h
#define Weapons_h

#include "Item.h"

class Weapon : public Item {
public:
    struct data_t {
        BYTE numberOfDice = 1;
        BYTE die = 4;
        BYTE modifier = 0;
    };
    
    data_t data;
    
    void initReflectors() override {
        REFLECT(numberOfDice)
        REFLECT(die)
    }
    
    DEBT interactable() override { return 1; }
    InteractionType getInteractionTypeForInteractor(Object *) override { return Take; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    bool isPortable() override { return true; }
    void update(DEBT d) override {}
    
    BYTE rollAttack() {
        BYTE dmg = 0;
        for (int i=0; i<data.numberOfDice; i++) {
            dmg += (int)ofRandom(data.die)+1;
        }
        printf("%dd%d rollAttack: %d\n", data.numberOfDice, data.die, dmg);
        return dmg;
    }
};


#endif /* Header_h */
