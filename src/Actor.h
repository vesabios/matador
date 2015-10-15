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
#include "ObjectFactory.h"

class Actor : public Object {
    
public:
    
    enum State {
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
  
    float speedMultiplier = 1.0f;
    
    int damage = 0;
    int morale = 100;
    
    Object * target;
    
    bool withinRange();
    bool canRunAwayFromTarget();
    void runAwayFromTarget();
    
    bool canAttackTarget();
    DEBT attackTarget();
    
    bool tooCloseToTarget();
    bool canMoveAwayFromTarget();
    void moveAwayFromTarget();
    
    bool tooFarFromTarget();
    bool canMoveTowardTarget();
    void moveTowardTarget();
    
    void setSpeedMultiplier(float s);
    
    
    float chargeProbability();
    float retreatProbability();
    DEBT standStill();
    
    DEBT attack(Actor * a);


    DEBT actionDebt;
    DEBT tryMoving(ofVec2i moveVector);

};

#endif /* Actor_h */
