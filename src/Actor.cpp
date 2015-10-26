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
    if (weaponSet==0) {
        return withinMeleeRange();
    } else if (weaponSet==1) {
        return withinRangedRange();
    }
    return false;
}


bool Actor::withinMeleeRange() {
    ofVec2i d = ofVec2i(target->x, target->y) - ofVec2i(x,y);
    return d.length()<1.5;
    
}

bool Actor::withinSwitchToMeleeRange() {
    ofVec2i d = ofVec2i(target->x, target->y) - ofVec2i(x,y);
    return d.length()<3.5;
    
}


bool Actor::withinRangedRange() {
    if (target==NULL) return false;
    Weapon * w = melee();
    
    if (w!=NULL) {
        ofVec2i d = ofVec2i(target->x, target->y) - ofVec2i(x,y);
        return d.length()<(w->data.range / 5);
    }
    return false;
}



bool Actor::canAttackTarget() {
    if (target==NULL) return false;

    return true;
}

void Actor::attackTarget() {
    if (target==NULL) return 0;
    
    attack((Actor*)(target));
    
}

bool Actor::tooCloseForRanged() {
    if (target==NULL) return false;
    
    return target->getPos().distance(getPos()) < 25 / FEET_PER_TILE;

}

bool Actor::tooFarForRanged() {
    if (target==NULL) return false;
    
    Weapon * w = ranged();
    
    if (w!=NULL) {
        return target->getPos().distance(getPos()) < w->data.range / FEET_PER_TILE;
    }
    return false;
    
}

bool Actor::tooFarFromTarget() {
    if (target==NULL) return false;
    
    return !withinRange();
}

bool Actor::canMoveTowardTarget() {
    if (target==NULL) return false;

    return true;
}


float Actor::closestTypeDistance() {
    
    float dist = 10000.0f;
    
    for(auto it = elements().begin(); it != elements().end(); it++) {
        Actor * a = dynamic_cast<Actor*>(*it);
        if (a) {
            if (a->z == z) {
                if (a->type == type) {
                    if (a!=this) {
                        dist = MIN(dist, a->getPos().distance(getPos()));
                    }
                }
            }
        }
    }
    
    return dist;
    
}


bool Actor::moveTowardsOwnKind() {
    if (target==NULL) return;
    
    graph.setWorldCenter(x,y);
    graph.goal = ofVec2i(target->x, target->y);
    graph.process();
    
    ///// use placeWorldGoal to place all other objects of this type in the graph
    
    for(auto it = elements().begin(); it != elements().end(); it++) {
        Actor * a = dynamic_cast<Actor*>(*it);
        if (a) {
            if (a->z == z) {
                if (a->type == type) {
                    if (a!=this) {
                        graph.placeWorldGoal(a->x, a->y, 100);
                    }
                }
             }
        }
    }
    
    graph.retreat();
    
    ofVec2i moveVector = graph.getMoveVector();
    
    return tryMoving(moveVector);
}


bool Actor::moveAwayFromTarget() {
    if (target==NULL) return;

    graph.setWorldCenter(x,y);
    graph.goal = ofVec2i(target->x, target->y);
    graph.process();

    graph.retreat();
    
    ofVec2i moveVector = graph.getMoveVector();
    
    return tryMoving(moveVector);

}

void Actor::moveTowardTarget() {
    if (target==NULL) return;
    
    graph.setWorldCenter(x,y);
    graph.goal = ofVec2i(target->x, target->y);
    graph.init();
    
    for(auto it = elements().begin(); it != elements().end(); it++) {
        Actor * a = dynamic_cast<Actor*>(*it);
        if (a) {
            if (a->z == z) {
                if (a!=this) {
                    graph.placeWorldGoal(a->x, a->y, OMIT_VALUE);
                }
            }
        }
    }
    
    graph.setBasis();

    
    graph.iterate();
    
    ofVec2i moveVector = graph.getMoveVector();
    
    tryMoving(moveVector);
    
}

bool Actor::canRunAwayFromTarget() {
    if (target==NULL) return false;
    return true;
    
}

void Actor::runAwayFromTarget() {
    if (target==NULL) return;
    
    ofLog()<< "!!!!!!! DEPRECATED, NO EFFECT";
    
   
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

void Actor::standStill() {

    // do nothing
    
    actionDebt += 20;
}

int Actor::armorBonus() {
    return data.armorBonus;
}

int Actor::ac() {
    
    if (type==Object::Player) {
        
        //10 + armor bonus + shield bonus + Dexterity modifier + size modifier
        
        return 10 + 5 + dexMod();

        
    } else {
        
        return data.armorClass;
    }
    
    return 10;
    
    
//   armor bonus + shield bonus + Dexterity modifier + other modifiers
    
}



Actor::HealthState Actor::getGeneralHealth() {
    
    float v = (float)data.hp / (float)data.maxhp ;
    
    if (v>0.75f) {
        return Uninjured;
    } else if (v>0.5f) {
        return Injured;
    } else if (v>0.25f) {
        return Wounded;
    } else if (v>0) {
        return NearDeath;
    } else {
        return Dead;
    }
        
    
}




void Actor::attack(Actor * a) {
    
    Weapon * w = activeWeapon();
    
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
        
        int ac = a->ac();
        
        int attackBonus = data.tohit + w->data.toHit;
        int damageBonus = data.damageBonus + w->data.damageBonus;

        if (type==Object::Player) {
            
            attackBonus = data.tohit + w->data.toHit;

            switch (w->data.weaponType) {
                case Weapon::LIGHT_WEAPON:
                case Weapon::REACH_WEAPON:
                case Weapon::DOUBLE_WEAPON:
                case Weapon::ONE_HANDED_WEAPON:
                case Weapon::TWO_HANDED_WEAPON:
                {
                    attackBonus += strMod();
                    break;
                }
                    
                case Weapon::THROWN_WEAPON:
                case Weapon::PROJECTILE_WEAPON:
                case Weapon::AMMUNITION_WEAPON:
                {
                    attackBonus += dexMod();
                }

            }
            
            damageBonus = 1 + w->data.damageBonus;
            
        }

        switch (w->data.weaponType) {
            case Weapon::THROWN_WEAPON:
            case Weapon::PROJECTILE_WEAPON:
            case Weapon::AMMUNITION_WEAPON:
            {
                attackBonus -= MAX(0,distanceToTarget - w->data.range) / FEET_PER_TILE;
            }
                
        }
        
        
        ofLog() << getName() << "ac: " << ac << " tohit: "<<attackBonus<<" damageBonus: "<<damageBonus;

        
        

        
        if (d20==1) {
            
            // whiff!!
            hit = false;
            
        } else if (d20>=w->data.criticalThreat) {
            
            // have critical threat, but roll again...
            d20 = (int)ofRandom(0,20)+1;
          
            if (d20+attackBonus>=ac) {
                crit = true;
                ce->type = CombatEvent::CRIT_EVENT;
            } else {
                hit = true;
                ce->type = CombatEvent::HIT_EVENT;
            }
            
        } else {
            if (d20+attackBonus>=ac) {
                hit = true;
                ce->type = CombatEvent::HIT_EVENT;
            }
        }

        if (crit||hit) {
            ce->dmg = w->rollAttack() + damageBonus;
            
            if (crit) {
                for (int i=1; i<w->data.criticalMultiplier; i++) {
                    ce->dmg += w->rollAttack() + damageBonus;
                }
            }
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
        
        actionDebt += ((float)w->data.attackDebt) * ((float)data.attackSpeed / 100.0f);

        

        
    } else {
        
        ofLog() << getName() << " has no weapon in hand!";
        
    }



    
    
    return 0;

}


void Actor::switchWeapons() {
    ofLog() << "switching weapons";
    if (weaponSet==0) {
        weaponSet = 1;
    } else if (weaponSet==1) {
        weaponSet = 0;
    }
    
    actionDebt += 300;
}


Weapon * Actor::activeWeapon() {
    if (weaponSet==0) {
        return melee();
    } else {
        return ranged();
    }
    return NULL;
}

Weapon * Actor::melee() {
    Weapon * w;
    
    if (data.meleeGuid!=0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object * o = Object::elements()[i];
            if (Weapon* w = dynamic_cast<Weapon*>(o)) {
                if (w->guid == data.meleeGuid) {
                    return w;
                
                }
            }
        }
    }
    return NULL;
}

Weapon * Actor::offHand() {
    Weapon * w;

    if (data.offHandGuid!=0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object * o = Object::elements()[i];
            if (Weapon* w = dynamic_cast<Weapon*>(o)) {
                if (w->guid == data.offHandGuid) {
                    return w;
                    
                }
            }
        }
        
    }
    return NULL;
}


Weapon * Actor::ranged() {
    Weapon * w;
    
    if (data.rangedGuid!=0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object * o = Object::elements()[i];
            if (Weapon* w = dynamic_cast<Weapon*>(o)) {
                if (w->guid == data.rangedGuid) {
                    return w;
                }
            }
        }
        
    }
    return NULL;
}


void Actor::takeDamage(int dmg) {
    
    data.hp -= dmg;
    

    
}

void Actor::die() {
    
    // any on-death actions should happen here.
    // class removal/cleanup/GC should happen at a higher level
    
    
    static ActorEvent ae;
    ae.a = this;
    ae.type = ActorEvent::DEATH_EVENT;
    
    ofNotifyEvent(ActorEvent::actorEvent, ae);
    
    
    
    // TODO:remove all items in possesion

    Weapon * w = melee();
    if (w!=NULL) {
        delete w;
    };
    
    w = offHand();
    if (w!=NULL) {
        delete w;
    };
    
    w = ranged();
    if (w!=NULL) {
        delete w;
    };
    
    delete this;
    
   
}

void Actor::readLines() {}


bool Actor::canAct() {
    return data.hp>0;
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

bool Actor::tryMoving(ofVec2i moveVector) {
   
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
    
    if (destination == ofVec2i(x,y)) {
        autoTravel = false;
    }
    
    DEBT nd = round((float)MAX(newDebt, 100) * (((float)data.movementSpeed) / 100.0f));
    
    actionDebt+= nd;
    
    return newDebt>0;
    
}





