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
    
    // Magicians need weapons too!
    
    /*
    Weapon * weapon = static_cast<Weapon*>(Object::create(Object::Club));
    weapon->init();
    weapon->z = VOID_LOCATION; // it's not on a map, it only exists abstractly as the Magician has no dedicated inventory
    data.rightHandGuid = weapon->guid;
    
    ofLog() << "Magician initted with club id: " << data.rightHandGuid;
    
     */
    // hit dice 1d8
    data.maxhp = data.hp = (int)ofRandom(8)+1;
    
    
    
    lines[0] = "What shall we achieve?";
    lines[1] = "Willpower and desire";
    lines[2] = "yield revelation";
    
    
    //Creation by willpower and desire. The mental image of what you want to achieve. Revelation.
    
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
    
    
    lines[0] = "What shall we achieve?";
    lines[1] = "Willpower and desire";
    lines[2] = "yield revelation";

    
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