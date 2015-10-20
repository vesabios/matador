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
    
    bool withinRange();
    bool canAttackTarget();

    InteractionType getInteractionTypeForInteractor(Object *) override;
    string getName() override;
    DEBT interactable() override;
    DEBT traversable() override;
    bool isPortable() override;
    float update(DEBT d) override;
    Pixel render(float luma) override;
    
    void init() override;
    
    OBJTYPE(Kobold);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};



#endif /* Kobold_h */
