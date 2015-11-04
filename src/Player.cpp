//
//  Player.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#include "Player.h"
#include "Core.h"
#include "Item.h"

string Player::getName() {
    return "Player";
}


void Player::init() {
    
    Weapon * weapon = static_cast<Weapon *>(Object::create(Object::Longsword));
    weapon->init();
    weapon->z = VOID_LOCATION;
    
    data.meleeGuid = weapon->guid;
    data.inventory[1] = weapon->guid;


    weapon = static_cast<Weapon *>(Object::create(Object::Sling));
    weapon->init();
    weapon->z = VOID_LOCATION;
    data.rangedGuid = weapon->guid;

    data.inventory[2] = weapon->guid;

    
    weapon = static_cast<Weapon *>(Object::create(Object::Fists));
    weapon->init();
    weapon->z = VOID_LOCATION;
    data.inventory[0] = weapon->guid;
    
    // set base stats
    
    data.strength = 16;
    data.dexterity = 17;
    data.constitution = 16;
    data.intelligence = 10;
    data.wisdom = 10;
    data.charisma = 10;
    data.tohit = 2;
    data.maxhp = data.hp = 20;
    
}

bool Player::hasWeaponTypeEquipped(Weapon::WeaponType t) {
    
    Weapon * w = melee();
    if (w != NULL) {
        if (w->data.weaponType == t) {
            return true;
        }
    }
    
    w = offHand();
    if (w != NULL) {
        if (w->data.weaponType == t) {
            return true;
        }
    }
    
    w = ranged();
    if (w != NULL) {
        if (w->data.weaponType == t) {
            return true;
        }
    }
    
    return false;
    
}




void Player::cycleWeapons() {
    
    
    int startingIndex = -1;
    
    for (int i=0; i<254; i++) {
        if (data.inventory[i]==data.meleeGuid) {
            startingIndex = i;
        }
    }
    
    Weapon * w = NULL;
    
    // search rest of inventory for next weapon
    for (int i=startingIndex+1; i<255; i++) {
        w = findWeaponByGuid(data.inventory[i]);
        if (w != NULL) {
            
            data.meleeGuid = w->guid;
            return;
        }
    }

    for (int i=0; i<startingIndex; i++) {
        w = findWeaponByGuid(data.inventory[i]);
        if (w != NULL) {
            data.meleeGuid = w->guid;
            return;
        }
    }
    
}


Weapon * Player::findWeaponByGuid(DWORD guid) {
    
    for (int i=0; i<Object::elements().size(); i++) {
        Object * o = Object::elements()[i];
        if (Weapon* w = dynamic_cast<Weapon*>(o)) {
            if (w->guid == guid) {
                return w;
            }
        }
    }
    return NULL;
    
}

int Player::armorBonus() {
    //10 + armor bonus + shield bonus + Dexterity modifier + size modifier
    return 10 + 5 + dexMod();
}

DEBT Player::traversable()  {
    return TRAVERSE_BLOCKED;
}

InteractionType Player::getInteractionType(Object *) {
    return Attack;
}

void Player::tryInteracting(ofVec2i moveVector) {
    
    for (int i=0; i<Object::elements().size(); i++) {
        Object * o = Object::elements()[i];
        if (o->z == engine.map->mapNumber) {
            if (o->x == x + moveVector.x) {
                if (o->y == y + moveVector.y) {
                    InteractionType t = o->getInteractionType(this);
                    if (t > 0) {
                        autoTravel = false;
                        interact(o);
                    }
                }
            }
        }
    }
}



void Player::interact(Object * o) {
    
    ofLog() << "interacting with " << o->getName() << endl;
    
    InteractionType it = o->getInteractionType(this);
    
    switch (it) {
        case Stare:
            break;
        case Read:
        {
            Actor* a = dynamic_cast<Actor*>(o);
            if (a) {
                
                a->readLines();
                
            }
            
            break;
        }
        case Take:
        {
            Item* item = dynamic_cast<Item*>(o);
            if (item) {
                addToInventory(item);
            }
            break;

        }
        case Use:
        {
            Item* i = dynamic_cast<Item*>(o);
            if (i) {
                
                // call the object's use function and pass the player object along as the argument
                
                actionDebt += i->use(static_cast<Actor*>(this));
            }
            break;
        }
        case Attack:
        {
            Actor* a = dynamic_cast<Actor*>(o);
            if (a) {
                
                // call the player's attack function and pass along the targeted actor (if it is one!) (it should be)
                ofLog() << "attacking!";

                attack(a);
            }
            break;
        }
        default:
            break;
    }
    
}


void Player::addToInventory(Item*item) {
    
    if (item->type == Object::Coin) {
        
        
        class Coin * coin = dynamic_cast<class Coin*>(item);
        if (coin) {
            ofLog() << "adding " << coin->data.amount << " GP";
            data.gp += coin->data.amount;
            delete coin;
        }
        
        
    } else {
        
    }
    
    
}

bool Player::isPortable()  {
    return false;
}

float Player::update(DEBT d)  {
    
    actionDebt -= d;
    if (actionDebt<0) {
        actionDebt = 0;
    }
    return 0.0f;
}

void Player::renderUpdate() {
    
    if (displayXP<data.xp) {
        if (data.xp-displayXP>10) {
            displayXP+= 10;
        } else {
            displayXP++;
        }
    }
    if (displayGP<data.gp) {
        if (data.gp-displayGP<10) {
            displayGP+=10;
        } else {
            displayGP++;
        }
    }
    
}





Pixel Player::render(float luma)  {
    Pixel p;
    p.fg = makeColor(5,5,5);
    p.bg = 0;
    p.c = CHAR_PLAYER;
    p.a = 0;
    return p;
}


void Player::die() {
    
}



REGISTER_OBJTYPE(Player);
