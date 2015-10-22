//
//  Actor.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//


#include "Actor.h"
#include "Core.h"
#include "ActorEvent.h"
#include "CombatEvent.h"


Actor::Actor() {
    ofAddListener(ActorEvent::actorEvent, this, &Actor::actorEvent);
}

Actor::~Actor() {
    ofRemoveListener(ActorEvent::actorEvent, this, &Actor::actorEvent);
}

void Actor::actorEvent(ActorEvent &e) {
    if (e.a != this) {
        if (e.a == target) {
            target = NULL;
        }
    }
}


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
    
    return attack((Actor*)(target));

    
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
    
    ofVec2f d = ofVec2f(x,y) - ofVec2f(target->x, target->y);
    d.normalize();
    
    if (abs(abs(d.x) - abs(d.y)) < 0.33333f) {
        tryMoving(ofVec2i(sgn(d.x),sgn(d.y)));
    } else if (abs(d.x)>abs(d.y)) {
        tryMoving(ofVec2i(sgn(d.x), 0));
    } else {
        tryMoving(ofVec2i(0,sgn(d.y)));
    }
    
    ofLog() << "moving away ... ";

}

void Actor::moveTowardTarget() {
    if (target==NULL) return;
    
    graph.setWorldCenter(x,y);
    graph.goal = ofVec2i(target->x, target->y);
    graph.process();
    
    ofVec2i moveVector = graph.getMoveVector();
    
    tryMoving(moveVector);

    
    /*
    
    ofVec2f d = ofVec2f(target->x, target->y) - ofVec2f(x,y);
    d.normalize();
    
  
    if (abs(abs(d.x) - abs(d.y)) < 0.33333f) {
        tryMoving(ofVec2i(sgn(d.x),sgn(d.y)));
    } else if (abs(d.x)>abs(d.y)) {
        tryMoving(ofVec2i(sgn(d.x), 0));
    } else {
        tryMoving(ofVec2i(0,sgn(d.y)));
    }
     */
    
}

bool Actor::canRunAwayFromTarget() {
    if (target==NULL) return false;
    return true;
    
}

void Actor::runAwayFromTarget() {
    if (target==NULL) return;
    
    ofVec2f d = ofVec2f(x,y) - ofVec2f(target->x, target->y);
    d.normalize();
    
    if (abs(abs(d.x) - abs(d.y)) < 0.33333f) {
        tryMoving(ofVec2i(sgn(d.x),sgn(d.y)));
    } else if (abs(d.x)>abs(d.y)) {
        tryMoving(ofVec2i(sgn(d.x), 0));
    } else {
        tryMoving(ofVec2i(0,sgn(d.y)));
    }
    
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
    
    target = static_cast<Actor*>(core->player);
    
    return 1;
    
}

int Actor::armorBonus() {
    return data.armorBonus;
}

int Actor::ac() {
    
    int a = 0;
    
    a+= armorBonus();
    a+= dexMod();
    return a;
    
    
//   armor bonus + shield bonus + Dexterity modifier + other modifiers
    
}

DEBT Actor::attack(Actor * a) {
    
    Weapon * w = rightHand();
    
    if (w!=NULL) {
        
        float distanceToTarget = ofVec2i(x, y).distance(ofVec2i(a->x, a->y)) * FEET_PER_TILE; // 5 feet per square

    
        int d20 = (int)ofRandom(0,20)+1;
        
        bool hit = false;
        bool crit = false;
        
        ofPtr<CombatEvent> ce = ofPtr<CombatEvent>(new CombatEvent());
        ce->a = this;
        ce->b = a;
        
        ce->type = CombatEvent::MISS_EVENT;
        ce->dmg = 0;
        
        if (d20==1) {
            hit = false;
        } else if (d20==w->data.criticalThreat) {
            ce->type = CombatEvent::CRIT_EVENT;
            hit = true;
            crit = true;
        } else {
           
            int ac = a->ac();
            int m =strMod() + data.tohit;
            
            ofLog() << getName() << " rolled a " << d20 << " + " << m <<", (ac:"<<ac<<")";
            
            if (d20+m>=ac) {
                hit = true;
                ce->type = CombatEvent::HIT_EVENT;
            }
            
            
        }

        if (crit) {
            
            ce->dmg = w->rollAttack();
            
            for (int i=0; i<w->data.cricitalMultiplier; i++) {
                ce->dmg += w->rollAttack();
            }
            
            
        } else if (hit) {
            BYTE dmg = w->rollAttack();
            
            ce->dmg = (int)dmg;

        }
        

        
        if (w->data.weaponType == Weapon::PROJECTILE_WEAPON) {
            
            
            ofPtr<CombatEvent> ae = ofPtr<CombatEvent>(new CombatEvent());
            
            ae->a = this;
            ae->b = a;
            ae->nextEvent = ce;
            ae->type = CombatEvent::ARROW_EVENT;
            ofNotifyEvent(CombatEvent::combatEvent, *ae);
            
        } else {
            
            ofNotifyEvent(CombatEvent::combatEvent, *ce);

            
        }
        
        //a->takeDamage(ce->dmg); // TODO: move this logic to a combat resolution class
        
        DEBT newDebt = ((float)w->data.attackDebt) * ((float)data.attackSpeed / 100.0f);

        
        ofLog()<<newDebt;

        return newDebt;
        
    } else {
        
        ofLog() << getName() << " has no weapon in hand!";
        
    }



    
    
    return 0;

}


Weapon * Actor::rightHand() {
    Weapon * w;
    
    if (data.rightHandGuid!=0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object * o = Object::elements()[i];
            if (Weapon* w = dynamic_cast<Weapon*>(o)) {
                if (w->guid == data.rightHandGuid) {
                    return w;
                
                }
            }
        }
    }
    return w;
}

Weapon * Actor::leftHand() {
    Weapon * w;

    if (data.leftHandGuid!=0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object * o = Object::elements()[i];
            if (Weapon* w = dynamic_cast<Weapon*>(o)) {
                if (w->guid == data.rightHandGuid) {
                    return w;
                    
                }
            }
        }
        
    }
    return w;
}

void Actor::takeDamage(int dmg) {
    
    data.hp -= dmg;
    

    
}

void Actor::die() {
    
    // any on-death actions should happen here.
    // class removal/cleanup/GC should happen at a higher level
    
    // TODO:remove items in possesion
    
    static ActorEvent ae;
    ae.a = this;
    ae.type = ActorEvent::DEATH_EVENT;
    
    ofNotifyEvent(ActorEvent::actorEvent, ae);
    
    //cleanup();
    
    Weapon * rhw = rightHand();
    if (rhw!=NULL) {
        delete rhw;
    };
    
    delete this;
    
   
}


void Actor::cleanup() {
    
    if (data.hp<=0) {
        die();
    }
    

}

void Actor::setDestination(const ofVec2i d) {
    destination = d;
    graph.goal = d;
    autoTravel = true;
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
    
    newDebt = MAX(newDebt, 100);
    
    if (destination == ofVec2i(x,y)) {
        autoTravel = false;
    }
    



    actionDebt+= round((float)newDebt * (((float)data.movementSpeed) / 100.0f));
    return newDebt;
    
}