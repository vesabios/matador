//
//  Kobold.hpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#ifndef Kobold_h
#define Kobold_h

#include <stdio.h>
#include "Actor.h"

class Kobold : public Actor {
public:
    
    struct data_t {
        int health = 10;
        int movementSpeed = 100;
        int attackSpeed = 100;
    };
    
    void initReflectors() override {
        REFLECT(health)
        REFLECT(movementSpeed)
        REFLECT(attackSpeed)
        
    }
    
    
    bool withinRange();
    bool canAttackTarget();

    
    InteractionType getInteractionTypeForInteractor(Object * o) override;
    string getName() override;
    DEBT interactable() override;
    DEBT traversable() override;
    bool isPortable() override;
    void update(DEBT d) override;
    Pixel render(float luma) override;
    
    


    
    OBJTYPE(Kobold);
    MSGPACK_DEFINE(type, bundle, x, y, z);
    
};



#endif /* Kobold_h */
