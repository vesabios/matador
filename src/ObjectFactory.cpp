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
    
    data_t data;
    
    void initReflectors() override  {
        REFLECT(open)
        REFLECT(keyId)
    }
    
    string getName() override { return "Door"; }
    
    DEBT interactable() override { return 1; }
    
    InteractionType getInteractionTypeForInteractor(Object *) override {
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
    
    void init() override {};
    
    OBJTYPE(Door);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};
REGISTER_OBJTYPE(Door);


class Coin : public Item {
public:
    
    struct data_t {
        int amount = 0;
    };
    
    data_t data;
    
    
    void initReflectors() override {
        REFLECT(amount)
    }
    
    InteractionType getInteractionTypeForInteractor(Object *) override {
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
    
    void init() override {};
    
    OBJTYPE(Coin);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Coin);




class Club : public Weapon {
public:
    
    Club() {
        data.die = 4;
        data.numberOfDice = 1;
    }

    
    InteractionType getInteractionTypeForInteractor(Object *) override {
        return Take;
    }
    
    
    string getName() override  { return "Club"; }
    DEBT interactable() override { return 1; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    bool isPortable() override { return true; }
    void update(DEBT d) override {}
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = toascii('/');
        return p;
    }
    
    void init() override {};
    
    OBJTYPE(Club);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

REGISTER_OBJTYPE(Club);

