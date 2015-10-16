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
    
 
    InteractionType getInteractionTypeForInteractor(Object *) override;
    DEBT interactable() override { return 0; }

    DEBT tryInteracting(ofVec2i moveVector);
    DEBT interact(ofPtr<Object>);
    
    string getName() override;
    DEBT traversable() override;
    bool isPortable() override;
    void update(DEBT d) override;
    Pixel render(float luma) override;
    
    void init() override;

    
    OBJTYPE(Player);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};
#endif /* Player_h */
