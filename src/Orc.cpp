//
//  Orc.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#include "Orc.h"
#include "Core.h"
#include "ObjectFactory.h"

void Orc::init() {
    
    data.strength = 7;
    data.dexterity = 15;
    data.constitution = 9;
    data.intelligence = 8;
    data.wisdom = 7;
    data.charisma = 8;
    data.attackSpeed = 100;
    data.movementSpeed = 150;
    
    data.tohit = 4;
    
    // Orcs need weapons too!
    
    Weapon * weapon = static_cast<Weapon*>(Object::create(Object::Falchion));
    weapon->init();
    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the Orc has no dedicated inventory
    data.meleeGuid = weapon->guid;
    
    ofLog() << "Orc initted with falchion id: " << data.meleeGuid;
    
    // hit dice 1d8
    data.maxhp = data.hp = (int)ofRandom(8)+1;
    

};


string Orc::getName() {
    return "Orc";
}



InteractionType Orc::getInteractionType(Object *) {
    return Attack;
}


DEBT Orc::traversable()  {
    return TRAVERSE_BLOCKED;
}

bool Orc::isPortable()  {
    return false;
}


bool Orc::canAttackTarget() {
    if (target==NULL) return false;

    return withinRange();
    
}

float Orc::update(DEBT d)  {
    
    if (actionDebt>0) actionDebt -= d;
    
    if (actionDebt<=0) {
    
        DEBT newDebt = 0;
        
        /*
        
        if (data.maxhp - data.hp > 1) {
            //ofLog() << "damage greater than morale..." ;
            if (canRunAwayFromTarget()) {
                runAwayFromTarget();
            } else if (canAttackTarget()) {
                attackTarget();
            }
        } else if (tooFarFromTarget() && canAttackTarget() && canMoveTowardTarget()) {
            //ofLog() << "too far, moving towards or attacking" ;

            if  (ofRandomf() < chargeProbability()) {
                moveTowardTarget();
            } else {
                attackTarget();
            }
        } else if (tooCloseToTarget() && canAttackTarget() && canMoveAwayFromTarget()) {
           // ofLog() << "too close, moving away or attacking" ;

            if (ofRandomf() < retreatProbability()) {
                moveAwayFromTarget();
            } else {
                attackTarget();
            }
        } else if (canAttackTarget()) {
            //ofLog() << "attacking";

            attackTarget();
        } else if (tooFarFromTarget() && canMoveTowardTarget()) {
            //ofLog() << "too far, moving towards" ;

            moveTowardTarget();
        } else if (tooCloseToTarget() && canMoveAwayFromTarget()) {
            
            //ofLog() << "too close, moving away" ;
            moveAwayFromTarget();
        } else {
            standStill();
        }
         */
        
       // ofLog() << getName() << " update debt incurred: "<<newDebt;
        
        actionDebt += newDebt;
        
        return ((float)newDebt) / TIME_TO_DEBT_SCALAR;
        
    } else {
        return 0.0f;
    }
    
    
}

Pixel Orc::render(float luma)  {
    Pixel p;
    p.fg = makeColor(2,2,2);
    p.bg = 0;
    p.a = 0;
    p.c = toascii('O');
    return p;
}


REGISTER_OBJTYPE(Orc);
