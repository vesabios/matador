//
//  Item.cpp
//  matador
//
//  Created by vesabios on 10/17/15.
//
//

#include <stdio.h>
#include "ObjectFactory.h"
#include "Item.h"
#include "Actor.h"
#include "Engine.h"

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
    
    DEBT use(Actor * a) override {
        return 0;
    }
    
    string getName() override {
        return "Door";
    }
    
    InteractionType getInteractionType(Object *) override {
        return Use;
    }
    
    DEBT traversable() override { return data.open ? TRAVERSE_NORMAL : TRAVERSE_BLOCKED; }
    bool isPortable() override { return false; }
    float update(DEBT d) override { return 0.0f; }
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



class Transit : public Item {
public:
    
    struct data_t {
        int from = 0;
        int to = 0;
    };
    
    data_t data;
    
    void initReflectors() override  {
        REFLECT(from)
        REFLECT(to)
    }
    
    DEBT use(Actor * a) override {
        ofLog() << "trying to use transit...";
        
        for (int i=0; i< Object::elements().size(); i++) {
            Object * o = Object::elements()[i];
            
            Transit * t = dynamic_cast<Transit*>(o);
            if (t) {
                if (t->data.to == data.from) {
                    // we found our transit!!
                    
                    ofLog()<< "bingo!";
                    
                    engine.transitActor(a, o->x, o->y, o->z);

                    
                    //return 1;
                    
                }
            }
        }
        
        return 0;
        
    }
    
    string getName() override { return "Transit"; }
    
    
    InteractionType getInteractionType(Object *) override {
        return Use;
    }
    
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    bool isPortable() override { return false; }
    float update(DEBT d) override { return 0.0f; }
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(4,4,4);
        p.bg = makeColor(2,2,2);
        p.c = toascii('>');
        return p;
    }
    
    void init() override {};
    
    OBJTYPE(Transit);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};
REGISTER_OBJTYPE(Transit);






REGISTER_OBJTYPE(Coin);