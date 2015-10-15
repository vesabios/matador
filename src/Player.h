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
 
    InteractionType getInteractionTypeForInteractor(Object * o) override;
    DEBT interactable() override { return 0; }

    DEBT tryInteracting(ofVec2i moveVector);
    DEBT interact(Object * o);
    
    string getName() override;
    DEBT traversable() override;
    bool isPortable() override;
    void update(DEBT d) override;
    Pixel render(float luma) override;
    
    OBJTYPE(Player);
    MSGPACK_DEFINE(type, bundle, x, y, z);
    
};
#endif /* Player_h */
