//
//  Magician.cpp
//  matador
//
//  Created by vesabios on 10/22/15.
//
//

#include "Magician.hpp"
#include "Core.h"
#include "ObjectFactory.h"

void Magician::init() {
    
    data.strength = 7;
    data.dexterity = 15;
    data.constitution = 9;
    data.intelligence = 8;
    data.wisdom = 7;
    data.charisma = 8;
    
    data.attackSpeed = 100;
    data.movementSpeed = 130;
    
    data.tohit = 4;
    
    // hit dice 1d8
    data.maxhp = data.hp = d8();
    
    // the tomb of an old wizard has been discovered
    

    /*
     
     The forgotten tomb,
     Recalls the last memory
     of a magician.
     
     
     */
    

    
    
    
};


string Magician::getName() {
    return "Magician";
}



InteractionType Magician::getInteractionType(Object *) {
    return Read;
}


DEBT Magician::traversable()  {
    return TRAVERSE_BLOCKED;
}

bool Magician::isPortable()  {
    return false;
}


void Magician::readLines() {
    
    
    lines[0] = "The forgotten tomb";
    lines[1] = "Recalls the last night";
    lines[2] = "of a magician";
    
    vector<string> l;
    
    for (int i=0; i<30; i++) {
        if (lines[i].length()>0) l.push_back(lines[i]);
    }
    
    dialog.center = core->worldToWindow(ofVec2i(x, y));
    dialog.lines = l;
    dialog.active = true;
    
    
}


float Magician::update(DEBT d)  {
    
    if (actionDebt>0) actionDebt -= d;
 
    return 0.0f;
    
}

Pixel Magician::render(float luma)  {
    Pixel p;
    p.fg = makeColor(1,4,5);
    p.bg = 0;
    p.a = 0;
    p.c = toascii('M');
    return p;
}


REGISTER_OBJTYPE(Magician);