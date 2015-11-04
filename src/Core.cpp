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


    vfx.init();
    console.init();
    engine.init();
    
    state = NORMAL_STATE;
    
    //sound.loadSound("afro_afro.mod");
    //sound.play();

    
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
ofVec2i Core::worldToWindow(const ofVec2i p) {
    ofPoint tl = engine.map->window.getTopLeft();
    return p - ofVec2i(tl.x, tl.y);
}

//--------------------------------------------------------------
ofVec2i Core::windowToWorld(const ofVec2i p) {
    ofPoint tl = engine.map->window.getTopLeft();
    return p + ofVec2i(tl.x, tl.y);
}



//--------------------------------------------------------------
void Core::update(){
 
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    menu.update(deltaTime);
    paint.update(deltaTime);
    inspect.update(deltaTime);
    
    if (state==NORMAL_STATE) {
        
        engine.update(deltaTime);

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
                engine.map->placeMaterial(cursorPos.x, cursorPos.y, (Material::MaterialType)engine.map->currentMaterial);
                
            } else if (menu.currentMainMenu>=Menu::MaterialMenu) {
                placeObject(cursorPos.x, cursorPos.y, engine.map->mapNumber, (Object::ObjectType)currentObject);
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

    
    if (state==NORMAL_STATE || state==EQUIP_STATE) {
        engine.preRender();
    } else {
        engine.setFullVisibility();
        
    }
    
    if (state != PAINT_STATE) {
        engine.renderWorld();
    } else {
        paint.render();
    }
    
    vfx.update(deltaTime);
    
    dialog.render();
    menu.render();
    inspect.render();
    
    
    if (!mouseHide) {
        if (engine.vis[(int)mousePos.x+(int)mousePos.y*CONSOLE_WIDTH]>0 && !engine.map->isWindowOpaque((int)mousePos.x, (int)mousePos.y)) {
            console.setPixel(mousePos.x, mousePos.y, makeColor(5,5,5), 0, '\\');
        } else {
            console.setPixel(mousePos.x, mousePos.y, makeColor(1,1,1), 0, 'X');
        }
    }
    
    if (state==NORMAL_STATE || state==INSPECT_STATE) {
        cursorPos = engine.player->getPos();
    } else {
        ofVec2i pp = worldToWindow(cursorPos);
        console.setPixel(pp.x, pp.y, makeColor(5,5,0), makeColor(5,0,0), 'X');
    }
    
    
    if (debugGraph) engine.player->graph.render();
    
    if (state==NORMAL_STATE || state==EQUIP_STATE) {
        
        console.setPixel(3, 3, makeColor(4,0,0), 0, CHAR_HEART);
        char buf[255];
        sprintf(buf,"%d/%d", engine.player->data.hp, engine.player->data.maxhp);
        console.writeString(5,3,buf, makeColor(3,3,3));
        
        
        sprintf(buf,"Lvl %d", engine.player->level());
        console.writeString(15,3,buf, makeColor(3,5,5));
        sprintf(buf,"%d XP", engine.player->displayXP);
        console.writeString(15,5,buf, makeColor(3,3,3));
        
        
        
        console.setPixel(3, 5, makeColor(4,4,1), 0, 151);
        sprintf(buf,"%d GP", engine.player->displayGP);
        console.writeString(5,5,buf, makeColor(3,3,3));
        
        if (!playing)
            console.writeString(35,3,"PAUSED", makeColor(5,0,0));


        
        //writeString(5,6,"Ruins of Matador", makeColor(4,4,4), 0);
        
    } else if (state==EDIT_STATE) {
   
        console.writeString(5,3,ofToString((int)cursorPos.x), makeColor(5,5,3));
        console.writeString(10,3,ofToString((int)cursorPos.y), makeColor(5,5,3));
        
    }

    
    equip.render();

    
    eventLog.render();
    
    
    
}


//--------------------------------------------------------------
void Core::adjustWindow() {
    
    if (state==NORMAL_STATE || state==EQUIP_STATE) {
        
        int failsafe = 0;
        
        while ((engine.player->x - engine.map->window.getMinX()) < 35) {
            engine.map->setWindow( engine.map->window.x -1 , engine.map->window.y);
            
            failsafe++;
            if (failsafe>3) break;
        }
        
        while ((engine.map->window.getMaxX() - engine.player->x) < 35) {
            engine.map->setWindow( engine.map->window.x +1 , engine.map->window.y);
            failsafe++;
            if (failsafe>3) break;

        }
        
        while ((engine.player->y - engine.map->window.getMinY()) < 20) {
            engine.map->setWindow( engine.map->window.x , engine.map->window.y -1);
            failsafe++;
            if (failsafe>3) break;

        }
        
        while((engine.map->window.getMaxY() - engine.player->y) < 20) {
            engine.map->setWindow( engine.map->window.x , engine.map->window.y +1);
            failsafe++;
            if (failsafe>3) break;

        }
        
    } else {
        engine.map->setWindow( cursorPos.x - CONSOLE_WIDTH/2, cursorPos.y - CONSOLE_HEIGHT/2);
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
            engine.actionDebt = 0;
        }
        
        if (key=='e') {
            equip.toggle();
        } else if (key=='g') {
            debugGraph = !debugGraph;
        } else if (key=='p') {
            playing = !playing;
        }
        
        
        if (key=='f') {
            engine.reticleTime = 0.0f;
            if (engine.fireTargeting) {
                controls.fire = true;
            }
            
            engine.toggleFiring();
            
        } else if (key == OF_KEY_TAB) {
            ofLog() << " <tab>";

            if (engine.fireTargeting) {
                engine.firingIndex++;
                engine.firingIndex %= engine.firingList.size();
                engine.player->target = engine.firingList[engine.firingIndex];
            } else {
                ofLog() << "setting controls.tab true";
                controls.tab = true;
            }
        } else {
            if (engine.fireTargeting) {
                engine.toggleFiring();
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
            engine.torch = !engine.torch;
        } else if (key == 'r') {
            engine.reset();
            //controls.rest = true;
        } else if (key==' ') {
            controls.space = true;
        } else if (key==OF_KEY_F1) {
            menu.toggle();
        } else if (key==OF_KEY_F2) {
            inspect.toggle();
        } else if (key=='s') {
            engine.map->save();
        } else if (key=='l') {
            engine.map->load();
        } else if (key=='z') {
            serializer.save();
        } else if (key=='x') {
            serializer.load();
            
            engine.map->mapNumber = engine.player->z;
            engine.map->load();
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
    } else if (key==OF_KEY_TAB) {
        controls.tab = false;
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
    mouseHide = false;

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
    
    if (state==NORMAL_STATE) {
        
        ofVec2i pp = windowToWorld(mousePos);
        
        if (engine.map->traversable(pp.x, pp.y)) {
            engine.player->setDestination(pp);
        }
        
    } else if (state==EDIT_STATE) {
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
                
                if (o->z == engine.map->mapNumber) {
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


