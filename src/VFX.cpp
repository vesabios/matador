//
//  VFX.cpp
//  matador
//
//  Created by vesabios on 10/19/15.
//
//

#include "VFX.hpp"
#include "Core.h"
#include "Actor.h"

VFX vfx;

void VFX::init() {
    
    ofAddListener(CombatEvent::combatEvent, this, &VFX::combatEvent);

}

//--------------------------------------------------------------
void VFX::combatEvent(CombatEvent &e) {
    
    switch (e.type) {
        case CombatEvent::NULL_EVENT: {
            

            
            break;
        }
        case CombatEvent::MISS_EVENT: {
            break;
        }
        case CombatEvent::ARROW_EVENT: {
            
            Actor * a = static_cast<Actor*>(e.a);
            Actor * b = static_cast<Actor*>(e.b);
            
            ofVec2i pa = ofVec2i(a->x, a->y);
            ofVec2i pb = ofVec2i(b->x, b->y);
            
            if (e.nextEvent->type == CombatEvent::MISS_EVENT) {
                pb += ofVec2i(round(ofRandom(-2, 2)), round(ofRandom(-2, 2)));
            }

            ofPoint tl = engine.map->window.getTopLeft();

            pa = pa - ofVec2i(tl.x, tl.y);
            pb = pb - ofVec2i(tl.x, tl.y);
            
            VFXArrow * fx = new VFXArrow();
            
            fx->pa = pa;
            fx->pb = pb;
            
            fx->nextEvent = e.nextEvent;

            ofLog() << "----------------- VFX ARROW";

            items.push_back( fx);
            break;
        }
        case CombatEvent::HIT_EVENT: {
            
            Actor * a = static_cast<Actor*>(e.b);
            
            ofVec2i p = ofVec2i(a->x, a->y);
            
            ofPoint tl = engine.map->window.getTopLeft();
            p = p - ofVec2i(tl.x, tl.y);
            
            VFXHit * fx = new VFXHit();
            
            fx->p = p;
            
            ofLog() << "----------------- VFX HIT";
            
            items.push_back( fx);
            
            
            VFXFloater * floater = new VFXFloater();
            floater->dmg = e.dmg;
            floater->p = p - ofVec2i(0,1);
            items.push_back(floater);
            
            break;
        }
        case CombatEvent::CRIT_EVENT: {
            
            Actor * a = static_cast<Actor*>(e.b);
            
            ofVec2i p = ofVec2i(a->x, a->y);
            
            ofPoint tl = engine.map->window.getTopLeft();
            p = p - ofVec2i(tl.x, tl.y);
            
            VFXHit * fx = new VFXHit();
            
            fx->p = p;
            
            ofLog() << "----------------- VFX CRIT";
            
            items.push_back( fx);
            
            
            VFXFloater * floater = new VFXFloater();
            floater->dmg = e.dmg;
            floater->p = p - ofVec2i(0,1);
            items.push_back(floater);
            
            
            break;
        }
        default:
            break;
    }
}