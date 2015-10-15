//
//  ObjectFactory.cpp
//
//  Created by vesabios on 10/11/15.
//  Copyright © 2015 vesabios. All rights reserved.
//

#include "ObjectFactory.h"
#include "Actor.h"



class Door : public Item {
public:
    
    struct data_t {
        bool open = false;
        int keyId = 0;
    };
    
    void initReflectors() override  {
        REFLECT(open)
        REFLECT(keyId)
    }
    
    string getName() override { return "Door"; }
    
    DEBT interactable() override { return 1; }
    
    InteractionType getInteractionTypeForInteractor(Object * o) {
        return Use;
    }
    
    DEBT traversable() override { return data.open ? TRAVERSE_NORMAL : TRAVERSE_BLOCKED; }
    bool isPortable() override { return false; }
    void update(DEBT d) override {}
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,5);
        p.bg = 0;
        p.c = toascii('X');
        return p;
    }
    
    OBJTYPE(Door);
    MSGPACK_DEFINE(type, bundle, x, y, z);
    
};
REGISTER_OBJTYPE(Door);


class Coin : public Item {
public:
    
    struct data_t {
        int amount = 0;
    };
    
    void initReflectors() override {
        REFLECT(amount)
    }
    
    InteractionType getInteractionTypeForInteractor(Object * o) {
        return Take;
    }
    
    
    string getName() override  { return "Coin"; }
    DEBT interactable() override { return 1; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    bool isPortable() override { return true; }
    void update(DEBT d) override {}
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii('0');
        return p;
    }
    
    OBJTYPE(Coin);
    MSGPACK_DEFINE(type, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Coin);






