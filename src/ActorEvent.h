//
//  ActorEvent.h
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#ifndef ActorEvent_h
#define ActorEvent_h
#pragma once

#include <stdio.h>
#include "ofEvents.h"


class Actor;
class ActorEvent : public ofEventArgs {
    
public:
    
    enum EventType {
        NULL_EVENT,
        DEATH_EVENT
    };
    
    Actor * a;
    EventType type;
    
    ActorEvent() {
        a = NULL;
        EventType type = NULL_EVENT;
    }
    
    static ofEvent <ActorEvent> actorEvent;
};
#endif /* ActorEvent_h */
