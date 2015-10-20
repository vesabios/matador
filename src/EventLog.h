//
//  EventLog.h
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#ifndef EventLog_h
#define EventLog_h

#include <stdio.h>
#include "ofEvents.h"
#include "CombatEvent.h"
#include "ActorEvent.h"

class EventLog {
public:
    
    EventLog();
    
    vector<string> messages;
    
    void actorEvent(ActorEvent &e);
    void combatEvent(CombatEvent &e);
    
    void addMessage(string s);

    void render();
    
};

extern EventLog eventLog;

#endif /* EventLog_h */
