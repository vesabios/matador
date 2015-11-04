//
//  Equip.h
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#ifndef Equip_h
#define Equip_h

#include <stdio.h>
#include "Core.h"
#include "Item.h"

#define STARTING_EQUIPMENT_ROW 20

class Equip {
public:
    
    enum SlotType {
        SLOT_MELEE,
        SLOT_OFFHAND,
        SLOT_RANGED,
        SLOT_ARMOR,
        SLOT_HEAD,
        SLOT_FEET,
        SLOT_NECK,
        SLOT_LRING,
        SLOT_RRING
    };
    
    
    void keyPressed(int key);
    bool isOn();
    void init();
    void toggle();
    
    BYTE slotFG(int i);
    BYTE slotBG(int i);

    
    BYTE getSlotLabelColor(int i);
    BYTE getSlotLabelBG(int i);
    void render();
    void drawSubMenu();
    
    
    int uiLevel = 0;
    int slot = 0;
    vector<Item*> items;
  
    
    
};


extern Equip equip;

#endif /* Equip_h */
