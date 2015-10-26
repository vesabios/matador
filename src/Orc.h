//
//  Orc.hpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#ifndef Orc_h
#define Orc_h

#include <stdio.h>
#include "Actor.h"

class Orc : public Actor {
public:
    
    bool canAttackTarget();

    InteractionType getInteractionType(Object *) override;
    string getName() override;
    DEBT traversable() override;
    bool isPortable() override;
    float update(DEBT d) override;
    Pixel render(float luma) override;
    
    void init() override;
    
    OBJTYPE(Orc);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};



#endif /* Orc_h */
