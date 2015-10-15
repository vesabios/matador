//
//  Inspect.cpp
//  matador
//
//  Created by vesabios on 10/8/15.
//
//

#include "ofMain.h"
#include "Inspect.h"
#include "Map.h"
#include "Core.h"
#include "Console.h"
#include "Defines.h"

Inspect inspect;


void Inspect::toggle() {
    if (visible) {
        hide();
    } else {
        show();
    }
}

void Inspect::show() {
    visible = true;
    core->state = InspectState;
}


void Inspect::hide() {
    visible = false;
    core->state = InspectState;
}

void Inspect::update(const float delaTime) {
    
    if (visible) {
        menuPos += (50.0f - menuPos) * 0.2f;
    } else {
        menuPos += (85.0f - menuPos) * 0.2f;
    }
    

    
}


void Inspect::keyPressed(int key) {
    

    
}

void Inspect::keyReleased(int key) {

}




void Inspect::mouseDragged(int inx, int iny, int button) {
    

    
}

void Inspect::mousePressed(int inx, int iny, int button) {
    
    for(auto it = btns.begin(); it != btns.end(); it++)
    {
        ofRectangle r = it->r;
        r.translate(menuPos, 0);
        if (r.inside(inx, iny)) {
            it->funptr(it->valuePtr);
        };
    }
    
}

void Inspect::toggleBoolean(void * ptr) {
    *(bool*)ptr = !*(bool*)ptr;
}

void Inspect::incInteger(void * ptr) {
    
    (*(int*)ptr)++;
    printf("int is: %d\n", *(int*)ptr);

    
    
}

void Inspect::decInteger(void * ptr) {
}


void Inspect::clearMenu() {
    btns.clear();
}

void Inspect::createMenu() {
    
    int x = 1;
    int y = 5;
    
    unsigned char bg = makeColor(0,1,3);
    unsigned char fg = makeColor(5,5,5);
    
    printf("creating menu!\n");
    
    if (currentObject!=NULL) {
        vector<Reflector> v = currentObject->reflectors;
        
        for (int i=0; i<v.size(); i++) {
            
            printf("%s\n", v[i].name.c_str());
            printf("%s\n", v[i].t->name());
            
            
            if (*v[i].t == typeid(int)) {
                printf("int identified\n");
                printf("%d\n", *(int*)v[i].ptr);

                
                Clickable c;
                c.fg = fg;
                c.bg = bg;
                c.text = CHAR_LEFT;
                c.r = ofRectangle(x,y,1,1);
                c.funptr = &decInteger;
                c.t = v[i].t;
                c.valuePtr = v[i].ptr;
                btns.push_back(c);
                
                c.text = CHAR_RIGHT;
                c.r = ofRectangle(x+1,y,1,1);
                c.funptr = &incInteger;
                btns.push_back(c);
                
                c.text = v[i].name;
                c.r = ofRectangle(x+3,y,1,1);
                c.funptr = NULL;
                btns.push_back(c);
                
            } else if (*v[i].t == typeid(bool)) {
                
                printf("bool identified\n");
                printf("%d\n", *(bool*)v[i].ptr);
                Clickable c;
                c.fg = fg;
                c.bg = bg;
                c.text = CHAR_EMPTYBOX;
                c.r = ofRectangle(x,y,1,1);
                c.funptr = &toggleBoolean;
                c.t = v[i].t;
                c.valuePtr = v[i].ptr;
                btns.push_back(c);
                
                c.text = v[i].name;
                c.r = ofRectangle(x+3,y,30,1);
                c.funptr = NULL;
                btns.push_back(c);
                
            }
            
            y++;
            
        }
        
    }
    
    
    
}

void Inspect::inspectObject(Object * o) {
    
    if (o==NULL) {
        currentObject = NULL;
        clearMenu();
    } else {
        if (currentObject!=o) {
            clearMenu();
            currentObject = o;
            createMenu();

        }
    }
}


void Inspect::render() {
    
    if (menuPos<80) {
        ofRectangle r = ofRectangle((int)menuPos,2,28,40);
        
        unsigned char bg = makeColor(0,1,3);
        unsigned char fg = makeColor(5,5,5);
        
        console.drawBox(r, bg);
        console.writeString((int)menuPos+1, 3, "Inspect", makeColor(5,0,5), bg);
        
        for(auto it = btns.begin(); it != btns.end(); it++)
        {
            if (it->funptr==NULL) {
                
                string valueString;
                
                if (*(it->t) == typeid(int)) {
                    int * i = (int*)it->valuePtr;
                    valueString = ofToString(*i);
                } else  if (*(it->t) == typeid(bool)) {
                    bool * i = (bool*)it->valuePtr;
                    valueString = *i ? "true" : "false";
                }
                
                console.writeString((int)menuPos+it->r.x, it->r.y, it->text+": "+valueString, it->fg, it->bg);
                
            } else {
                console.writeString((int)menuPos+it->r.x, it->r.y, it->text, it->fg, it->bg);
            }
        }
        
    }

    
}