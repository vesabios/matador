//
//  CombatEvent.h
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#ifndef CombatEvent_h
#define CombatEvent_h

#include <stdio.h>
#include "ofEvents.h"
#include "ofTypes.h"

class Actor;
class CombatEvent : public ofEventArgs {
    
public:
    
    ~CombatEvent() {
        printf("dtor combat event\n");
    }
    
    enum EventType {
        NULL_EVENT,
        MISS_EVENT,
        HIT_EVENT,
        CRIT_EVENT,
        ARROW_EVENT
    };
    
    Actor * a;
    Actor * b;

    EventType type;
    int dmg;
    
    
    ofPtr<CombatEvent> nextEvent;
    
    
    CombatEvent() {
        a = NULL;
        b = NULL;
        dmg = 0;
        EventType type = NULL_EVENT;
    }
    
    static ofEvent <CombatEvent> combatEvent;

};

#endif /* CombatEvent_h */
