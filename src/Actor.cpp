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
        return target->getPos().distance(getPos()) > w->data.range / FEET_PER_TILE;
    }
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
    
    ofLog()<<"move towards kind 1";
    
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
    ofLog()<<"move towards kind 2";

    return tryMoving(moveVector);
}


bool Actor::moveAwayFromTarget() {
    if (target==NULL) return;
    ofLog()<<"move away 0";

    graph.setWorldCenter(x,y);
    graph.goal = ofVec2i(target->x, target->y);
    graph.process();

    graph.retreat();
    
    ofVec2i moveVector = graph.getMoveVector();
    
    ofLog()<<"move away 1";

    
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
    
    /*
     Level	Base Attack Bonus	Fort Save	Ref Save	Will Save	Special
     1st	+1	+2	+0	+0	Bonus feat
     2nd	+2	+3	+0	+0	Bonus feat
     3rd	+3	+3	+1	+1
     4th	+4	+4	+1	+1	Bonus feat
     5th	+5	+4	+1	+1
     6th	+6/+1	+5	+2	+2	Bonus feat
     7th	+7/+2	+5	+2	+2
     8th	+8/+3	+6	+2	+2	Bonus feat
     9th	+9/+4	+6	+3	+3
     10th	+10/+5	+7	+3	+3	Bonus feat
     11th	+11/+6/+1	+7	+3	+3
     12th	+12/+7/+2	+8	+4	+4	Bonus feat
     13th	+13/+8/+3	+8	+4	+4
     14th	+14/+9/+4	+9	+4	+4	Bonus feat
     15th	+15/+10/+5	+9	+5	+5
     16th	+16/+11/+6/+1	+10	+5	+5	Bonus feat
     17th	+17/+12/+7/+2	+10	+5	+5
     18th	+18/+13/+8/+3	+11	+6	+6	Bonus feat
     19th	+19/+14/+9/+4	+11	+6	+6	
     20th	+20/+15/+10/+5	+12	+6	+6	Bonus feat
     */
    
    if (type==Object::Player) {
        
        //10 + armor bonus + shield bonus + Dexterity modifier + size modifier
        
        int dodge  = 0;
        if (level()>2) {
            dodge = 1;
            
            
        }
        
        return 10 + 5 + dexMod() + dodge;

        
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

    
        int roll = d20();
        
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
                    attackBonus += strMod() ;
                    break;
                }
                    
                case Weapon::THROWN_WEAPON:
                case Weapon::PROJECTILE_WEAPON:
                case Weapon::AMMUNITION_WEAPON:
                {
                    attackBonus += dexMod() ;
                }
                    

            }
            
            attackBonus += level();
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
        
        
        //ofLog() << getName() << " > target ac: " << ac << " tohit: "<<attackBonus<<" damageBonus: "<<damageBonus;

        
        //ofLog() << "attack roll: " << roll << " +" << attackBonus << ", against ac: "<<ac;

        
        if (roll==1) {
            
            // whiff!!
            hit = false;
            
        } else if (roll>=w->data.criticalThreat) {
            
            //ofLog() << "Crit!";
            
            // have critical threat, but roll again...
            roll = d20();
          
            if (roll+attackBonus>=ac) {
                crit = true;
                ce->type = CombatEvent::CRIT_EVENT;
            } else {
                hit = true;
                ce->type = CombatEvent::HIT_EVENT;
            }
            
        } else if (roll+attackBonus>=ac) {
            hit = true;
            ce->type = CombatEvent::HIT_EVENT;
            
        }

        if (crit||hit) {
            ce->dmg = w->rollAttack() + damageBonus;
            
            if (crit) {
                for (int i=1; i<w->data.criticalMultiplier; i++) {
                    ce->dmg += w->rollAttack() + damageBonus;
                }
            }
        }
        
        if (ce->dmg ==0) ce->type = CombatEvent::MISS_EVENT;

        
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

void Actor::takeDamageFrom(int dmg, Actor * a) {
    takeDamage(dmg);
    if (data.hp<=0) {
        a->awardExperienceForKilling(this);
    }
}

void Actor::awardExperienceForKilling(Actor * a) {
    
    int cl = level();
    
    data.xp += experience(level(), a->data.CR);
    
    if (level()>cl) {
        data.hp = data.maxhp;
    }
    
    
}



vector<Item*> Actor::getInventory() {
    
    vector<Item*> items;
    
    for (int i=0; i<256; i++) {
        DWORD id = data.inventory[i];
        
        for(auto it = elements().begin(); it != elements().end(); it++) {
            Item * item = dynamic_cast<Item*>(*it);
            if (item) {
                if (item->guid == id) {
                    items.push_back(item);
                }
            }
        }
    }
    
    return items;

    
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
        DEBT d = engine.traversable(x, y+moveVector.y);
        if (d!=0) {
            vert = true;
            y += moveVector.y;
            newDebt += d;
        }
    }
    
    if (moveVector.x!=0) {
        DEBT d = engine.traversable(x+moveVector.x,y);
        if (d!=0) {
            x += moveVector.x;
            if (vert) {
                newDebt *= 1.40;
            } else {
                newDebt += d;
            }
            
        }
    }
    
    newDebt = newDebt;
    
    if (destination == ofVec2i(x,y)) {
        autoTravel = false;
    }
    
    DEBT nd = round((float)MAX(newDebt, 10) * (((float)data.movementSpeed) / 100.0f));
    
    actionDebt+= nd;
    
    return newDebt>0;
    
}





