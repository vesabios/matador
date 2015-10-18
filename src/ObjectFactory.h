//
//  ObjectFactory.h
//
//  Created by vesabios on 10/11/15.
//  Copyright © 2015 vesabios. All rights reserved.
//

#ifndef ObjectFactory_h
#define ObjectFactory_h


#define OBJTYPE(T) \
T * pack() override { \
    bundle.size = sizeof(data_t); \
    bundle.ptr = (const char*) &data; \
    return this; \
} \
\
unsigned char type = Object::T; \
void unpack() override { \
    memcpy(&data, bundle.ptr, bundle.size); \
} \


#define REGISTER_OBJTYPE(T) \
class T##Factory : public ObjectFactory { \
public: \
    T##Factory() \
    { \
        Object::registerType(Object::T, this); \
        name = #T; \
    } \
    virtual Object * create() { \
        Object * o(new class T()); \
        o->initReflectors(); \
        o->guid = genereateGuid(); \
        o->type = Object::T; \
        return o; \
    } \
}; \
static T##Factory global_##T##Factory;


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
    virtual Object * create() = 0;
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
        MapData = 1,
        Nothing = 256,
        Coin,
        Door,
        Transit,
        Club = 10000,
        Dagger,
        Greatclub,
        Handaxe,
        Javelin,
        LightHammer,
        Mace,
        Quarterstaff,
        Sickle,
        Spear,
        Fists,
        LightCrossbow,
        Dart,
        Shortbow,
        Sling,
        Battleaxe,
        Flail,
        Glaive,
        Greataxe,
        Greatsword,
        Halberd,
        Lance,
        Longsword,
        Maul,
        Morningstar,
        Pike,
        Rapier,
        Scimitar,
        Shortsword,
        Trident,
        WarPick,
        Whip,
        Blowgun,
        HandCrossbow,
        HeavyCrossbow,
        Longbow,
        Sword,
        Kobold = 65536,
        Player
        
    };
    
    DWORD                       type;
    DWORD                       guid;
    WORD                        x;
    WORD                        y;
    WORD                        z;
    
    struct data_t {
        char a;
    };
    
    data_t data;
   
    
    
    
    static std::vector<Object*> &elements(){
        static std::vector<Object*> v;
        return v;
    }
    
    Object() {
        ofLog() << "object ctor: " ;
        elements().push_back(this);
    }
    
    virtual ~Object() {
        ofLog() << "object dtor" ;
        elements().erase(std::remove(elements().begin(), elements().end(), this), elements().end());
    }
    
    
    virtual string getName() = 0;
    virtual DEBT interactable() = 0;
    virtual DEBT traversable() = 0;
    virtual bool isPortable() = 0;
    virtual Pixel render(float luma) = 0;
    virtual void update(DEBT d) = 0;
    virtual InteractionType getInteractionTypeForInteractor(Object *) = 0;
    virtual void init() = 0;

    vector<Reflector> reflectors;
    virtual void initReflectors() = 0;
    virtual Object * pack() = 0;
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
    
    MSGPACK_DEFINE(type, guid, bundle, x, y, z);
    
    static map<ObjectType, ObjectFactory*> &factories() {
        static map<Object::ObjectType, ObjectFactory*> f;
        return f;
    }

    
    
};




static DWORD genereateGuid() {
    static DWORD guid = 0;
    
    return ++guid;
}


#endif /* ObjectFactory_h */





