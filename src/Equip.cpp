//
//  Equip.cpp
//  matador
//
//  Created by vesabios on 10/18/15.
//
//

#include "Equip.h"

Equip equip;


void Equip::keyPressed(int key) {
    
    if (key==OF_KEY_ESC) {
        core->state=NORMAL_STATE;
    } else if (key=='e') {
        core->state=NORMAL_STATE;
    } else if (key==OF_KEY_TAB) {
        core->player->cycleWeapons();
    } else if (key==OF_KEY_RIGHT) {
        core->player->cycleWeapons();
    }
    
}

void Equip::render() {
    
    console.writeString(5,6,"RH: ", makeColor(4,4,4), 0);
    console.writeString(9,6,core->player->melee()->getName(), makeColor(5,5,0), 0);
    
}