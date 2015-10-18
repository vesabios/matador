//
//  Menu.h
//  matador
//
//  Created by vesabios on 10/8/15.
//
//

#ifndef Menu_h
#define Menu_h

#include <stdio.h>
#include "ofMain.h"
#include "Defines.h"
#include "Materials.h"



class Menu {
    
    bool menuVisible = false;
    float menuPos = 100.0f;
    
    
public:
    
    
    class Clickable {
    public:
        ofRectangle r;
        void (*funptr)(int v);
        string text;
        BYTE fg;
        BYTE bg;
        DWORD arg = 0;
        void * valuePtr;
        Clickable(){};
        void operator()() {};
    };
    
    enum MAIN_MENU {
        MaterialMenu,
        ItemMenu,
        ActorMenu
    };
    
    
    typedef void (Menu::*callback)(int v);
    
    void showMenu();
    void hideMenu();
    
    void createMenu();
    
    MaterialType currentMaterial = (MaterialType)0;

    MAIN_MENU currentMainMenu = MaterialMenu;
    Axis orthoAxis = AXIS_NONE;

    
    vector<Clickable> categoryBtns;
    vector<Clickable> materialBtns;
    vector<Clickable> itemBtns;
    vector<Clickable> actorBtns;
    
    vector<Clickable> mapSelectBtns;

    
    void toggle();
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    static void setMainMenu(int v);
    void _setMainMenu(int v);
    
    static void setMaterial(int v);
    void _setMaterial(int v);
    
    static void setItem(int v);
    void _setItem(int v);
    
    static void cycleMap(int v);
    void _cycleMap(int v);

    void brushMaterial(ofVec2i p, MaterialType mt);

    
    void update(const float deltaTime);
    void render();
    
    
    bool mouseDown = false;
    bool ortho = false;
    ofPoint startMousePoint;
    char brush = 0;
    BYTE brushSize = 1;
    
};






#endif /* Menu_h */
