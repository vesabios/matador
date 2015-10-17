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


void Player::init() {
    
    Weapon * weapon = static_cast<Weapon *>(Object::create(Object::Club));
    weapon->init();

    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the kobold has no dedicated inventory
    
    data.rightHandGuid = weapon->guid;
    
    data.strength = 11;
    data.dexterity = 17;
    data.constitution = 16;
    data.intelligence = 10;
    data.wisdom = 10;
    data.charisma = 10;
    data.tohit = 2;
    
    
    data.maxhp = data.hp = 20;
    
}

int Player::armorBonus() {
    return 11;
}

DEBT Player::traversable()  {
    return TRAVERSE_BLOCKED;
}

InteractionType Player::getInteractionTypeForInteractor(Object *) {
    return Attack;
}

DEBT Player::tryInteracting(ofVec2i moveVector) {

    for (int i=0; i<Object::elements().size(); i++) {
        Object * o = Object::elements()[i];
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

DEBT Player::interact(Object * o) {
    
    ofLog() << "interacting with " << o->getName() << endl;
    
    InteractionType it = o->getInteractionTypeForInteractor(this);
    
    switch (it) {
        case Stare:
            break;
        case Read:
            break;
        case Take:
            break;
        case Use:
        {
            Item* i = dynamic_cast<Item*>(o);
            if (i) {
                
                // call the object's use function and pass the player object along as the argument
                
                return i->use(static_cast<Actor*>(this));
            } else {
                return 0;
            }
            break;
        }
        case Attack:
        {
            Actor* a = dynamic_cast<Actor*>(o);
            if (a) {
                
                // call the player's attack function and pass along the targeted actor (if it is one!) (it should be)

                return attack(a);
            } else {
                return 0;
            }
            break;
        }
        default:
            break;
    }
    return 0;
    
}

bool Player::isPortable()  {
    return false;
}

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


void Player::die() {
    ofLog() << "PLAYER death!";
    
}



REGISTER_OBJTYPE(Player);
