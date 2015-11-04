//
//  Item.h
//  matador
//
//  Created by vesabios on 10/17/15.
//
//

#ifndef Item_h
#define Item_h

class Actor;

class Item : public Object {
public:
    
    virtual DEBT use(Actor * a) = 0;
    
};

class Coin : public Item {
public:
    
    struct data_t {
        int amount = 0;
    };
    
    data_t data;
    
    
    void initReflectors() override {
        REFLECT(amount)
    }
    
    InteractionType getInteractionType(Object *) override {
        return Take;
    }
    
    DEBT use(Actor * a) override {
        return 0;
    }
    
    string getName() override  { return "Coin"; }
    DEBT traversable() override { return TRAVERSE_NORMAL; }
    bool isPortable() override { return true; }
    float update(DEBT d) override { return 0.0f; }
    Pixel render(float luma) override {
        Pixel p;
        p.fg = makeColor(5,5,0);
        p.bg = 0;
        p.c = 151;
        return p;
    }
    
    void init() override {};
    
    OBJTYPE(Coin);
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
};

#endif /* Item_h */
