//
//  EventLog.cpp
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#include "EventLog.h"
#include "Actor.h"
#include "Console.h"


EventLog eventLog;

EventLog::EventLog() {
    ofAddListener(ActorEvent::actorEvent, this, &EventLog::actorEvent);
    ofAddListener(CombatEvent::combatEvent, this, &EventLog::combatEvent);
}


//--------------------------------------------------------------
void EventLog::actorEvent(ActorEvent &e) {
    
    switch (e.type) {
        case ActorEvent::DEATH_EVENT: {
            
            stringstream ss;
            
            ss << e.a->getName() << " has died!";
            
            addMessage(ss.str());
            
            

            break;
        }
        default:
            break;
    }
}

//--------------------------------------------------------------
void EventLog::combatEvent(CombatEvent &e) {
    ofLog() << "combat event...";
    
    switch (e.type) {
        case CombatEvent::NULL_EVENT: {
            break;
        }
        case CombatEvent::MISS_EVENT: {
            stringstream ss;
            ss << e.a->getName() << " missed " << e.b->getName() << ".";
            addMessage(ss.str());
            ofLog() << ss;

            break;
        }
        case CombatEvent::HIT_EVENT: {
            stringstream ss;
            ss << e.a->getName() << " hit " << e.b->getName() << " for " << e.dmg << " damage.";
            addMessage(ss.str());
            ofLog() << ss;
            
            e.b->takeDamage(e.dmg);

            break;
        }
        case CombatEvent::CRIT_EVENT: {
            stringstream ss;
            ss << e.a->getName();
            ss << " critted ";
            ss << e.b->getName();
            ss << " for ";
            ss << e.dmg;
            ss << " damage!";
            addMessage(ss.str());
            
            ofLog() << ss;

            
            e.b->takeDamage(e.dmg); 

            break;
        }
        default:
            break;
    }
}

void EventLog::addMessage(string s) {
    messages.push_back(s);
    
    if (messages.size()>10) {
        messages.erase (messages.begin());
    }
    
}

void EventLog::render() {
    
    for (int i=0; i<messages.size(); i++) {
        
        
        console.writeString(2, (48-messages.size())+i, messages[i]);//, makeColor(3,3,3), 0);
        
    }
    
    
}