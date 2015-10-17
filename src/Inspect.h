//
//  Inspect.h
//  matador
//
//  Created by vesabios on 10/8/15.
//
//

#ifndef Inspect_h
#define Inspect_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"
#include "ObjectFactory.h"






class Inspect {
    
   
    bool visible = false;
    float menuPos = 100.0f;
    
    Object * currentObject;
    
    class Clickable {
    public:
        ofRectangle r;
        void (*funptr)(void*ptr);
        string text;
        BYTE fg;
        BYTE bg;
        DWORD arg = 0;
        type_info * t;
        void * valuePtr;
        Clickable(){};
        void operator()() {};
    };
    
public:
    
    
    vector<Clickable> btns;

    
    void show();
    void hide();
    
    void toggle();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    static void toggleBoolean(void* b_ptr);
    static void incInteger(void* i_ptr);
    static void decInteger(void* i_ptr);
    static void incUnsignedChar(void* i_ptr);
    static void decUnsignedChar(void* i_ptr);
    
    void clearMenu();
    void createMenu();
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    void inspectObject(Object * o);
    
    void update(const float deltaTime);
    void render();
    
};



extern Inspect inspect;


#endif /* Inspect_h */
