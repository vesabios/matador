//
//  Actor.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#include "Actor.h"
#include "Core.h"



bool Actor::withinRange() {
    
    if (target==NULL) return false;
    
    ofVec2i d = ofVec2i(target->x, target->y) - ofVec2i(x,y);
    
    return d.length()<1.5; // technically this just needs to be less than the square root of 2, to test for diagonals
    
}

bool Actor::canAttackTarget() {
    if (target==NULL) return false;

    return true;
}

DEBT Actor::attackTarget() {
    if (target==NULL) return 0;
    
    return attack(static_pointer_cast<Actor>(target));

    
}

bool Actor::tooCloseToTarget() {
    if (target==NULL) return false;

    return false;
}

bool Actor::canMoveAwayFromTarget() {
    if (target==NULL) return false;

    return true;
}


bool Actor::tooFarFromTarget() {
    if (target==NULL) return false;
    
    return !withinRange();
}

bool Actor::canMoveTowardTarget() {
    if (target==NULL) return false;

    return true;
}


void Actor::moveAwayFromTarget() {
    if (target==NULL) return;
    
    ofLog() << "moving away..." ;
}
void Actor::moveTowardTarget() {
    if (target==NULL) return;
    
    ofVec2i d = ofVec2i(target->x, target->y) - ofVec2i(x,y);
    d.normalize();
    
  
    if (abs(abs(d.x) - abs(d.y)) < 0.5) {
        ofLog() << "moving diagonally!";
        tryMoving(ofVec2i(sgn(d.x),sgn(d.y)));
    } else if (abs(d.x)>abs(d.y)) {
        tryMoving(ofVec2i(sgn(d.x), 0));
    } else {
        tryMoving(ofVec2i(0,sgn(d.y)));
     
    }
    
    
    ofLog() << "moving toward..." ;
}

bool Actor::canRunAwayFromTarget() {
    if (target==NULL) return false;
    return true;
    
}

void Actor::runAwayFromTarget() {
    if (target==NULL) return;
    
    ofVec2i d = ofVec2i(x,y) - ofVec2i(target->x, target->y);
    d.normalize();
    
    int mx = round(d.x+0.4f);
    int my = round(d.y+0.4f);
    
    tryMoving(ofVec2i(mx,my));
    
    ofLog() << "running away ... ";
}

void Actor::setSpeedMultiplier(float s) {
    speedMultiplier = s;
}


float Actor::chargeProbability() {

    return 0.5f;
}

float Actor::retreatProbability() {
    
    return 0.5f;
}

DEBT Actor::standStill() {
    ofLog() << "standing still...";
    
    target = core->player;
    
    return 100;
    
}


DEBT Actor::attack(ofPtr<Actor> a) {
    
    int d20 = (int)ofRandom(0,20)+1;
    
    bool hit = false;
    bool crit = false;
    
    
    if (d20==1) {
        ofLog() << getName() << " rolled a natural 1! whiff!";
        hit = false;
    } else if (d20==20) {
        ofLog() << getName() << " rolled a natural 20! crit!";
        hit = true;
        crit = true;
    } else {
        ofLog() << getName() << " rolled a " << d20;
       
        int ac = 10;
        if (d20>=ac) {
            hit = true;
            ofLog()<< getName() << " hit the " << a->getName();
            
        }
    }
    
    ofPtr<Weapon> w = rightHand();
    
    if (w!=NULL) {
        if (crit) {
            
            BYTE dmg = w->rollAttack();
            dmg += w->rollAttack();
            
            ofLog()<< getName() << " critted the " << a->getName() << " for " << (int)dmg << " damage";
            


            
        } else if (hit) {
            BYTE dmg = w->rollAttack();

            ofLog()<< getName() << " hit the " << a->getName() << " for " << (int)dmg << " damage";;
            

        } else {
            ofLog()<< getName() << " missed " << a->getName();
        }
    } else {
        ofLog() << "no weapon in hand!";
    }



    
    
    return round((float)100 * speedMultiplier);

}


ofPtr<Weapon> Actor::rightHand() {
    ofPtr<Weapon> w;
    if (data.rightHandGuid!=0) {
        for (int i=0; i<core->objects.size(); i++) {
            if (core->objects[i]->guid == data.rightHandGuid) {
                return static_pointer_cast<Weapon >(core->objects[i]);
            }
        }
    }
    return w;
}

ofPtr<Weapon> Actor::leftHand() {
    ofPtr<Weapon> w;

    if (data.leftHandGuid!=0) {
        for (int i=0; i<core->objects.size(); i++) {
            if (core->objects[i]->guid == data.leftHandGuid) {
                return static_pointer_cast<Weapon >(core->objects[i]);
            }
        }
    }
    return w;
}


DEBT Actor::tryMoving(ofVec2i moveVector) {
    
    bool vert = false;
    
    DEBT newDebt = 0;
  
    if (moveVector.y!=0) {
        DEBT d = core->traversable(x, y+moveVector.y);
        if (d!=0) {
            vert = true;
            y += moveVector.y;
            newDebt += d;
        }
    }
    
    if (moveVector.x!=0) {
        DEBT d = core->traversable(x+moveVector.x,y);
        if (d!=0) {
            x += moveVector.x;
            if (vert) {
                newDebt *= 1.40;
            } else {
                newDebt += d;
            }
            
        }
    }

    actionDebt+= round((float)newDebt * speedMultiplier);
    return newDebt;
    
}