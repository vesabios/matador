//
//  Actor.h
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#ifndef Actor_h
#define Actor_h

#include <stdio.h>
#include "ActorEvent.h"
#include "ObjectFactory.h"
#include "Weapon.h"
#include "Dijkstra.hpp"

class Weapon;



class Actor : public Object {
    
public:
    
    enum HealthState {
        Uninjured,  // >75
        Injured,    // >50
        Wounded,    // >25
        NearDeath,  // >0
        Dead,       // 0
    };
    
    enum ActorState {
        VOID,
        SLEEP,
        IDLE,
        PATROL,
        CHASE,
        BEZERK,
        AFRAID
        
    };
    
    enum Disposition {
        NEUTRAL,
        FRIENDLY,
        HOSTILE
    };
    
    
    
    struct data_t {
        
        int maxhp = 10;
        int hp = maxhp;
        int movementSpeed = 100;
        int attackSpeed = 100;
        
        DWORD meleeGuid = 0;
        DWORD offHandGuid = 0;
        DWORD rangedGuid = 0;
        
        BYTE strength = 10;
        BYTE dexterity = 10;
        BYTE constitution = 10;
        BYTE intelligence = 10;
        BYTE wisdom = 10;
        BYTE charisma = 10;
        
        int tohit = 0;
        BYTE armorBonus = 10;
        int damageBonus = 0;
        BYTE armorClass = 10;
        

        int xp = 0;
        int gold = 0;
        
        DWORD inventory[255];
        
    };
    
    data_t data;
    
    string lines[30];
    
    
    Weapon * activeWeapon();
    
    void switchWeapons();

    Weapon * offHand();
    Weapon * melee();
    Weapon * ranged();

    
    void initReflectors() override {
        REFLECT(hp)
        REFLECT(movementSpeed)
        REFLECT(attackSpeed)
        REFLECT(strength)
        REFLECT(dexterity)
        REFLECT(constitution)
        REFLECT(intelligence)
        REFLECT(wisdom)
        REFLECT(charisma)
        
    }
    
    Actor();
    ~Actor();
    
    virtual int armorBonus();
    int ac();
    
    int strength() {
        return data.strength;
    }
    
    int strMod() {
        return getModifier(strength());
    }
    
    int dexterity() {
        return data.dexterity;
    }
    
    int dexMod() {
        return getModifier(dexterity());
    }
    
    int constitution() {
        return data.constitution;
    }
    
    int conMod() {
        return getModifier(constitution());
    }

    int intelligence() {
        return data.intelligence;
    }

    int intMod() {
        return getModifier(intelligence());
    }

    int wisdom() {
        return data.wisdom;
    }
    
    int widMod() {
        return getModifier(wisdom());
    }
    
    int charisma() {
        return data.charisma;
    }
    
    int chrMod() {
        return getModifier(charisma());
    }
    
    int getModifier(int stat) {
        switch (stat) {
            case 0:
            case 1:
                return -5;
                break;
            case 2:
            case 3:
                return -4;
            case 4:
            case 5:
                return -3;
            case 6:
            case 7:
                return -2;
            case 8:
            case 9:
                return -1;
            case 10:
            case 11:
                return 0;
            case 12:
            case 13:
                return 1;
            case 14:
            case 15:
                return 2;
            case 16:
            case 17:
                return 3;
            case 18:
            case 19:
                return 4;
            case 20:
            case 21:
                return 5;
            case 22:
            case 23:
                return 6;
            case 24:
            case 25:
                return 7;
            case 26:
            case 27:
                return 8;
            case 28:
            case 29:
                return 9;
            case 30:
                return 10;
            default:
                return 0;
        }
    }
    
    int getProficiencyBonus() {
        if (data.xp>=225000) {
            return 6;
        } else if (data.xp>=120000) {
            return 5;
        } else if (data.xp>=48000) {
            return 4;
        } else if (data.xp>=6500) {
            return 3;
        } else {
            return 2;
        }
    }
    
    int level() {
        if (data.xp>=355000) {
            return 20;
        } else if (data.xp>=305000) {
            return 19;
        } else if (data.xp>=265000) {
            return 18;
        } else if (data.xp>=225000) {
            return 17;
        } else if (data.xp>=195000) {
            return 16;
        } else if (data.xp>=165000) {
            return 15;
        } else if (data.xp>=140000) {
            return 14;
        } else if (data.xp>=120000) {
            return 13;
        } else if (data.xp>=100000) {
            return 12;
        } else if (data.xp>=85000) {
            return 11;
        } else if (data.xp>=64000) {
            return 10;
        } else if (data.xp>=48000) {
            return 9;
        } else if (data.xp>=34000) {
            return 8;
        } else if (data.xp>=23000) {
            return 7;
        } else if (data.xp>=14000) {
            return 6;
        } else if (data.xp>=6500) {
            return 5;
        } else if (data.xp>=2700) {
            return 4;
        } else if (data.xp>=900) {
            return 3;
        } else if (data.xp>=300) {
            return 2;
        } else {
          1;
        }
        
    }
    
    int weaponSet = 0;
    
    float speedMultiplier = 1.0f;
    
    int damage = 0;
    int morale = 100;
    
    int fear = 0;
    
    Dijkstra graph;

    
    
    bool autoTravel = false;
    ofVec2i destination;
    
    Actor * target = NULL;
    
    bool withinRange();
    bool withinMeleeRange();
    bool withinRangedRange();
    bool withinSwitchToMeleeRange();
    
    bool canRunAwayFromTarget();
    void runAwayFromTarget();
    
    bool canAttackTarget();
    void attackTarget();
    
    bool tooCloseForRanged();
    bool tooFarForRanged();
  //  bool canMoveAwayFromTarget();
    bool moveAwayFromTarget();
    bool moveTowardsOwnKind();
    
    bool tooFarFromTarget();
    bool canMoveTowardTarget();
    void moveTowardTarget();
    
    float closestTypeDistance();

    void setSpeedMultiplier(float s);
    
    void takeDamage(int dmg);
    
    virtual void die();
    virtual void readLines();
  
    float chargeProbability();
    float retreatProbability();
    
    void setDestination(ofVec2i d);
    
    void actorEvent(ActorEvent &e);
    
    void cleanup();
        
    HealthState getGeneralHealth();
    
    bool canAct();
    void standStill();
    void attack(Actor * a);
    void fire(Actor * a);
    DEBT actionDebt = 0;
    bool tryMoving(ofVec2i moveVector);

};

#endif /* Actor_h */
