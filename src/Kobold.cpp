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

//--------------------------------------------------------------
Kobold::Kobold() {
    fsm = new StateMachine<Kobold>(this);
    fsm->SetCurrentState(Idle::instance());
}

Kobold::~Kobold() {
    delete fsm;
}


//--------------------------------------------------------------
void Kobold::Ready::update(Kobold*k) {
    printf("-----------------------kobold combat ready update \n");
    // first check to see if it's hurt... if so, switch to retreat state
    
    HealthState h = k->getGeneralHealth();
        
    if (h == NearDeath)
        k->ChangeState(Retreat::instance());
    if (h == Wounded)
        k->ChangeState(Retreat::instance());
    

    // is the current target injured at all?
    if (k->target!=NULL) {
        
        float ctd = k->closestTypeDistance();
        if (ctd < 32 && ctd>10) {
            k->moveTowardsOwnKind();
            return;
            
        }

        
        if (k->target->getGeneralHealth()==Uninjured) {
            
            // if the target is uninjured, the kobold should try to
            // get away from the target.
            
            ofLog() << "target is uninjured";
            
            

            if (k->tooCloseToTarget()) {
                
                ofLog() << ".. too close to target";

                
                // if we're too close, let's try to get away
                
                if (k->moveAwayFromTarget()) {
                    ofLog() << ".... moved away from target";
                    return;

                } else {
                    ofLog() << ".... can't move away from target";

                }// if we can move, we're done this turn
                
                // if we can't move, we can't get away, and we need to prepare for battle
                
                // do we have a ranged weapon equipped?
                if (k->weaponSet==1) {
                    ofLog() << ".... switching to melee weapon";

                    // switch to melee
                    k->switchWeapons();
                    return;
                }
                
                // ok now we are too close, we can't run away, and we have the right weapon equipped
                
                // are we close enough to strike?
                if (k->tooFarFromTarget()) {
                    ofLog() << ".... too far from target to use melee weapon, moving towards";

                    // then get close!
                    k->moveTowardTarget();
                    return;
                } else {
                    ".... attacking";
                    k->attackTarget();
                    return;
                }
                
            } else {
                
                ofLog() << ".. not too close to target";
                
                bool hasRangedWeapon = (k->ranged() != NULL);
                
                if (hasRangedWeapon) {

                    ofLog() << ".... has a ranged weapon";

                    if (k->weaponSet==0) {
                        ofLog() << "...... switching to ranged weapon";

                        k->switchWeapons();
                        return;
                    }
                    ofLog() << ".... attacking target";

                    k->attackTarget();
                    return;
                }
                ofLog() << ".. moving away from target (no ranged weapon)";

                k->moveAwayFromTarget();
                return;
                
                

                
            }
            
        } else {
            
            if (k->withinSwitchToMeleeRange()) {
                if (k->weaponSet==1) {
                    k->switchWeapons();
                    return;
                }
            } else {
                if (k->weaponSet==0) {
                    k->switchWeapons();
                    return;
                }
            }
            
            if (k->canAttackTarget()) {
                k->attackTarget();
            } else {
                k->moveTowardTarget();
            }
            
        }
        
    }

    
};


//--------------------------------------------------------------
void Kobold::Idle::update(Kobold*k) {
    
    
    // scan for player
    float distance = core->player->getPos().distance(k->getPos());
    
    ofLog() << "dist: " << distance;

    // if within range, set state to ready
    if (distance<10) {
        bool canSee = core->losCheck(k, core->player);
        if (canSee) {
            ofLog() << "canSee: true";
            
            k->target = static_cast<Actor*>(core->player);
            
            k->ChangeState(Ready::instance());
        } else {
            ofLog() << "canSee: false";
        }
    }
    
    printf("kobold standing around update \n");
};

//--------------------------------------------------------------
void Kobold::Retreat::update(Kobold*k) {
    
    // scan for player
    float distance = core->player->getPos().distance(k->getPos());
    
    if (k->fear>8) {
        k->ChangeState(Bezerk::instance());
    }
    
    // if within range, set state to ready
    if (distance<12) {
        bool canSee = core->losCheck(k, core->player);
        if (canSee) {
            //ofLog()<< "Kobold::Retreat::update::moveTowardsOwnKind";
            k->fear++;
        }
        
        k->moveAwayFromTarget();
        return;

    }
    
    
    if (k->fear>0) {
        k->fear--;
        k->moveAwayFromTarget();
    } else {
        return;
        k->ChangeState(Idle::instance());
    }
    
};


//--------------------------------------------------------------
void Kobold::Bezerk::update(Kobold*k) {
    
    if (k->tooFarFromTarget()) {
        k->moveTowardTarget();
        return;
    } else {
        k->attackTarget();
        return;
    }
    
};


//--------------------------------------------------------------
void Kobold::init() {
    
    data.strength = 7;
    data.dexterity = 15;
    data.constitution = 9;
    data.intelligence = 8;
    data.wisdom = 7;
    data.charisma = 8;
    data.attackSpeed = 100;
    data.movementSpeed = 100;
    
    data.tohit = 4;
    
    // kobolds need weapons too!
    
    Weapon * weapon = static_cast<Weapon*>(Object::create(Object::Club));
    weapon->init();
    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the kobold has no dedicated inventory
    data.meleeGuid = weapon->guid;
    
    
    weapon = static_cast<Weapon*>(Object::create(Object::Sling));
    weapon->init();
    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the kobold has no dedicated inventory
    data.rangedGuid = weapon->guid;
    
    ofLog() << "Kobold initted with club id: " << data.meleeGuid;
    ofLog() << "Kobold initted with sling id: " << data.rangedGuid;

    // hit dice 1d8
    data.maxhp = data.hp = (int)ofRandom(8)+1;
    

};

//--------------------------------------------------------------
string Kobold::getName() {
    return "Kobold";
}


//--------------------------------------------------------------
InteractionType Kobold::getInteractionType(Object *) {
    return Attack;
}

//--------------------------------------------------------------
DEBT Kobold::traversable()  {
    return TRAVERSE_BLOCKED;
}

//--------------------------------------------------------------
bool Kobold::isPortable()  {
    return false;
}

//--------------------------------------------------------------
bool Kobold::canAttackTarget() {
    if (target==NULL) return false;

    return withinRange();
    
}
//--------------------------------------------------------------
float Kobold::update(DEBT d)  {
    
    if (canAct() && actionDebt>0) actionDebt -= d;
    if (actionDebt<=0) {
        
        DEBT currentDebt = actionDebt;
        
        fsm->update();
    
        /*
        if (data.maxhp - data.hp > 1) {
            if (canRunAwayFromTarget()) {
                runAwayFromTarget();
            } else if (canAttackTarget()) {
                attackTarget();
            }
        } else if (tooFarFromTarget() && canAttackTarget() && canMoveTowardTarget()) {

            if  (ofRandomf() < chargeProbability()) {
                moveTowardTarget();
            } else {
                attackTarget();
            }
        } else if (tooCloseToTarget() && canAttackTarget() && canMoveAwayFromTarget()) {

            if (ofRandomf() < retreatProbability()) {
                moveAwayFromTarget();
            } else {
                attackTarget();
            }
        } else if (canAttackTarget()) {

            attackTarget();
        } else if (tooFarFromTarget() && canMoveTowardTarget()) {

            moveTowardTarget();
        } else if (tooCloseToTarget() && canMoveAwayFromTarget()) {
            
            moveAwayFromTarget();
        } else {
            standStill();
        }
         */
        
        
        DEBT newDebt = actionDebt - currentDebt;
        
        if (newDebt==0) actionDebt += 20;
        
        return ((float)newDebt) / TIME_TO_DEBT_SCALAR;
        
    } else {
        return 0.0f;
    }
    
}

//--------------------------------------------------------------
void Kobold::ChangeState(State<Kobold>* pNewState)
{
    fsm->ChangeState(pNewState);
}
    

//--------------------------------------------------------------
Pixel Kobold::render(float luma)  {
    Pixel p;
    p.fg = makeColor(5,4,1);
    p.bg = 0;
    p.a = 0;
    p.c = toascii('k');
    return p;
}


REGISTER_OBJTYPE(Kobold);
