//
//  Menu.cpp
//  matador
//
//  Created by vesabios on 10/8/15.
//
//

#include "ofMain.h"
#include "Menu.h"
#include "Map.h"
#include "Core.h"
#include "Console.h"
#include "Defines.h"

Menu menu;


void Menu::toggle() {
    if (menuVisible) {
        hideMenu();
    } else {
        showMenu();
    }
}

void Menu::showMenu() {
    menuVisible = true;
    core->state = EditState;
    
    createMenu();
}


void Menu::hideMenu() {
    menuVisible = false;
    core->state = NormalState;
    
    categoryBtns.clear();
    materialBtns.clear();
    actorBtns.clear();
    itemBtns.clear();
    mapSelectBtns.clear();
}

void Menu::update(const float delaTime) {
    
    if (menuVisible) {
        menuPos += (50.0f - menuPos) * 0.2f;
    } else {
        menuPos += (85.0f - menuPos) * 0.2f;
    }
    
}


void Menu::keyPressed(int key) {
    
    if (key=='=') {
        brushSize+=2;
        brushSize = MIN(brushSize,51);
    } else if (key=='-') {
        brushSize-=2;
        brushSize = MAX(brushSize,1);
    }
    
    if (key=='1') {
        brush = 1;
    } else if (key=='2') {
        brush = 2;
    } else if (key=='3') {
        brush = 3;
    } else if (key=='4') {
        brush = 4;
    } else if (key=='5') {
        brush = 5;
    } else if (key=='6') {
        brush = 6;
    } else if (key=='7') {
        brush = 7;
    } else if (key=='8') {
        brush = 8;
    } else if (key=='i') {
        
        // TODO: get material under cursor
        
    } else if (key=='o') {
        ortho = true;
        
    }
    
}

void Menu::keyReleased(int key) {
    if (key=='o') {
        ortho = false;
        orthoAxis = AXIS_NONE;
    }
}




void Menu::mouseDragged(int inx, int iny, int button) {
    
    if (inx < (int)menuPos) {
        
        ofVec2i pp;
        
        if (ortho) {
            printf("ortho\n");
            
            if (orthoAxis==AXIS_NONE) {
                
                if ((int)startMousePoint.x!=inx) {
                    orthoAxis = AXIS_HORIZONTAL;
                } else if ((int)startMousePoint.y!=iny) {
                    orthoAxis = AXIS_VERTICAL;
                }
                
                pp = core->windowToWorld(ofVec2i(inx, iny));

            } else if (orthoAxis == AXIS_HORIZONTAL) {
                pp = core->windowToWorld(ofVec2i(inx, startMousePoint.y));
            } else if (orthoAxis == AXIS_VERTICAL) {
                pp = core->windowToWorld(ofVec2i(startMousePoint.x, iny));
            }
            
        } else {
            pp = core->windowToWorld(ofVec2i(inx, iny));
            
        }
        
        brushMaterial(pp, (MaterialType)core->map->currentMaterial);

    }
    
}


void Menu::mousePressed(int x, int y, int button) {
    
    if (x>=50) {

        for(auto it = categoryBtns.begin(); it != categoryBtns.end(); it++)
        {
            ofRectangle r = it->r;
            r.translate(menuPos, 0);
            if (r.inside(x, y)) {
                it->funptr(it->arg);
            };
        }
        
        if (currentMainMenu==MaterialMenu) {
            for(auto it = materialBtns.begin(); it != materialBtns.end(); it++)
            {
                ofRectangle r = it->r;
                r.translate(menuPos, 0);
                if (r.inside(x, y)) {
                    it->funptr(it->arg);
                };
            }
            
        } else if (currentMainMenu==ItemMenu) {
            for(auto it = itemBtns.begin(); it != itemBtns.end(); it++)
            {
                ofRectangle r = it->r;
                r.translate(menuPos, 0);
                if (r.inside(x, y)) {
                    it->funptr(it->arg);
                };
            }
            
        } else if (currentMainMenu==ActorMenu) {
            for(auto it = actorBtns.begin(); it != actorBtns.end(); it++)
            {
                ofRectangle r = it->r;
                r.translate(menuPos, 0);
                if (r.inside(x, y)) {
                    it->funptr(it->arg);
                };
            }
        }
        
        for(auto it = mapSelectBtns.begin(); it != mapSelectBtns.end(); it++)
        {
            ofRectangle r = it->r;
            r.translate(menuPos, 0);
            
            if (r.inside(x, y)) {
                it->funptr(it->arg);
            };
        }
        
    } else {
        
        if (currentMainMenu==MaterialMenu) {
            ofVec2i pp = core->windowToWorld(ofVec2f(x,y));
            
            brushMaterial(pp, (MaterialType)core->map->currentMaterial);
        }
        
    }
    
    startMousePoint = ofPoint(x, y);

}


void Menu::setMainMenu(int v) {
    menu._setMainMenu(v);
}

void Menu::_setMainMenu(int v) {
    
    currentMainMenu = (MAIN_MENU)v;
    
    for(auto it = categoryBtns.begin(); it != categoryBtns.end(); it++)
    {
        auto i = std::distance(categoryBtns.begin(), it);
        if (i==v) {
            categoryBtns[i].fg = makeColor(0,5,5);
        } else {
            categoryBtns[i].fg = makeColor(5,5,5);
        }
    }
}


void Menu::setMaterial(int v) {
    menu._setMaterial(v);
}

void Menu::_setMaterial(int v) {
    
    core->map->currentMaterial = (MaterialType)v;
    
    for(auto it = materialBtns.begin(); it != materialBtns.end(); it++)
    {
        auto i = std::distance(materialBtns.begin(), it);
        if (i==v) {
            materialBtns[i].fg = makeColor(0,5,5);
        } else {
            materialBtns[i].fg = makeColor(5,5,5);
        }
    }
}


void Menu::setItem(int v) {
    menu._setItem(v);
}

void Menu::_setItem(int v) {
    core->currentObject = (Object::ObjectType)v;
    
    for(auto it = itemBtns.begin(); it != itemBtns.end(); it++)
    {
        if (it->arg==v) {
            it->fg = makeColor(0,5,5);
        } else {
            it->fg = makeColor(5,5,5);
        }
    }
    
    for(auto it = actorBtns.begin(); it != actorBtns.end(); it++)
    {
        if (it->arg==v) {
            it->fg = makeColor(0,5,5);
        } else {
            it->fg = makeColor(5,5,5);
        }
    }
    
    
}



void Menu::cycleMap(int v) {
    menu._cycleMap(v);
}

void Menu::_cycleMap(int v) {
    
    if (v==0) {
        core->map->cycle(-1);
    } else {
        core->map->cycle(1);
    }
    
}




void Menu::createMenu() {
    
    int x = 1;
    int y = 5;
    
    unsigned char bg = makeColor(0,1,3);
    unsigned char fg = makeColor(5,5,5);
    
    Clickable c;
    c.fg = fg;
    c.bg = bg;
    c.text = "Materials";
    c.r = ofRectangle(x,y++,30,1);
    c.funptr = &setMainMenu;
    c.arg = 0;
    categoryBtns.push_back(c);

    c.arg = 1;
    c.text = "Items";
    c.r = ofRectangle(x,y++,30,1);
    categoryBtns.push_back(c);

    c.arg = 2;
    c.text = "Actors";
    c.r = ofRectangle(x,y++,30,1);
    categoryBtns.push_back(c);
    

    
    y = 10;
    for (int i=0; i<255; i++) {
        if (materials.mats[i]!=NULL) {
            c.text = materials.mats[i]->getName();
            c.funptr = &setMaterial;
            c.arg = i;
            c.r = ofRectangle(x,y++,30,1);
            materialBtns.push_back(c);
        }
    }
    
    
    y = 10;
    vector<Object::ObjectType> v = Object::getKeys();
    
    for (int j=0; j<v.size(); j++) {
        if (dynamic_cast<Item *>(ofPtr<Object>(Object::create(v[j])).get())) {
            c.text = Object::getNameFromType(v[j]);
            c.funptr = &setItem;
            c.arg = v[j];
            c.r = ofRectangle(x,y++,30,1);
            itemBtns.push_back(c);
        }
    }

    y = 10;
    for (int j=0; j<v.size(); j++) {
        if (dynamic_cast<Actor *>(ofPtr<Object>(Object::create(v[j])).get())) {
            c.text = Object::getNameFromType(v[j]);
            c.funptr = &setItem;
            c.arg = v[j];
            c.r = ofRectangle(x,y++,30,1);
            actorBtns.push_back(c);
        }
    }
    
    
    c.fg = makeColor(5,5,0);
    c.bg = makeColor(0,3,0);
    c.text =string(1,CHAR_LEFT);
    c.funptr = &cycleMap;
    c.arg = 0;
    c.r = ofRectangle(11,3,1,1);
    
    mapSelectBtns.push_back(c);

    c.fg = makeColor(5,5,0);
    c.bg = makeColor(0,3,0);
    c.text =string(1,CHAR_RIGHT);
    c.funptr = &cycleMap;
    c.arg = 1;
    c.r = ofRectangle(18,3,1,1);
    
    mapSelectBtns.push_back(c);
    

}



void Menu::brushMaterial(ofVec2i p, MaterialType mt) {
    
    int offset = (brushSize -1) /2;
    for (int y=0; y<brushSize; y++) {
        for (int x=0; x<brushSize; x++) {
            core->map->placeMaterial((p.x+x) - offset , (p.y+y) - offset, mt);
            
        }
    }
    
}

void Menu::render() {
    
    if (menuPos<80) {
        ofRectangle r = ofRectangle((int)menuPos,2,28,40);
        
        unsigned char bg = makeColor(0,1,3);
        unsigned char fg = makeColor(5,5,5);
        
        console.drawBox(r, bg);
        
        
        console.writeString((int)menuPos+1, 3, "Map Edit", makeColor(5,0,5), bg);

        // map select buttons
        console.writeString((int)menuPos+13, 3, ofToString(core->map->mapNumber), makeColor(5,5,5), makeColor(0,1,4));
        for(auto it = mapSelectBtns.begin(); it != mapSelectBtns.end(); it++)
        {
            console.writeString((int)menuPos+it->r.x, it->r.y, it->text, it->fg, it->bg);
        }
        
        // category select buttons
        
        for(auto it = categoryBtns.begin(); it != categoryBtns.end(); it++)
        {
            console.writeString((int)menuPos+it->r.x, it->r.y, it->text, it->fg, it->bg);
        }
        
        
        //category list buttons
        
        if (currentMainMenu==MaterialMenu) {
            for(auto it = materialBtns.begin(); it != materialBtns.end(); it++)
            {
                console.writeString((int)menuPos+it->r.x, it->r.y, it->text, it->fg, it->bg);
            }
        } else if (currentMainMenu==ItemMenu) {
            for(auto it = itemBtns.begin(); it != itemBtns.end(); it++)
            {
                console.writeString((int)menuPos+it->r.x, it->r.y, it->text, it->fg, it->bg);
            }
        } else if (currentMainMenu==ActorMenu) {
            for(auto it = actorBtns.begin(); it != actorBtns.end(); it++)
            {
                console.writeString((int)menuPos+it->r.x, it->r.y, it->text, it->fg, it->bg);
            }
            
        }
        
    }
    
}