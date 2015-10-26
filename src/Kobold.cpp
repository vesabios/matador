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
    printf("-----------------------Kobold::Ready::update\n");

    // first check to see if it's hurt... if so, switch to retreat state
    
    HealthState h = k->getGeneralHealth();
        
    if (h == NearDeath) {
        if (d4()>1) {
            k->ChangeState(Retreat::instance());
        }
    }
    if (h == Wounded) {
        if (d4()>2) {
            k->ChangeState(Retreat::instance());
        }
        
    }
    

    if (k->target!=NULL) {
        
        
        if (d4()>=2) {
            float ctd = k->closestTypeDistance();
            if (ctd < 32 && ctd>5) {
                printf("---------------moveTowardsOwnKind \n");
                
                k->moveTowardsOwnKind();
                return;
                    
            }
        }

        


        // is the current target injured at all?

        if (k->target->getGeneralHealth()==Uninjured) {
            
            // if the target is uninjured, the kobold should try to
            // get away from the target.
            
            ofLog() << "target is uninjured";
            
            
            if (d20()==1) k->ChangeState(Bezerk::instance());
            
            if (k->tooFarForRanged()) {
                
                k->moveTowardTarget();
                return;
                
            } else if (k->tooCloseForRanged()) {
                
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
                    
                    int d4 = (int)ofRandom(0,4)+1;
                    
                    if (d4>1) {
                        k->attackTarget();
                        ofLog() << ".... attacking target";
                        
                    } else {
                        k->moveTowardTarget();

                    }


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
                return;
                
            } else {
                k->moveTowardTarget();
                return;
                
            }
            
        }
        
    }

    
};


//--------------------------------------------------------------
void Kobold::Idle::update(Kobold*k) {
    printf("-----------------------Kobold::Idle::update\n");
  
    
    // scan for player
    float distance = core->player->getPos().distance(k->getPos());
    
    ofLog() << "dist: " << distance;

    // if within range, set state to ready
    if (distance<20) {
        bool canSee = core->losCheck(k, core->player);
        if (canSee) {
            ofLog() << "canSee: true";
            
            k->target = static_cast<Actor*>(core->player);
            
            k->ChangeState(Ready::instance());
        } else {
            ofLog() << "canSee: false";
        }
    }
    
};

//--------------------------------------------------------------
void Kobold::Retreat::update(Kobold*k) {
    printf("-----------------------Kobold::Retreat::update\n");

    // scan for player
    float distance = core->player->getPos().distance(k->getPos());
    
    if (k->fear>8) {
        int d4 = (int)ofRandom(0,4)+1;
        if (d4>3) {
            k->ChangeState(Bezerk::instance());
        }
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
    printf("-----------------------Kobold::Bezerk::update\n");
 
    
    if (k->weaponSet==1) {
        k->switchWeapons();
        return;
    }
    
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
    
    data.strength = 9;
    data.dexterity = 13;
    data.constitution = 9;
    data.intelligence = 8;
    data.wisdom = 7;
    data.charisma = 8;
    data.attackSpeed = 100;
    data.movementSpeed = 100;
    
    data.tohit = 1;
    data.armorClass = 15;
    data.damageBonus = -1;
    
    // kobolds need weapons too!
    
    Weapon * weapon = static_cast<Weapon*>(Object::create(Object::Spear));
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
    p.fg = makeColor(round(5.0f*luma),round(4.0f*luma),round(1.0f*luma));
    p.bg = 0;
    p.a = 0;
    p.c = toascii('k');
    return p;
}


REGISTER_OBJTYPE(Kobold);
