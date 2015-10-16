#include <ctype.h>
#include "Core.h"
#include "Materials.h"
#include "ObjectFactory.h"
#include "Engine.h"
#include "Console.h"
#include "Menu.h"
#define STRINGIFY(A) #A

extern Engine engine;
extern Console console;
extern Menu menu;



//--------------------------------------------------------------
void Core::setup(){
    
    console.init();
    engine.init();
    
    state = NormalState;
    
    map = new Map(512,512);
    
    // create the player!
    
    player = static_pointer_cast<Player >(Object::create(Object::Player));
    
    player->x = map->startingPosition.x;
    player->y = map->startingPosition.y;
    player->z = map->mapNumber;
    
    player->init();
    
    core->objects.push_back(player);
    
    
    // give the player a weapon!
    

    

    map->setWindow(player->x-40,player->x-25);
    
    //populateMap();
    //sound.loadSound("afro_afro.mod");
    //sound.play();
    



    
}


//--------------------------------------------------------------
ofPtr<Object> Core::placeObject(int x, int y, int mapNumber, Object::ObjectType it) {
    
    printf("placing object: %d\n", it);
    
    ofPtr<Object> o = Object::create((Object::ObjectType)it);
    
    o->init();
    o->x = x;
    o->y = y;
    o->z = mapNumber;
    
    objects.push_back(o);
    
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
    
    for (; n > 0; --n)
    {
        if (isOpaque(x, y)) break;
        
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
bool Core::isOpaque(int x, int y) {
    vis[x+y*80]=1;
    return (map->isWindowOpaque(x,y));
}



//--------------------------------------------------------------
void Core::renderWorld() {
    
    float amp = torchBrightness;
    float radius = visionRadius;
    
    for (int y=0; y<50; y++) {
        for (int x=0; x<80; x++) {
            
            ofVec2i windowPos = ofVec2i(x,y);
            ofVec2i worldPos = windowToWorld(ofVec2i(x,y));
            
            float att = 0.0f;
            float luma = 0.0f;
            
            if (isVisible(windowPos)) {
                float d = windowPos.distance(worldToWindow(player->getPos()) +flickerOffset);
                att = clamp(1.0 - ((d*d)/(radius*radius)), 0.0, 1.0);
                att *= att;
                luma = (att * flickerValue);
                
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
    
    
    for (int i=0; i<objects.size(); i++) {
        ofPtr<Object> o = objects[i];
        
        if (o->z == map->mapNumber) {
            ofVec2i pp = worldToWindow(ofVec2i(o->x, o->y));
            if ((pp.x>=0) && (pp.x<80) && (pp.y>=0) && (pp.y<50)) {
                if (vis[(int)pp.x+((int)pp.y*80)]==1) {
                    
                    Pixel p = o->render(1.0f);
                    
                    console.setPixel(pp.x, pp.y, p.fg, p.bg, p.c);
                    
                }
                
            }         
        }

        
    }
    
    
    /*
    for (int i=0; i<actors.size(); i++) {
        
        Actor *a = &actors[i];
        ofVec2i pp = a->pos - map->window.getTopLeft();
        
        
        if ((pp.x>=0) && (pp.x<80) && (pp.y>=0) && (pp.y<50)) {
            
            if (vis[(int)pp.x+((int)pp.y*80)]==1) {
                
                if (ofNoise((float)i*0.4)>0.5f) {
                    console.setPixel(pp.x, pp.y-1, makeColor(5,5,5), makeColor(2,2,2), CHAR_TEXT);
                    
                    console.setPixel(pp.x, pp.y, makeColor(2,2,2), 0, toascii('h'));
                    if (playerPos == a->pos - ofVec2i(0,1)) {
                        
                        BYTE fg = makeColor(5,5,5);
                        BYTE bg = makeColor(2,2,2);
                        
                        
                        int tx = playerPos.x- map->window.getMinX() - 12;
                        int ty = playerPos.y- map->window.getMinY() - 7;
                        
                        
                        writeString(tx,ty,  "                       ", fg, bg);
                        writeString(tx,ty+1," The quick brown fox   ", fg, bg);
                        writeString(tx,ty+2," Jumped over the lazy       ", fg, bg);
                        writeString(tx,ty+3," And find his tr    ", fg, bg);
                        writeString(tx,ty+4,"                       ", fg, bg);
                    }
                } else {
                    
                    BYTE c = toascii('0');
                    int v = 5;
                    if (ofGetFrameNum()%41>20) {
                        c = toascii('!');
                        v = 2;
                        
                    }
                    
                    console.setPixel(pp.x, pp.y, makeColor(v,v,1), 0, c);
                    
                }
            }
        }
    }
    */
    
    ofPoint tl = map->window.getTopLeft();
    
    ofVec2i pp = player->getPos() - ofVec2i(tl.x, tl.y);
    console.setPixel(pp.x, pp.y, makeColor(3,3,3), 0, CHAR_PLAYER);

    
}




DEBT Core::traversable(int dx, int dy) {
    
    DEBT d = map->traversable(dx, dy);
    if (d>0) {
        for (int i=0; i<objects.size(); i++) {
            ofPtr<Object>  o = objects[i];
            
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

bool Core::isVisible(const ofVec2i pos) {
    return vis[pos.x+pos.y*80];
    
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
        visionRadius += (16 - visionRadius) * 0.02f;
    } else {
        torchBrightness += (0.6f - torchBrightness) * 0.1f;
        visionRadius += (2 - visionRadius) * 0.02f;
    }
    
    
    bool moved = false;
    
    if (state==NormalState) {
        
        if (playerMvt>=0) playerMvt -= 10.0f * 60.0f * deltaTime;
        
        if (controls.active()) {

            bool vert = false;
            
            DEBT newPlayerDebt = 0;
            
            if (playerMvt<=0) {
                
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
                
                if (newPlayerDebt < DEBT_TURN_THRESHOLD ) {
                    newPlayerDebt += player->tryMoving(moveVector);
                    if (newPlayerDebt >= DEBT_TURN_THRESHOLD) {
                        moved = true;
                    }
                }
                
                playerMvt += newPlayerDebt;
                
                if (controls.rest) {
                    playerMvt += 100;
                    moved = true;
                }
                
                for (int i=0; i<objects.size(); i++) {
                    if (objects[i]->z== map->mapNumber) {
                        objects[i]->update(newPlayerDebt);
                    }
                }
                
                map->upkeep();
                
            }
            

        }
        
        adjustWindow();
        
        if (playerMvt<0) {
            flickerOffset = ofVec2i(ofRandomuf()-0.5f, ofRandomuf()-0.5f) ;
            flickerValue = (ofRandomuf()*0.02f)+0.9f;
            playerMvt+=800;
        }
    
    } else if (state==EditState) {
        
        if (controls.down) {
            cursorPos.y++;
        } else if (controls.up) {
            cursorPos.y--;
        } else if (controls.left) {
            cursorPos.x--;
        } else if (controls.right) {
            cursorPos.x++;
        } else if (controls.space) {
            
            if (menu.currentMainMenu==0) {
                map->placeMaterial(cursorPos.x, cursorPos.y, (MaterialType)map->currentMaterial);
                
            } else if (menu.currentMainMenu==1) {
                placeObject(cursorPos.x, cursorPos.y, map->mapNumber, (Object::ObjectType)currentObject);
            }
            
        }
        
        controls.right = false;
        controls.left = false;
        controls.up = false;
        controls.down = false;
        controls.space = false;
        
    } else if (state==PaintState) {
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

    
    // reset visibility
    for (int i=0; i<80*50; i++) {
        vis[i]=0;
    }
    
    ofPoint tl =map->window.getTopLeft();
    ofVec2i pp = player->getPos() - ofVec2i(tl.x, tl.y);
    
    // calculate visibilty from character position
    for (int i=0; i<50; i++) {
        raytrace(pp.x, pp.y, 0,i);
        raytrace(pp.x, pp.y, 79,i);
    }
    
    for (int j=0; j<80; j++) {
        raytrace(pp.x, pp.y, j, 0);
        raytrace(pp.x, pp.y, j, 49);
    }
    
    // post processing, fix visibility problem with walls
    
    for (int y=0; y<50; y++) {
        for (int x=0; x<80; x++) {
            int index = y*80+x;
            if (vis[index]==0 && map->isWindowOpaque(x,y)) {
                // visibility is off here
                if (x<pp.x) {
                    if (!map->isWindowOpaque(x+1,y)) {
                        if (vis[index+1]==1) {
                            vis[index] = 1;
                        }
                    }
                } else if (x>pp.x) {
                    if (!map->isWindowOpaque(x-1,y)) {
                        if (vis[index-1]==1) {
                            vis[index] = 1;
                        }
                    }
                }
                
                
                if (y<pp.y) {
                    if (!map->isWindowOpaque(x,y+1)) {
                        if (vis[index+80]==1) {
                            vis[index] = 1;
                        }
                    }
                } else if (y>pp.y) {
                    if (!map->isWindowOpaque(x,y-1)) {
                        if (vis[index-80]==1) {
                            vis[index] = 1;

                        }
                    }
                }
                
                // fix corners
                
                if (!map->isWindowOpaque(x+1,y+1) && (vis[index+81]==1))
                    vis[index] = 1;
                if (!map->isWindowOpaque(x+1,y-1) && (vis[index-79]==1))
                    vis[index] = 1;
                if (!map->isWindowOpaque(x-1,y+1) && (vis[index+79]==1))
                    vis[index] = 1;
                if (!map->isWindowOpaque(x-1,y-1) && (vis[index-81]==1))
                    vis[index] = 1;
            }
        }
    }
    
    if (state != PaintState) {
        renderWorld();
    } else {
        paint.render();
    }
    
    menu.render();
    inspect.render();
    
    
    if (vis[(int)mousePos.x+(int)mousePos.y*80]==1 && !map->isWindowOpaque((int)mousePos.x, (int)mousePos.y)) {
        console.setPixel(mousePos.x, mousePos.y, makeColor(5,5,5), 0, '\\');
    } else {
        console.setPixel(mousePos.x, mousePos.y, makeColor(1,1,1), 0, 'X');
    }
    
    if (state==NormalState || state==InspectState) {
        cursorPos = player->getPos();
    } else {
        ofVec2i pp = worldToWindow(cursorPos);
        console.setPixel(pp.x, pp.y, makeColor(5,5,0), makeColor(5,0,0), 'X');
    }
    

    
    if (state==NormalState) {
        
        console.setPixel(3, 3, makeColor(4,0,0), 0, CHAR_HEART);
        
        
        char buf[255];
        
        sprintf(buf,"%d/%d HP", player->data.hp, player->data.maxhp);
        
        writeString(5,3,buf, makeColor(3,3,3), 0);
        
        console.setPixel(3, 4, makeColor(4,4,1), 0, toascii('0'));
        writeString(5,4,"0 GP", makeColor(3,3,3), 0);
        
        writeString(5,6,"Ruins of Matador", makeColor(4,4,4), 0);
        
    } else if (state==EditState) {
   
        writeString(5,3,ofToString((int)cursorPos.x), makeColor(5,5,3), 0);
        writeString(10,3,ofToString((int)cursorPos.y), makeColor(5,5,3), 0);

        
    }

    

    
}


//--------------------------------------------------------------
void Core::adjustWindow() {
    while ((player->x - map->window.getMinX()) < 15) {
        map->window.translate(-1,0);
    }
    
    while ((map->window.getMaxX() - player->x) < 15) {
        map->window.translate(1,0);
    }
    
    while ((player->y - map->window.getMinY()) < 15) {
        map->window.translate(0,-1);
    }
    while((map->window.getMaxY() - player->y) < 15) {
        map->window.translate(0,1);
    }
}


//--------------------------------------------------------------
void Core::draw(){
    
    ofBackground(0);
    ofFill();
    ofSetColor(255,255,255);
    
    console.render();

}


//--------------------------------------------------------------
void Core::keyPressed(int key){
    
    if (!controls.active()) {
        playerMvt = 0;
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
        controls.rest = true;
    } else if (key==' ') {
        controls.space = true;
    }
    
    if (key=='e') {
        menu.toggle();
    } else if (key=='i') {
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
        
    } else if (key=='p') {
        paint.toggle();
    }
    
    if (state==PaintState) {
        paint.keyPressed(key);
    } else if (state==InspectState) {
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
    } else if (key == 'r') {
        controls.rest = false;
    } else if (key== ' ') {
        controls.space = false;
    }
    
    if (state==PaintState) {
        paint.keyReleased(key);
    } else if (state==InspectState) {
        paint.keyReleased(key);
    }
}

//--------------------------------------------------------------
void Core::mouseMoved(int x, int y){
    
    mousePos = ofVec2i((x-1) / 16,(y-1) /16);

}

//--------------------------------------------------------------
void Core::mouseDragged(int x, int y, int button){
    
    ofVec2i np = ofVec2i((x-1) / 16,(y-1) /16);

    if (state==PaintState) {
        if (np!=mousePos) {
            paint.mouseDragged(np.x, np.y, button);
        }
    }
    
    mousePos = np;
    
}

//--------------------------------------------------------------
void Core::mousePressed(int x, int y, int button){
    
    mousePos = ofVec2i((x-1) / 16,(y-1) /16);

    if (state==EditState) {
        menu.mousePressed(mousePos.x, mousePos.y, button);
    } else if (state==PaintState) {
        paint.mousePressed(mousePos.x, mousePos.y, button);
    } else if (state==InspectState) {
        
        if (mousePos.x<50) {
        
            ofVec2i pp = windowToWorld(mousePos);
            bool hit = false;
            ofPtr<Object> io;
            
            for (int i=0; i<objects.size(); i++) {
               ofPtr<Object> o = objects[i];
                
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