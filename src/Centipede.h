//
//  Centipede.hpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#ifndef Centipede_h
#define Centipede_h

#include <stdio.h>
#include "Actor.h"
#include "StateMachine.h"

class Centipede : public Actor {
public:
    
    Centipede();
    ~Centipede() override;

    
    //--------------------------------------------------------------
    class Idle : public State<Centipede> {
    public:
        static Idle* instance() { static Idle s; return &s; };
        
        void enter(Centipede*k) override {};
        void update(Centipede*k) override;
        void exit(Centipede*k) override {};
        
    };
    
    class Ready : public State<Centipede>  {
    public:
        static Ready* instance() { static Ready s; return &s; };
        
        void enter(Centipede*k) override {};
        void update(Centipede*k) override;
        void exit(Centipede*k) override {};
        
    };
    
    class Retreat : public State<Centipede> {
    public:
        static Retreat* instance() { static Retreat s; return &s; };
        
        void enter(Centipede*k) override {};
        void update(Centipede*k) override;
        void exit(Centipede*k) override {};
        
    };
    
    class Bezerk : public State<Centipede> {
    public:
        static Bezerk* instance() { static Bezerk s; return &s; };
        
        void enter(Centipede*k) override {};
        void update(Centipede*k) override;
        void exit(Centipede*k) override {};
        
    };
    
    //--------------------------------------------------------------
    bool canAttackTarget();

    InteractionType getInteractionType(Object *) override;
    string getName() override;
    DEBT traversable() override;
    bool isPortable() override;
    float update(DEBT d) override;
    Pixel render(float luma) override;
    
    void init() override;
    
    StateMachine<Centipede>* fsm;
    StateMachine<Centipede>* getFSM() const { return fsm; }
    
    void ChangeState(State<Centipede>* pNewState);
 
    
    OBJTYPE(Centipede);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};



#endif /* Centipede_h */
