//
//  Kobold.hpp
//  matador
//
//  Created by vesabios on 10/14/15.
//
//

#ifndef Kobold_h
#define Kobold_h

#include <stdio.h>
#include "Actor.h"
#include "StateMachine.h"

class Kobold : public Actor {
public:
    
    Kobold();
    ~Kobold() override;

    
    //--------------------------------------------------------------
    class Idle : public State<Kobold> {
    public:
        static Idle* instance() { static Idle s; return &s; };
        
        void enter(Kobold*k) override {};
        void update(Kobold*k) override;
        void exit(Kobold*k) override {};
        
    };
    
    class Ready : public State<Kobold>  {
    public:
        static Ready* instance() { static Ready s; return &s; };
        
        void enter(Kobold*k) override {};
        void update(Kobold*k) override;
        void exit(Kobold*k) override {};
        
    };
    
    class Retreat : public State<Kobold> {
    public:
        static Retreat* instance() { static Retreat s; return &s; };
        
        void enter(Kobold*k) override {};
        void update(Kobold*k) override;
        void exit(Kobold*k) override {};
        
    };
    
    class Bezerk : public State<Kobold> {
    public:
        static Bezerk* instance() { static Bezerk s; return &s; };
        
        void enter(Kobold*k) override {};
        void update(Kobold*k) override;
        void exit(Kobold*k) override {};
        
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
    
    StateMachine<Kobold>* fsm;
    StateMachine<Kobold>* getFSM() const { return fsm; }
    
    void ChangeState(State<Kobold>* pNewState);
 
    
    OBJTYPE(Kobold);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};



#endif /* Kobold_h */
