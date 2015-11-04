//
//  Equip.cpp
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#include "Equip.h"

Equip equip;



void Equip::toggle() {
    
    if (core->state==NORMAL_STATE) {
        core->state=EQUIP_STATE;
        init();
    } else if (core->state==EQUIP_STATE) {
        core->state = NORMAL_STATE;
    }
}

void Equip::init() {
    
    items = engine.player->getInventory();
    
    /*
     engine.player->data.inventory; // all DWORD ids
     vector<Item*> items; // vector of pointers to all inventory items
     
     
     
     */

}


bool Equip::isOn() {
    return core->state==EQUIP_STATE;
}

void Equip::keyPressed(int key) {
    
    if (key==OF_KEY_ESC) {
        toggle();
    } else if (key=='e') {
        toggle();
    } else if (key==OF_KEY_TAB) {
        engine.player->cycleWeapons();
        
    } else if (key==OF_KEY_RIGHT) {
        
        if (uiLevel==0) {
            uiLevel=1;
        }
        
        //engine.player->cycleWeapons();
        
    } else if (key==OF_KEY_LEFT) {
        
        if (uiLevel==1) {
            uiLevel=0;
        }
    }
    
    if (uiLevel==0) {
        if (key==OF_KEY_UP) {
            slot--;
            if (slot<0) {
                slot = EQUIPMENT_SLOTS-1;
            }
        } else if (key==OF_KEY_DOWN) {
            slot++;
            slot %= EQUIPMENT_SLOTS;
        }
        
    }
    
}

BYTE Equip::getSlotLabelBG(int i) {
    
    if (i==slot) {
        return makeColor(0,0,5);
    }
    return makeColor(0,0,0);
    
}

BYTE Equip::getSlotLabelColor(int i) {
    
    if (i==slot) {
        return makeColor(3,3,3);
    }
    return makeColor(2,2,2);
    
}


BYTE Equip::slotFG(int i) {
    if (i==slot) {
        return makeColor(4,4,0);
    }
    return makeColor(3,3,0);
}

BYTE Equip::slotBG(int i) {
    if (i==slot) {
        return makeColor(0,0,5);
    }
    return makeColor(0,0,0);
}

void Equip::render() {
    
    
    BYTE c_onA;
    BYTE c_onB;
    BYTE c_offA;
    BYTE c_offB;

    if (engine.player->weaponSet==0) {
        c_onA = makeColor(4,4,4);
        c_onB = makeColor(5,5,0);
        c_offA = makeColor(2,2,2);
        c_offB = makeColor(3,3,0);
    } else {
        c_offA = makeColor(4,4,4);
        c_offB = makeColor(5,5,0);
        c_onA = makeColor(2,2,2);
        c_onB = makeColor(3,3,0);
    }

    int row = STARTING_EQUIPMENT_ROW;
    
    int col =  5;
    

    
    if (isOn()) {
        
        console.drawBox(ofRectangle(2,14,25,30), makeColor(1,1,1));
        
        console.writeString(5,16,"Equipment", makeColor(5,5,5));

        
        
        col = 15;
        
        
        int hr = 0;
        
        switch (slot) {
            case 0:
                hr = 0;
                break;
            case 1:
                hr = 1;
                break;
            case 2:
                hr = 3;
                break;
            case 3:
                hr = 5;
                break;
            case 4:
                hr = 6;
                break;
            case 5:
                hr = 7;
                break;
            case 6:
                hr = 9;
                break;
            case 7:
                hr = 10;
                break;
            case 8:
                hr = 11;
                break;
                
        }

        Pixel cursor;
        cursor.fg = c_onA;
        cursor.a = 0;
        cursor.c = 124;
        
        if (uiLevel==0) {
            //console.drawBox(ofRectangle(15,row+hr,11,1), makeColor(0,0,4));
            console.setPixel(3, row+hr, cursor);
        } else {
            //console.drawBox(ofRectangle(15,row+hr,11,1), makeColor(0,0,2));
            
            console.setPixel(29, row, cursor);
            
            // drawSubMenu

            drawSubMenu();
        }
        
        int lc = 5;

        console.writeString(lc, row, "Melee",        getSlotLabelColor(0));
        console.writeString(lc, row+1, "Offhand",    getSlotLabelColor(1));
        console.writeString(lc, row+3, "Ranged",     getSlotLabelColor(2));
        console.writeString(lc, row+5, "Armor",      getSlotLabelColor(3));
        console.writeString(lc, row+6, "Head",       getSlotLabelColor(4));
        console.writeString(lc, row+7, "Feet",       getSlotLabelColor(5));
        console.writeString(lc, row+9, "Neck",       getSlotLabelColor(6));
        console.writeString(lc, row+10, "L Ring",    getSlotLabelColor(7));
        console.writeString(lc, row+11, "R Ring",    getSlotLabelColor(8));
        
        
        
        Weapon * mle = engine.player->melee();
        if (mle!=NULL) {
            console.writeString(col,row,mle->getName(), slotFG(0), slotBG(0));
        }
        
        Weapon * oh = engine.player->offHand();
        if (oh!=NULL) {
            console.writeString(col,row+1,oh->getName(), slotFG(1), slotBG(1));
        }
        
        Weapon * rng = engine.player->ranged();
        if (rng!=NULL) {
            console.writeString(col,row+3,rng->getName(), slotFG(2), slotBG(2));
        }
        
        

        
    } else {

        console.setPixel(3, row, c_onA, makeColor(0,0,2), 153); // sword

        Weapon * mle = engine.player->melee();
        if (mle!=NULL) {
            console.writeString(col,row,mle->getName(), c_onB);
        }
        
        Weapon * oh = engine.player->offHand();
        if (oh!=NULL) {
            console.writeString(col,row+1,oh->getName(), c_onB);
        }
        
        Weapon * rng = engine.player->ranged();
        if (rng!=NULL) {
            console.writeString(col,row+3,rng->getName(), c_offB);
        }

    }
    
    



    
    
    
    
}



void Equip::drawSubMenu() {
    
    switch (slot) {
        case SLOT_MELEE:
        {
            
            int row =STARTING_EQUIPMENT_ROW;

            console.drawBox(ofRectangle(28,14,25,30), makeColor(0,0,2));
            
            console.writeString(31,16,"Melee Weapons", makeColor(5,5,5));


            for (auto it = items.begin(); it != items.end(); ++it) {
                
                int i =  it - items.begin();
                
                if (i==slot) {
                    console.writeString(31,row++,(*it)->getName(), makeColor(5,5,0), makeColor(0,0,5));
                } else {
                    console.writeString(31,row++,(*it)->getName(), makeColor(4,4,0), makeColor(0,0,0));
                    
                }
                

            }

            // get vector of all items in inventory
            break;
        }
    }
    
}