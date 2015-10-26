//
//  Magician.hpp
//  matador
//
//  Created by vesabios on 10/22/15.
//
//

#ifndef Magician_hpp
#define Magician_hpp

#include <stdio.h>
#include "Actor.h"

class Magician : public Actor {
public:
    
    
    InteractionType getInteractionType(Object *) override;
    string getName() override;
    DEBT traversable() override;
    bool isPortable() override;
    float update(DEBT d) override;
    Pixel render(float luma) override;
    
    void init() override;
    
    void readLines() override;
    
    OBJTYPE(Magician);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

#endif /* Magician_hpp */
