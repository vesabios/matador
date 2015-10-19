#include <ctype.h>
#include "Core.h"
#include "Materials.h"
#include "ObjectFactory.h"
#include "Engine.h"
#include "Console.h"
#include "Menu.h"
#include "Item.h"
#define STRINGIFY(A) #A

extern Engine engine;
extern Console console;
extern Menu menu;



//--------------------------------------------------------------
void Core::setup(){
    
    
    gui.setup();
    
    
    materials.mats[Material::DenseForest]->aFreq = &aFreq;
    materials.mats[Material::DenseForest]->aAmp = &aAmp;
    materials.mats[Material::DenseForest]->aOffset = &aOffset;
    materials.mats[Material::DenseForest]->bFreq = &bFreq;
    materials.mats[Material::DenseForest]->bAmp = &bAmp;
    materials.mats[Material::DenseForest]->bOffset = &bOffset;
    materials.mats[Material::DenseForest]->cFreq = &cFreq;
    materials.mats[Material::DenseForest]->cAmp = &cAmp;
    materials.mats[Material::DenseForest]->cOffset = &cOffset;
    
    gui.add(aFreq.setup("Freq A",0.5,0,2));
    gui.add(aAmp.setup("Amp A",0.5,0,10));
    gui.add(aOffset.setup("Offset A",0,-2,2 ));
    gui.add(bFreq.setup("Freq B",0.5,0,2));
    gui.add(bAmp.setup("Amp B",0.5,0,10));
    gui.add(bOffset.setup("Offset B",0,-2,2 ));
    gui.add(cFreq.setup("Freq C",0.5,0,2));
    gui.add(cAmp.setup("Amp C",0.5,0,10));
    gui.add(cOffset.setup("Offset C",0,-2,2 ));

 
    console.init();
    engine.init();
    
    state = NORMAL_STATE;
    
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
    
    // create the player!
    /*
    
    player = static_cast<Player *>(Object::create(Object::Player));
    player->x = map->startingPosition.x;
    player->y = map->startingPosition.y;
    player->z = map->mapNumber;
    player->init();
    */
    reset();

    map->setWindow(player->x-40,player->x-25);

    
    
    //populateMap();
    //sound.loadSound("afro_afro.mod");
    //sound.play();
    

    ofAddListener(ActorEvent::actorEvent, this, &Core::actorEvent);


    
}


//--------------------------------------------------------------
Object * Core::placeObject(int x, int y, int mapNumber, Object::ObjectType it) {
    
    printf("placing object: %d\n", it);
    
    Object * o = Object::create((Object::ObjectType)it);
    
    o->init();
    o->x = x;
    o->y = y;
    o->z = mapNumber;
    
    return o;
    
    
}


//--------------------------------------------------------------
void Core::populateMap() {
    
    for (int i=0; i<1000; i++) {
        
        int x = ofRandom(311)+100;
        int y = ofRandom(311)+100;
        
        if (map->traversable(x,y) > 0) {
            
            /*
            Actor a;
            
            a.pos = ofVec2i(x,y);
            actors.push_back(a);
            */
            
        };
    }
 
}


//--------------------------------------------------------------
void Core::writeString(int x, int y, string s) {
    
    int sz = s.length();
    
    for (int i=0; i<sz; i++) {
        console.setPixel(x+i, y, makeColor(5,5,2), makeColor(0,2,2), s.at(i));
    }
    
}


//--------------------------------------------------------------
void Core::writeString(int x, int y, string s, BYTE fg, BYTE bg) {
    
    int sz = s.length();
    
    
    for (int i=0; i<sz; i++) {
        console.setPixel(x+i, y, fg, bg, s.at(i));
    }
    
    
}





//--------------------------------------------------------------
void Core::raytrace(int x0, int y0, int x1, int y1)
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
    
    for (; n > 0; --n)
    {
        opaque += isOpaque(x, y, opaque);
        if (opaque>=1.0f) break;
        
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
float Core::isOpaque(int x, int y, float currentOpaque) {
    vis[x+y*CONSOLE_WIDTH]=255 - (currentOpaque*255.0f);
    return (map->isWindowOpaque(x,y));
}



//--------------------------------------------------------------
void Core::renderWorld() {
    
    float amp = torchBrightness;
    float radius = visionRadius;
    
    for (int y=0; y<CONSOLE_HEIGHT; y++) {
        for (int x=0; x<CONSOLE_WIDTH; x++) {
            
            ofVec2i windowPos = ofVec2i(x,y);
            ofVec2i worldPos = windowToWorld(ofVec2i(x,y));
            
            float att = 0.0f;
            float luma = 0.0f;
            float ambient = 0.0f;
            
            ambient = ((float)mapData[map->mapNumber]->data.ambient) / 255.0f;
            
            BYTE visible = isVisible(windowPos);
            if (state == EDIT_STATE) {
                visible = 255;
                ambient = 1.0;
            }
            
            if (visible>0) {
                
                //radius += (float)ambient * 0.25f;
                
                float d = windowPos.distance(worldToWindow(player->getPos()) +flickerOffset);
                att = clamp(1.0 - ((d*d)/(radius*radius)), 0.0, 1.0);
                att *= att;
                luma = (att * flickerValue);
                luma *= 1.3f;
                luma += ambient;
                luma *= ((float)visible) / 255.0f;
                luma = MIN(luma, 1.0f);
                luma = MAX(luma, 0.0f);
            }
            
            
            // if we can see it, refresh our memory
            if (luma>0) map->setWindowSeen(x,y);
            
            // if we've seen it recently (or now), let's render it.
            if ( map->getWindowLastSeen(x,y) < 2800) {
         
                Pixel p;
                
                Tile * t = map->getWindowTile(x,y);
                
                if (t->mat != 0) {
                    p = (*t->mat)(worldPos, luma);
                    console.setPixel(x,y, p.fg, p.bg, p.c);
                } else {
                    console.setPixel(x,y, makeColor(5,5,0),makeColor(5,0,0),toascii('X'));
                }

            } else {
                console.setPixel(x,y, 0,0,0);
     
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
            ofVec2i pp = worldToWindow(ofVec2i(o->x, o->y));
            if ((pp.x>=0) && (pp.x<CONSOLE_WIDTH) && (pp.y>=0) && (pp.y<CONSOLE_HEIGHT)) {
                if (vis[(int)pp.x+((int)pp.y*CONSOLE_WIDTH)]>0) {
                    Pixel p = o->render(1.0f);
                    console.setPixel(pp.x, pp.y, p);
                }
            }
        }
    }
    
    
    ofVec2i pp = worldToWindow(ofVec2i(player->x, player->y));
    if ((pp.x>=0) && (pp.x<CONSOLE_WIDTH) && (pp.y>=0) && (pp.y<CONSOLE_HEIGHT)) {
        if (vis[(int)pp.x+((int)pp.y*CONSOLE_WIDTH)]>0) {
            Pixel p = player->render(1.0f);
            console.setPixel(pp.x, pp.y, p);
        }
    }
    
    
    
    if (fireTargeting) {
        
        Actor * a = firingList[firingIndex];
        
        ofVec2i pp = worldToWindow(ofVec2i(a->x, a->y));
        
        double i;
        float f = modf(ofGetElapsedTimef() * 1.5f, &i);
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
        
        Pixel p(makeColor(5,5,0), 0, c, 0);
        console.setPixel(pp + reticle, p);
        
        
    }
    
    
}




DEBT Core::traversable(int dx, int dy) {
    
    DEBT d = map->traversable(dx, dy);
    if (d>0) {
        for (int i=0; i<Object::elements().size(); i++) {
            Object* o = Object::elements()[i];
            
           // ofLog()<< "checking " << o->getName() << "..." ;
            
           // ofLog() << o->z << " == " << map->mapNumber;
            
            if (o->z == map->mapNumber) {
                //ofLog()<< "map..." ;

                if (o->x == dx) {
                    //ofLog()<< "x..." ;
                    if (o->y == dy) {
                        //ofLog()<< "y..." ;

                        if (o->traversable()==TRAVERSE_BLOCKED) {
                            //ofLog()<< "blocked" << endl;
                            d = 0;
                        } else {
                            d = MAX(d,o->traversable());

                            ofLog()<< d << endl;
                        }
                    }
                }
            }
        }
    }
    
    return d;
                    
}


//--------------------------------------------------------------
ofVec2i Core::worldToWindow(const ofVec2i p) {
    ofPoint tl = map->window.getTopLeft();
    return p - ofVec2i(tl.x, tl.y);
}

//--------------------------------------------------------------
ofVec2i Core::windowToWorld(const ofVec2i p) {
    ofPoint tl = map->window.getTopLeft();
    return p + ofVec2i(tl.x, tl.y);
}


//--------------------------------------------------------------

BYTE Core::isVisible(const ofVec2i pos) {
    return vis[pos.x+pos.y*CONSOLE_WIDTH];
    
}

void Core::transitActor(Actor * a, int x, int y, int z) {
    
    Player*p = dynamic_cast<Player*>(a);
    
    if (p) {
        
        ofLog() << "core transiting player...";
        
        p->x = x;
        p->y = y;
        p->z = z;
        
        map->mapNumber = z;
        map->load();
        
        
    } else {
        
        ofLog() << "core transiting NPC...";
        a->x = x;
        a->y = y;
        a->z = z;
        
    }
    

    
}


void Core::toggleFiring() {
    
    if (fireTargeting) {
        fireTargeting = false;
        firingList.clear();
        firingIndex = 0;

    } else {
        
        firingIndex = 0;

        
        if (player->hasWeaponTypeEquipped(Weapon::PROJECTILE_WEAPON) ||
            player->hasWeaponTypeEquipped(Weapon::THROWN_WEAPON)) {
            
            // create a list of all possible firing targets
            
            for (int i=0; i<Object::elements().size(); i++) {
                Object * o = Object::elements()[i];
                
                if (o->z == map->mapNumber) {
                    
                    
                    if (o != player) {
                        Actor* a = dynamic_cast<Actor*>(o);
                        
                        if (a) {
                            ofVec2i pp = worldToWindow(ofVec2i(o->x, o->y));
                            if ((pp.x>=0) && (pp.x<CONSOLE_WIDTH) && (pp.y>=0) && (pp.y<CONSOLE_HEIGHT)) {
                                if (vis[(int)pp.x+((int)pp.y*CONSOLE_WIDTH)]>0) {
                                    
                                    // actor is visible
                                    
                                    // float distance = ofVec2i(player->x, player->y).distance(ofVec2i(a->x, a->y));
                                    
                                    ofLog() << "adding " << a->getName() << " to firing list";
                                    firingList.push_back(a);
                                    
                                }
                            }
                        }
                    }
                }
            }
            
            // don't follow through unless there are actual targets to show
            if (firingList.size()>0) {
                fireTargeting = true;
                player->target = firingList[firingIndex];
            }
        
        }
       
        
     }
}



//--------------------------------------------------------------
void Core::reset() {
    
    serializer.load();
    
    delete player;
    player = NULL;
    
    player = static_cast<Player *>(Object::create(Object::Player));
    player->x = map->startingPosition.x;
    player->y = map->startingPosition.y;
    player->z = 1;
    player->init();
    
    
    map->mapNumber = player->z;
    map->load();
    adjustWindow();
    
    
}

//--------------------------------------------------------------
void Core::actorEvent(ActorEvent &e) {
    
    switch (e.type) {
        case ActorEvent::DEATH_EVENT: {
            if (e.a == player) {
                ofLog() << "player has been killed!!";
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
void Core::update(){
 
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    engine.update(deltaTime);
    menu.update(deltaTime);
    paint.update(deltaTime);
    inspect.update(deltaTime);
    

    if (torch) {
        torchBrightness += (1.1f - torchBrightness) * 0.1f;
        visionRadius += (16 - visionRadius) * 0.2f;
    } else {
        torchBrightness += (0.6f - torchBrightness) * 0.1f;
        visionRadius += (2 - visionRadius) * 0.2f;
    }
    
    
    
    bool moved = false;
    
    if (state==NORMAL_STATE) {
        
        if (actionDebt>=0) actionDebt -= 15.0f * 60.0f * deltaTime;
        
        if (controls.active()) {
           

            bool vert = false;
            
            
            
            if (actionDebt<=0) {
                
                trail.push_back(player->getPos());
                if (trail.size()>30) {
                    trail.erase (trail.begin());
                }
                
                ofVec2i moveVector;
                
                if (controls.up) {
                    moveVector.y = -1;
                } else if (controls.down) {
                    moveVector.y = 1;
                }
                
                if (controls.left) {
                    moveVector.x = -1;
                } else if (controls.right) {
                    moveVector.x = 1;
                }
                
                newPlayerDebt = player->tryInteracting(moveVector);
                
                if (controls.fire) {
                    newPlayerDebt += player->attack(player->target);
                }

                
                if (newPlayerDebt>=0 && newPlayerDebt < DEBT_TURN_THRESHOLD ) {
                    newPlayerDebt += player->tryMoving(moveVector);
                    if (newPlayerDebt >= DEBT_TURN_THRESHOLD) {
                        moved = true;
                    }
                }
                
                
                if (!controlsHaveBeenActive) {
                    if (newPlayerDebt>0) {
                        actionDebt += 100;
                    }
                    
                }
                
                actionDebt += newPlayerDebt;
                
                if (controls.rest) {
                    actionDebt += 100;
                    moved = true;
                }
                
                for (int i=0; i<Object::elements().size(); i++) {
                    if (Object::elements()[i]->z== map->mapNumber) {
                        Object::elements()[i]->update(newPlayerDebt);
                    }
                }
                
                newPlayerDebt = 0;
                
                map->upkeep();
                
            }
            
            controlsHaveBeenActive = true;
            

        } else {
            controlsHaveBeenActive = false;
        }
        
        
        
        if (actionDebt<0) {
            flickerOffset = ofVec2i(ofRandomuf()-0.5f, ofRandomuf()-0.5f) ;
            flickerValue = (ofRandomuf()*0.02f)+0.9f;
            actionDebt+=800;
        }
    
    } else if (state==EDIT_STATE) {
        
        if (controls.down) {
            cursorPos.y++;
        } else if (controls.up) {
            cursorPos.y--;
        } else if (controls.left) {
            cursorPos.x--;
        } else if (controls.right) {
            cursorPos.x++;
        } else if (controls.space) {
            
            if (menu.currentMainMenu==Menu::MaterialMenu) {
                map->placeMaterial(cursorPos.x, cursorPos.y, (Material::MaterialType)map->currentMaterial);
                
            } else if (menu.currentMainMenu>=Menu::MaterialMenu) {
                placeObject(cursorPos.x, cursorPos.y, map->mapNumber, (Object::ObjectType)currentObject);
            }
            
        }
        
        controls.right = false;
        controls.left = false;
        controls.up = false;
        controls.down = false;
        controls.space = false;
        
    } else if (state==PAINT_STATE) {
        if (controls.down) {
            cursorPos.y++;
        } else if (controls.up) {
            cursorPos.y--;
        } else if (controls.left) {
            cursorPos.x--;
        } else if (controls.right) {
            cursorPos.x++;
        } else if (controls.space) {
            
            paint.placePixel(worldToWindow(cursorPos));
            
        }
        
        controls.right = false;
        controls.left = false;
        controls.up = false;
        controls.down = false;
        controls.space = false;
    }
    
    adjustWindow();

    
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
    
    for (int y=0; y<CONSOLE_HEIGHT; y++) {
        for (int x=0; x<CONSOLE_WIDTH; x++) {
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
    
    if (state != PAINT_STATE) {
        renderWorld();
    } else {
        paint.render();
    }
    
    menu.render();
    inspect.render();
    
    
    if (vis[(int)mousePos.x+(int)mousePos.y*CONSOLE_WIDTH]>0 && !map->isWindowOpaque((int)mousePos.x, (int)mousePos.y)) {
        console.setPixel(mousePos.x, mousePos.y, makeColor(5,5,5), 0, '\\');
    } else {
        console.setPixel(mousePos.x, mousePos.y, makeColor(1,1,1), 0, 'X');
    }
    
    if (state==NORMAL_STATE || state==INSPECT_STATE) {
        cursorPos = player->getPos();
    } else {
        ofVec2i pp = worldToWindow(cursorPos);
        console.setPixel(pp.x, pp.y, makeColor(5,5,0), makeColor(5,0,0), 'X');
    }
    

    
    if (state==NORMAL_STATE || state==EQUIP_STATE) {
        
        console.setPixel(3, 3, makeColor(4,0,0), 0, CHAR_HEART);
        
        char buf[255];
        
        sprintf(buf,"%d/%d HP", player->data.hp, player->data.maxhp);
        
        writeString(5,3,buf, makeColor(3,3,3), 0);
        
        console.setPixel(3, 4, makeColor(4,4,1), 0, toascii('0'));
        writeString(5,4,"0 GP", makeColor(3,3,3), 0);
        
        //writeString(5,6,"Ruins of Matador", makeColor(4,4,4), 0);
        
    } else if (state==EDIT_STATE) {
   
        writeString(5,3,ofToString((int)cursorPos.x), makeColor(5,5,3), 0);
        writeString(10,3,ofToString((int)cursorPos.y), makeColor(5,5,3), 0);
        
    }

    
    
    if (state==EQUIP_STATE) {
        equip.render();
    }
    
    
    
}


//--------------------------------------------------------------
void Core::adjustWindow() {
    
    if (state==NORMAL_STATE || state==EQUIP_STATE) {
        
        while ((player->x - map->window.getMinX()) < 35) {
            map->window.translate(-1,0);
        }
        
        while ((map->window.getMaxX() - player->x) < 35) {
            map->window.translate(1,0);
        }
        
        while ((player->y - map->window.getMinY()) < 20) {
            map->window.translate(0,-1);
        }
        
        while((map->window.getMaxY() - player->y) < 20) {
            map->window.translate(0,1);
        }
        
    } else {
        map->window.x = cursorPos.x - CONSOLE_WIDTH/2;
        map->window.y = cursorPos.y - CONSOLE_HEIGHT/2;
    }
    
    
    

}


//--------------------------------------------------------------
void Core::draw(){
    
    ofBackground(0);
    ofFill();
    ofSetColor(255,255,255);
    
    console.render();
    
    //gui.draw();

}


//--------------------------------------------------------------
void Core::keyPressed(int key){
    
    if (state==EQUIP_STATE) {
        
        equip.keyPressed(key);
        
    } else if (state==NORMAL_STATE || state==EDIT_STATE || state==PAINT_STATE || state==INSPECT_STATE) {
        
        if (!controls.active()) {
            actionDebt = 0;
        }
        
        if (key=='e') {
            state=EQUIP_STATE;
        }
        
        if (key=='f') {
            if (fireTargeting) {
                controls.fire = true;
            } else {
                toggleFiring();
            }
        } else if (key == OF_KEY_TAB) {
            if (fireTargeting) {
                firingIndex++;
                firingIndex %= firingList.size();
                player->target = firingList[firingIndex];
            }
        } else {
            if (fireTargeting) {
                toggleFiring();
            }
        }
        
        if (key == OF_KEY_LEFT) {
            controls.left = true;
        } else if (key== OF_KEY_RIGHT) {
            controls.right = true;
        } else if (key== OF_KEY_UP) {
            controls.up = true;
        } else if (key== OF_KEY_DOWN) {
            controls.down = true;
        } else if (key == 't') {
            torch = !torch;
        } else if (key == 'r') {
            reset();
            //controls.rest = true;
        } else if (key==' ') {
            controls.space = true;
        } else if (key==OF_KEY_F1) {
            menu.toggle();
        } else if (key==OF_KEY_F2) {
            inspect.toggle();
        } else if (key=='s') {
            map->save();
        } else if (key=='l') {
            map->load();
        } else if (key=='z') {
            serializer.save();
        } else if (key=='x') {
            serializer.load();
            
            map->mapNumber = player->z;
            map->load();
            adjustWindow();
            
        } else if (key==OF_KEY_F3) {
            paint.toggle();
        } else if (key== OF_KEY_ESC ){
            ofGetWindowPtr()->windowShouldClose();
        }
    
    }
    
    if (state==PAINT_STATE) {
        paint.keyPressed(key);
    } else if (state==EDIT_STATE) {
        menu.keyPressed(key);
    } else if (state==INSPECT_STATE) {
        inspect.keyPressed(key);
    }

}

//--------------------------------------------------------------
void Core::keyReleased(int key) {
    if (key == OF_KEY_LEFT) {
        controls.left = false;
    } else if (key== OF_KEY_RIGHT) {
        controls.right = false;
    } else if (key== OF_KEY_UP) {
        controls.up = false;
    } else if (key== OF_KEY_DOWN) {
        controls.down = false;
    } else if (key=='f') {
        controls.fire = false;
    } else if (key == 'r') {

    } else if (key== ' ') {
        controls.space = false;
    }
    
    if (state==PAINT_STATE) {
        paint.keyReleased(key);
    } else if (state==EDIT_STATE) {
        menu.keyReleased(key);
    } else if (state==INSPECT_STATE) {
        inspect.keyReleased(key);
    }
}

//--------------------------------------------------------------
void Core::mouseMoved(int x, int y){
    
    mousePos = ofVec2i((x-1) / 16,(y-1) /16);

}

//--------------------------------------------------------------
void Core::mouseDragged(int x, int y, int button){
    
    ofVec2i np = ofVec2i((x-1) / 16,(y-1) /16);

    if (state==PAINT_STATE) {
        if (np!=mousePos) {
            paint.mouseDragged(np.x, np.y, button);
        }
    } else if (state==EDIT_STATE) {
        if (np!=mousePos) {
            menu.mouseDragged(np.x, np.y, button);
        }
    }
    
    mousePos = np;
    
}

//--------------------------------------------------------------
void Core::mousePressed(int x, int y, int button){
    
    mousePos = ofVec2i((x-1) / 16,(y-1) /16);

    if (state==EDIT_STATE) {
        menu.mousePressed(mousePos.x, mousePos.y, button);
    } else if (state==PAINT_STATE) {
        paint.mousePressed(mousePos.x, mousePos.y, button);
    } else if (state==INSPECT_STATE) {
        
        if (mousePos.x<50) {
        
            ofVec2i pp = windowToWorld(mousePos);
            bool hit = false;
            Object * io;
            
            for (int i=0; i<Object::elements().size(); i++) {
               Object * o = Object::elements()[i];
                
                if (o->z == map->mapNumber) {
                    if (o->x == (int)pp.x) {
                        if (o->y == (int)pp.y) {
                            hit = true;
                            io = o;
                        }
                    }
                }
            }
            
            inspect.inspectObject(io);
        } else {
            inspect.mousePressed(mousePos.x, mousePos.y, button);
        }
    }

}

//--------------------------------------------------------------
void Core::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void Core::windowResized(int w, int h){

}

//--------------------------------------------------------------
void Core::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void Core::dragEvent(ofDragInfo dragInfo){ 

}