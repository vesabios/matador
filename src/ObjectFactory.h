//
//  ObjectFactory.h
//
//  Created by vesabios on 10/11/15.
//  Copyright © 2015 vesabios. All rights reserved.
//

#ifndef ObjectFactory_h
#define ObjectFactory_h


#define OBJTYPE(ot) \
ot * pack() override { \
    bundle.size = sizeof(data_t); \
    bundle.ptr = (const char*) &data; \
    return this; \
} \
\
data_t data; \
unsigned char type = Object::ot; \
void unpack() override { \
    memcpy(&data, bundle.ptr, bundle.size); \
} \


#define REGISTER_OBJTYPE(klass) \
class klass##Factory : public ObjectFactory { \
public: \
    klass##Factory() \
    { \
        Object::registerType(Object::klass, this); \
        name = #klass; \
    } \
    virtual Object *create() { \
        Object * i = new class klass(); \
        i->initReflectors(); \
        i->type = Object::klass; \
        return i; \
    } \
}; \
static klass##Factory global_##klass##Factory;


#define REFLECT(key) \
reflectors.push_back(Reflector(#key, const_cast<type_info*>(&typeid(data.key)), &data.key));



#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <msgpack.hpp>
#include "Defines.h"

using namespace std;
class Object;

class ObjectFactory
{
public:
    std::string name;
    virtual Object *create() = 0;
};


struct Reflector {
public:
    string name;
    type_info * t;
    void * ptr;
    Reflector(string n, type_info * ti, void * p) : name(n), t(ti), ptr(p) {}
};
 


class Msg {
public:
    DWORD type;
    MSGPACK_DEFINE(type);
};

#define ITEM_TYPE_START_INDEX 1<<8


class Object {
 
public:
    
    enum ObjectType {
        Nothing = 1<<8,
        Coin,
        Door,
        Sword,
        Kobold = 1<<16,
        Player
        
    };
    
    DWORD                       type;
    WORD                        x;
    WORD                        y;
    WORD                        z;
    
    struct data_t {
        char a;
    };
    
    data_t data;
    
    virtual string getName() = 0;
    virtual DEBT interactable() = 0;

    virtual DEBT traversable() = 0;
    virtual bool isPortable() = 0;
    virtual Pixel render(float luma) = 0;
    virtual void update(DEBT d) = 0;
    
    virtual InteractionType getInteractionTypeForInteractor(Object * o) = 0;
    
    vector<Reflector> reflectors;
    virtual void initReflectors() = 0;
    virtual Object *pack() = 0;
    virtual void unpack() = 0;
    
   
    ofVec2i getPos() { return ofVec2i(x,y); }
    
    static void registerType(const ObjectType& type, ObjectFactory *factory) {
        factories()[type] = factory;
    }
    
    static Object * create(const ObjectType &type)
    {
        return factories()[type]->create();
    }
    
    static vector<ObjectType> getKeys() {
        vector<ObjectType> v;
        for(auto it = factories().begin(); it != factories().end(); ++it) {
            v.push_back(it->first);
            cout << it->first << "\n";
            cout << it->second->name << "\n";

        }
        return v;
    }
    
    static string getNameFromType(ObjectType it) {
        return factories()[it]->name;
    }
    
    static bool doesObjectTypeExist(ObjectType it) {
        if ( factories().find(it) == factories().end() ) {
            return false;
        }
        return true;
    }
    
    msgpack::type::raw_ref      bundle;
    msgpack::sbuffer *          sbuf;
    
    MSGPACK_DEFINE(type, bundle, x, y, z);
    
private:

    static map<ObjectType, ObjectFactory*> &factories() {
        static map<Object::ObjectType, ObjectFactory*> f;
        return f;
    }
    
};


class Item : public Object {
    
};




#endif /* ObjectFactory_h */





