//
//  Kobold.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#include "Kobold.h"
#include "Core.h"
#include "ObjectFactory.h"

void Kobold::init() {
    
    // kobolds need weapons too!
    
    ofPtr<Weapon> weapon = static_pointer_cast<Weapon >(Object::create(Object::Club));
    
    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the kobold has no dedicated inventory
    
    data.rightHandGuid = weapon->guid;
    
    core->objects.push_back(weapon);
    
    ofLog() << "Kobold initted with club id: " << data.rightHandGuid;
    
    


};


string Kobold::getName() {
    return "Kobold";
}

DEBT Kobold::interactable() {
    return 1;
}

InteractionType Kobold::getInteractionTypeForInteractor(Object *) {
    return Attack;
}


DEBT Kobold::traversable()  {
    return TRAVERSE_BLOCKED;
}

bool Kobold::isPortable()  {
    return false;
}

bool Kobold::withinRange() {
    
    if (target==NULL) return false;
    
    ofVec2i d = ofVec2i(target->x, target->y) - ofVec2i(x,y);
    
    return d.length()<1.5;
    
}

bool Kobold::canAttackTarget() {
    if (target==NULL) return false;

    return withinRange();
    
}

void Kobold::update(DEBT d)  {
    
    if (actionDebt>0) actionDebt -= d;
    
    if (actionDebt<=0) {
    
        setSpeedMultiplier(1.5f);
        
        DEBT newDebt = 0;
    
        if (damage > morale) {
            ofLog() << "damage greater than morale..." ;
            if (canRunAwayFromTarget()) {
                runAwayFromTarget();
            } else if (canAttackTarget()) {
                newDebt = attackTarget();
            }
        } else if (tooFarFromTarget() && canAttackTarget() && canMoveTowardTarget()) {
            ofLog() << "too far, moving towards or attacking" ;

            if  (ofRandomf() < chargeProbability()) {
                moveTowardTarget();
            } else {
                newDebt = attackTarget();
            }
        } else if (tooCloseToTarget() && canAttackTarget() && canMoveAwayFromTarget()) {
            ofLog() << "too close, moving away or attacking" ;

            if (ofRandomf() < retreatProbability()) {
                moveAwayFromTarget();
            } else {
                newDebt = attackTarget();
            }
        } else if (canAttackTarget()) {
            ofLog() << "attacking";

            newDebt = attackTarget();
        } else if (tooFarFromTarget() && canMoveTowardTarget()) {
            ofLog() << "too far, moving towards" ;

            moveTowardTarget();
        } else if (tooCloseToTarget() && canMoveAwayFromTarget()) {
            
            ofLog() << "too close, moving away" ;
            moveAwayFromTarget();
        } else {
            newDebt = standStill();
        }
        
        actionDebt += newDebt;
        
    }
    
    
}

Pixel Kobold::render(float luma)  {
    Pixel p;
    p.fg = makeColor(4,3,1);
    p.bg = 0;
    p.c = toascii('k');
    return p;
}


REGISTER_OBJTYPE(Kobold);
