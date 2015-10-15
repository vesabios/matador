//
//  Player.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#include "Player.h"
#include "Core.h"

string Player::getName() {
    return "Player";
}

DEBT Player::traversable()  {
    return TRAVERSE_BLOCKED;
}

InteractionType Player::getInteractionTypeForInteractor(Object * o) {
    return Attack;
}

DEBT Player::tryInteracting(ofVec2i moveVector) {

    for (int i=0; i<core->objects.size(); i++) {
        Object * o = core->objects[i];
        if (o->z == core->map->mapNumber) {
            if (o->x == x + moveVector.x) {
                if (o->y == y + moveVector.y) {
                    DEBT d = o->interactable();
                    if (d > 0) {
                        return interact(o);
                    }
                }
            }
        }
    }
    return 0;
}

DEBT Player::interact(Object*o) {
    
    //ofLog() << "interacting with " << o->getName() << endl;
    
    InteractionType it = o->getInteractionTypeForInteractor(this);
    
    switch (it) {
        case Stare:
            break;
        case Read:
            break;
        case Take:
            break;
        case Use:
            break;
        case Attack:
            //ofLog() << getName() << " is attacking " << o->getName();
            return attack(static_cast<Actor*>(o));
            break;
        default:
            break;
    }
    
}

bool Player::isPortable()  {return false;}
void Player::update(DEBT d)  {
    actionDebt -= d;
}

Pixel Player::render(float luma)  {
    Pixel p;
    p.fg = makeColor(5,5,5);
    p.bg = 0;
    p.c = CHAR_PLAYER;
    return p;
}






REGISTER_OBJTYPE(Player);
