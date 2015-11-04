//
//  Centipede.cpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#include "Centipede.h"
#include "Core.h"
#include "ObjectFactory.h"

//--------------------------------------------------------------
Centipede::Centipede() {
    fsm = new StateMachine<Centipede>(this);
    fsm->SetCurrentState(Idle::instance());
}

Centipede::~Centipede() {
    delete fsm;
}


//--------------------------------------------------------------
void Centipede::Ready::update(Centipede*k) {
    if (k->target!=NULL) {
        
    }
    
};


//--------------------------------------------------------------
void Centipede::Idle::update(Centipede*k) {
    
    // scan for player
    float distance = engine.player->getPos().distance(k->getPos());
    

    // if within range, set state to ready
    if (distance<20) {
        bool canSee = engine.losCheck(k, engine.player);
        if (canSee) {
            k->target = static_cast<Actor*>(engine.player);
            k->ChangeState(Bezerk::instance());
        }
    }
    
};

//--------------------------------------------------------------
void Centipede::Retreat::update(Centipede*k) {
    
    k->moveAwayFromTarget();
    
};


//--------------------------------------------------------------
void Centipede::Bezerk::update(Centipede*k) {
    
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
void Centipede::init() {
    
    data.strength = 9;
    data.dexterity = 15;
    data.constitution = 10;
    data.intelligence = 0;
    data.wisdom = 10;
    data.charisma = 2;
    data.attackSpeed = 100;
    data.movementSpeed = 75;
    
    data.tohit = 0;
    data.armorClass = 14;
    data.damageBonus = -1;
    
    data.CR = 1.0f / 2.0f;
    
    // Centipedes need weapons too!
    
    Weapon * weapon = static_cast<Weapon*>(Object::create(Object::Fists));
    weapon->init();
    weapon->data.toHit = 2;
    weapon->data.die = 6;
    weapon->data.attackDebt = 100;
    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the Centipede has no dedicated inventory
    data.meleeGuid = weapon->guid;
    


    // hit dice 1d8
    data.maxhp = data.hp = d8();
    

};

//--------------------------------------------------------------
string Centipede::getName() {
    return "Monstrous Centipede";
}


//--------------------------------------------------------------
InteractionType Centipede::getInteractionType(Object *) {
    return Attack;
}

//--------------------------------------------------------------
DEBT Centipede::traversable()  {
    return TRAVERSE_BLOCKED;
}

//--------------------------------------------------------------
bool Centipede::isPortable()  {
    return false;
}

//--------------------------------------------------------------
bool Centipede::canAttackTarget() {
    if (target==NULL) return false;

    return withinRange();
    
}
//--------------------------------------------------------------
float Centipede::update(DEBT d)  {
    
    //if (canAct() && actionDebt>0) actionDebt -= d;
    if (canAct()) actionDebt -= d;

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
void Centipede::ChangeState(State<Centipede>* pNewState)
{
    fsm->ChangeState(pNewState);
}
    

//--------------------------------------------------------------
Pixel Centipede::render(float luma)  {
    Pixel p;
    p.fg = makeColor(round(4.0f*luma),round(5.0f*luma),round(0.0f*luma));
    p.bg = 0;
    p.a = 0;
    p.c = toascii('c');
    return p;
}


REGISTER_OBJTYPE(Centipede);
