//
//  CombatEvent.hpp
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#ifndef CombatEvent_hpp
#define CombatEvent_hpp

#include <stdio.h>
#include "ofMain.h"

class Actor;
class CombatEvent : public ofEventArgs {
    
public:
    
    enum EventType {
        NULL_EVENT,
        DEATH_EVENT
    };
    
    Actor * a;
    Actor * b;

    EventType type;
    
    CombatEvent() {
        a = NULL;
        b = NULL;

        EventType type = NULL_EVENT;
    }
    
    static ofEvent <CombatEvent> combatEvent;
};

#endif /* CombatEvent_hpp */
