//
//  Engine.cpp
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#include "Engine.h"


Engine engine;


//--------------------------------------------------------------
void Engine::init() {
    
    for (int i=0; i<256; i++) {
        MapData* md = dynamic_cast<MapData*>(Object::create(Object::MapData));
        if (md) {
            mapData[i] = md;
            md->id = i;
            
            if (i==2) {
                md->data.ambient = 255;
            } if (i==3) {
                md->data.ambient = 255;
            }
        }
    }
    
    map = new Map(512,512);
    
    reset();
    
    ofAddListener(ActorEvent::actorEvent, this, &Engine::actorEvent);

}

//--------------------------------------------------------------
void Engine::reset() {
    
    serializer.load();
    
    ofLog() << "elements before prune: " << Object::elements().size();
    
    /*
     for (auto it=Object::elements().begin();
     it!=Object::elements().end();
     ){
     
     Item * item = dynamic_cast<Item*>(*it);
     
     bool cont = true;
     
     if (item) {
     if(item->z == VOID_LOCATION) {
     delete *it;
     Object::elements().erase(it);
     }
     
     }
     
     
     if (cont)
     ++it;
     }
     
     ofLog() << "elements after prune: " << Object::elements().size();
     */
    
    delete player;
    player = NULL;
    
    player = static_cast<Player *>(Object::create(Object::Player));
    player->x = map->startingPosition.x;
    player->y = map->startingPosition.y;
    player->z = 1;
    player->init();
    
    map->mapNumber = player->z;
    map->load();
    
    map->setWindow(player->x-40,player->x-25);
    
    
}




//--------------------------------------------------------------
void Engine::actorEvent(ActorEvent &e) {
    
    switch (e.type) {
        case ActorEvent::DEATH_EVENT: {
            if (e.a == player) {
                ofLog() << "You have been killed!!";
            } else {
                if (fireTargeting) {
                    toggleFiring();
                }
            }
            break;
        }
        default:
            break;
    }
}



//--------------------------------------------------------------
void Engine::resolveTurn() {
    
    resolvingTurn = true;
    updateIndex = 0;
    updateList.clear();
    
    for (int i=0; i<Object::elements().size(); i++) {
        Object * o = Object::elements()[i];
        
        if (o->z== map->mapNumber) {
            updateList.push_back(o);
        }
    }
    
}



//--------------------------------------------------------------
void Engine::toggleFiring() {
    
    
    if (fireTargeting) {
        fireTargeting = false;
        firingList.clear();
        firingIndex = 0;
        ofLog()<< "toggleFiring off";
        
    } else {
        
        firingIndex = 0;
        ofLog()<< "toggleFiring building new index";
        
        if (player->hasWeaponTypeEquipped(Weapon::PROJECTILE_WEAPON) ||
            player->hasWeaponTypeEquipped(Weapon::THROWN_WEAPON)) {
            
            
            ofLog()<< "found weapon of type";
            
            
            // create a list of all possible firing targets
            
            float closestDistance = 10000.0f;
            bool usingLastTarget = false;
            
            for (int i=0; i<Object::elements().size(); i++) {
                Object * o = Object::elements()[i];
                
                if (o->z == map->mapNumber) {
                    
                    if (o != player) {
                        Actor* a = dynamic_cast<Actor*>(o);
                        
                        if (a) {
                            ofVec2i pp = core->worldToWindow(ofVec2i(o->x, o->y));
                            if ((pp.x>=0) && (pp.x<CONSOLE_WIDTH) && (pp.y>=0) && (pp.y<CONSOLE_HEIGHT)) {
                                if (vis[(int)pp.x+((int)pp.y*CONSOLE_WIDTH)]>0) {
                                    
                                    ofLog() << "adding " << a->getName() << " to firing list";
                                    firingList.push_back(a);
                                    
                                    if (a==player->target) {
                                        firingIndex = firingList.size()-1;
                                        usingLastTarget = true;
                                        ofLog() << "using last target";
                                    } else if (!usingLastTarget) {
                                        float dist = a->getPos().distance(player->getPos());
                                        if (closestDistance > dist) {
                                            closestDistance = dist;
                                            firingIndex = firingList.size()-1;
                                            ofLog() << "found closer target";
                                            
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
            
            // don't follow through unless there are actual targets to show
            if (firingList.size()>0) {
                fireTargeting = true;
                if (!usingLastTarget) {
                    ofLog() << "setting target "<<firingIndex<<" : "<< firingList[firingIndex]->getName();
                    
                    player->target = firingList[firingIndex];
                }
            }
        }
    }
}


//--------------------------------------------------------------
void Engine::transitActor(Actor * a, int x, int y, int z) {
    
    Player*p = dynamic_cast<Player*>(a);
    
    if (p) {
        
        ofLog() << "core transiting player...";
        
        p->x = x;
        p->y = y;
        p->z = z;
        
        map->mapNumber = z;
        map->load();
        
        p->graph.process();
        
        map->setWindow(player->x-40,player->x-25);
        
        
    } else {
        
        ofLog() << "core transiting NPC...";
        a->x = x;
        a->y = y;
        a->z = z;
        
    }
}


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// rendering


//--------------------------------------------------------------
void Engine::raytrace(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x = x0;
    int y = y0;
    int n = 1 + dx + dy;
    int x_inc = (x1 > x0) ? 1 : -1;
    int y_inc = (y1 > y0) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;
    
    float opaque = 0;
    
    float ambient = ((float)mapData[map->mapNumber]->data.ambient) / 255.0f;
    
    for (; n > 0; --n)
    {
        opaque += isOpaque(x, y, opaque);
        
        if (opaque>=1.0f) break;
        
        //if (opaque>0.5f) {
        opaque -= ambient * 0.04f;
        //}
        
        opaque = MAX(0.0f, opaque);
        
        if (error > 0)
        {
            x += x_inc;
            error -= dy;
        }
        else
        {
            y += y_inc;
            error += dx;
        }
    }
}


//--------------------------------------------------------------
BYTE Engine::isVisible(const ofVec2i pos) {
    return vis[pos.x+pos.y*CONSOLE_WIDTH];
}


//--------------------------------------------------------------
float Engine::isOpaque(int x, int y, float currentOpaque) {
    vis[x+y*CONSOLE_WIDTH] = MAX( vis[x+y*CONSOLE_WIDTH], 255 - (currentOpaque*255.0f));
    return (map->isWindowOpaque(x,y));
}

//--------------------------------------------------------------
bool Engine::losCheck(Object * a, Object * b) {
    
    int x0 = a->x;
    int y0 = a->y;
    int x1 = b->x;
    int y1 = b->y;
    
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    
    for(;;){
        
        ofVec2i pp = core->worldToWindow(ofVec2i(x0,y0));
        
        if (vis[pp.x+pp.y*CONSOLE_WIDTH]<127) {
            return false;
        }
        
        if (x0==x1 && y0==y1) return true;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
    
    return true;
    
}

//--------------------------------------------------------------
float Engine::calcLuma(int x, int y) {
    float amp = torchBrightness;
    float radius = visionRadius;
    
    ofVec2i windowPos = ofVec2i(x,y);
    
    float att = 0.0f;
    float luma = 0.0f;
    
    
    BYTE visible = isVisible(windowPos);
    if (core->state == EDIT_STATE) {
        visible = 255;
        ambient = 1.0;
        luma = 1.0f;
    } else {
        if (visible>0) {
            
            //radius += (float)ambient * 0.25f;
            
            float d = windowPos.distance(core->worldToWindow(player->getPos()) +flickerOffset);
            att = clamp(1.0 - ((d*d)/(radius*radius)), 0.0, 1.0);
            att *= att;
            luma = (att * flickerValue);
            luma *= 1.3f;
            luma += ambient;
            luma *= ((float)visible) / 255.0f;
            luma = MIN(luma, 1.0f);
            luma = MAX(luma, 0.0f);
        }
    }
    
    return luma;
    
}


//--------------------------------------------------------------
DEBT Engine::traversable(int dx, int dy) {
    
    DEBT d = map->traversable(dx, dy);
    if (d>0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object* o = Object::elements()[i];
            if (o->z == map->mapNumber) {
                if (o->x == dx) {
                    if (o->y == dy) {
                        if (o->traversable()==TRAVERSE_BLOCKED) {
                            d = 0;
                        } else {
                            d = MAX(d,o->traversable());
                        }
                    }
                }
            }
        }
    }
    
    return d;
    
}


void Engine::preRender() {
    // reset visibility
    for (int i=0; i<CONSOLE_WIDTH*CONSOLE_HEIGHT; i++) {
        vis[i]=0;
    }
    
    
    ofPoint tl =map->window.getTopLeft();
    ofVec2i pp = player->getPos() - ofVec2i(tl.x, tl.y);
    
    // calculate visibilty from character position
    for (int i=0; i<CONSOLE_HEIGHT; i++) {
        raytrace(pp.x, pp.y, 0,i);
        raytrace(pp.x, pp.y, 79,i);
    }
    
    for (int j=0; j<CONSOLE_WIDTH; j++) {
        raytrace(pp.x, pp.y, j, 0);
        raytrace(pp.x, pp.y, j, 49);
    }
    
    // post processing, fix visibility problem with walls
    
    for (int y=1; y<CONSOLE_HEIGHT-1; y++) {
        for (int x=1; x<CONSOLE_WIDTH-1; x++) {
            int index = y*CONSOLE_WIDTH+x;
            if (vis[index]<255 && map->isWindowOpaque(x,y)) {
                // visibility is off here
                if (x<pp.x) {
                    if (!map->isWindowOpaque(x+1,y)) {
                        if (vis[index+1]==255) {
                            vis[index] = 255;
                        }
                    }
                } else if (x>pp.x) {
                    if (!map->isWindowOpaque(x-1,y)) {
                        if (vis[index-1]==255) {
                            vis[index] = 255;
                        }
                    }
                }
                
                
                if (y<pp.y) {
                    if (!map->isWindowOpaque(x,y+1)) {
                        if (vis[index+CONSOLE_WIDTH]==255) {
                            vis[index] = 255;
                        }
                    }
                } else if (y>pp.y) {
                    if (!map->isWindowOpaque(x,y-1)) {
                        if (vis[index-CONSOLE_WIDTH]==255) {
                            vis[index] = 255;
                            
                        }
                    }
                }
                
                // fix corners
                
                if (!map->isWindowOpaque(x+1,y+1) && (vis[index+(CONSOLE_WIDTH+1)]==255))
                    vis[index] = 255;
                if (!map->isWindowOpaque(x+1,y-1) && (vis[index-(CONSOLE_WIDTH-1)]==255))
                    vis[index] = 255;
                if (!map->isWindowOpaque(x-1,y+1) && (vis[index+(CONSOLE_WIDTH-1)]==255))
                    vis[index] = 255;
                if (!map->isWindowOpaque(x-1,y-1) && (vis[index-(CONSOLE_WIDTH+1)]==255))
                    vis[index] = 255;
            }
        }
    }
}


//--------------------------------------------------------------
void Engine::setFullVisibility() {
    for (int i=0; i<CONSOLE_WIDTH*CONSOLE_HEIGHT; i++) {
        vis[i]=255;
    }
}


//--------------------------------------------------------------
void Engine::renderWorld() {
    
    float amp = torchBrightness;
    float radius = visionRadius;
    
    ambient = ((float)mapData[map->mapNumber]->data.ambient) / 255.0f;
    
    
    for (int y=0; y<CONSOLE_HEIGHT; y++) {
        for (int x=0; x<CONSOLE_WIDTH; x++) {
            
            ofVec2i worldPos = core->windowToWorld(ofVec2i(x,y));
            
            float luma = calcLuma(x, y);
            
            
            // if we can see it, refresh our memory
            if (luma>0) map->setWindowSeen(x,y);
            
            // if we've seen it recently (or now), let's render it.
            if ( map->getWindowLastSeen(x,y) < 100) {
                
                Pixel p;
                
                Tile * t = map->getWindowTile(x,y);
                
                if (t->mat != 0) {
                    p = (*t->mat)(worldPos, luma);
                    console.setPixel(x,y, p.fg, p.bg, p.c);
                } else {
                    console.setPixel(x,y, makeColor(5,5,0),makeColor(5,0,0),toascii('X'));
                }
                
            } else {
                console.setPixel(x,y, makeColor(1,1,1),0,252);
            }
        }
        
    }
    
    
    /*
     for (int i=0; i<trail.size(); i++) {
     ofVec2i pp = trail[i] - map->window.getTopLeft();
     console.setPixel(pp.x, pp.y, makeColor(1,0,2), 0, 243);
     }
     */
    
    
    for (int i=0; i<Object::elements().size(); i++) {
        Object * o = Object::elements()[i];
        
        if (o->z == map->mapNumber) {
            ofVec2i pp = core->worldToWindow(ofVec2i(o->x, o->y));
            if ((pp.x>=0) && (pp.x<CONSOLE_WIDTH) && (pp.y>=0) && (pp.y<CONSOLE_HEIGHT)) {
                
                
                if (calcLuma(pp.x, pp.y)>0.5f) {
                    Pixel p = o->render(1.0f);
                    console.setPixel(pp.x, pp.y, p);
                }
            }
        }
    }
    
    
    ofVec2i pp = core->worldToWindow(ofVec2i(player->x, player->y));
    if ((pp.x>=0) && (pp.x<CONSOLE_WIDTH) && (pp.y>=0) && (pp.y<CONSOLE_HEIGHT)) {
        if (calcLuma(pp.x, pp.y)>0.5f) {
            Pixel p = player->render(1.0f);
            console.setPixel(pp.x, pp.y, p);
        }
    }
    
    
    
    if (fireTargeting) {
        reticleTime += core->deltaTime;
        
        Actor * a = firingList[firingIndex];
        
        ofVec2i pp = core->worldToWindow(ofVec2i(a->x, a->y));
        
        double i;
        float f = modf(reticleTime * 1.5f, &i);
        
        
        BYTE v = int(f*6.0f);
        Pixel p(0, makeColor(0,v, v), 0, 1);
        console.setPixel(pp, p);
        
        /*
         
         BYTE reticleIndex = int(f * 8.0f);
         
         ofVec2i reticle;
         BYTE c;
         switch (reticleIndex) {
         case 0:
         reticle = ofVec2i(-1,0);
         c=11;
         break;
         case 1:
         reticle = ofVec2i(-1,-1);
         c=3;
         break;
         case 2:
         reticle = ofVec2i(0,-1);
         c=12;
         break;
         case 3:
         reticle = ofVec2i(1,-1);
         c=1;
         break;
         case 4:
         reticle = ofVec2i(1,0);
         c=9;
         break;
         case 5:
         reticle = ofVec2i(1,1);
         c=2;
         break;
         case 6:
         reticle = ofVec2i(0,1);
         c=10;
         break;
         case 7:
         reticle = ofVec2i(-1,1);
         c=4;
         break;
         
         }
         
         
         Pixel p(makeColor(1,5,5), 0, c, 0);
         console.setPixel(pp + reticle, p);
         */
        
        
    }
    
    
}




//--------------------------------------------------------------
void Engine::update(float deltaTime) {
    
    
    if (torch) {
        torchBrightness += (1.1f - torchBrightness) * 0.1f;
        visionRadius += (16 - visionRadius) * 0.2f;
    } else {
        torchBrightness += (0.6f - torchBrightness) * 0.1f;
        visionRadius += (2 - visionRadius) * 0.2f;
    }
 
    
    // normal state
    
    
    if (core->playing) {
        if (actionDebt>=0) actionDebt -= 12.0f * 60.0f * deltaTime;
    } else {
        actionDebt = 0;
    }
    
    
    if (fireTargeting) {
        if (player->tooFarForRanged()) {
            toggleFiring();
        }
    }
    
    if (resolvingTurn) {
        
        if (resolveDelay<=0) {
            
            if (!vfx.busy()) {
                
                float d = 0.0f;
                
                DEBT leastDebt = 0;
                while (d==0 && updateIndex<updateList.size()) {
                    
                    Object * o = updateList[updateIndex];
                    
                    Actor * a = dynamic_cast<Actor*>(o);
                    if (a) {
                        
                        //ofLog() << "updating actor " << o->getName() << " at index: "<<updateIndex << " with " << updateDebt << " debt (currently has: "<<a->actionDebt<<")";
                        Player * p = dynamic_cast<Player*>(o);
                        
                        if (p || core->playing) {
                            d = o->update(updateDebt);
                        }
                        
                        
                        //ofLog() << "actor now has " << a->actionDebt << " debt";
                        
                        leastDebt = MIN(leastDebt, a->actionDebt);
                        
                        
                    }
                    
                    updateIndex++;
                }
                
                
                
                if (updateIndex>=updateList.size()) {
                    
                    if (player->actionDebt>=100) {
                        
                        updateIndex = 0;
                        updateDebt = 100;
                        
                        
                    } else if (leastDebt<0) {
                        
                        updateIndex = 0;
                        updateDebt = 0;
                        
                    } else {
                        
                        resolvingTurn = false;
                        
                        for (int i=0; i<updateList.size(); i++) {
                            
                            Actor *a = dynamic_cast<Actor*>(updateList[i]);
                            if (a) a->cleanup();
                        }
                        
                    }
                    
                }
            }
            
            
        } else {
            
            resolveDelay -= deltaTime;
        }
        
    } else {
        
        
        
        if (core->controls.active() || player->autoTravel ) {
            
            core->mouseHide = true;
            
            
            bool vert = false;
            
            if (actionDebt<=0) {
                
                //ofLog() << "####################################################";
                
                /*
                trail.push_back(player->getPos());
                if (trail.size()>30) {
                    trail.erase (trail.begin());
                }
                 */
                
                ofVec2i moveVector;
                
                DEBT currentDebt = player->actionDebt;
                
                
                
                if (player->autoTravel) {
                    player->graph.setWorldCenter(player->x, player->y);
                    player->graph.process();
                    
                    moveVector = player->graph.getMoveVector();
                    
                } else {
                    if (core->controls.up) {
                        moveVector.y = -1;
                    } else if (core->controls.down) {
                        moveVector.y = 1;
                    }
                    
                    if (core->controls.left) {
                        moveVector.x = -1;
                    } else if (core->controls.right) {
                        moveVector.x = 1;
                    }
                    
                }
                
                dialog.active = false;
                
                if (moveVector.length()>0) {
                    player->tryInteracting(moveVector);
                }
                
                if (core->controls.fire) {
                    ofLog()<< "controls.fire";
                    
                    if (player->weaponSet==0) {
                        player->switchWeapons();
                    }
                    if (player->target != NULL){
                        player->attack(player->target);
                    }
                    core->controls.fire = false;
                }
                
                if (core->controls.tab) {
                    ofLog() << "controls.tab";
                    player->switchWeapons();
                    core->controls.tab = false;
                }
                
                resolveDelay = MIN(0.2f,((float)newPlayerDebt) / TIME_TO_DEBT_SCALAR);
                
                newPlayerDebt = player->actionDebt-currentDebt;
                if (newPlayerDebt==0) {
                    player->tryMoving(moveVector);
                }
                
                newPlayerDebt = player->actionDebt-currentDebt;
                
                if (!core->controlsHaveBeenActive) {
                    if (newPlayerDebt>0) {
                        actionDebt += 100;
                    }
                    
                }
                
                actionDebt += newPlayerDebt;
                
                if (core->controls.rest) {
                    actionDebt += 100;
                }
                
                // here we are arbitrarily updating every element. should change this to be the following sequence:
                
                // CHECK - lock controls
                
                // CHECK - create vector of items on map
                
                // --- sort vector based on initiative first, ambient/not engaged in combat second
                
                // for each element in vector:
                
                // CHECK --- update with new player debt
                // --- all each element to generate events if necessary
                // CHECK --- once event chain has completed, go to next element
                
                // CHECK unlock controls
                
                
                if (player->actionDebt>=100) {
                    updateDebt = 100;
                    
                    resolveTurn();
                }
                
                newPlayerDebt = 0;
                
                map->upkeep();
                
            }
            
            core->controlsHaveBeenActive = true;
            
            
        } else {
            core->controlsHaveBeenActive = false;
        }
        
        if (actionDebt<0) {
            flickerOffset = ofVec2i(ofRandomuf()-0.5f, ofRandomuf()-0.5f) ;
            flickerValue = (ofRandomuf()*0.02f)+0.9f;
            actionDebt+=100;
        }
        
    }
    
    
    player->renderUpdate();


    
}


