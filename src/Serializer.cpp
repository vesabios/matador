//
//  Serializer.cpp
//  matador
//
//  Created by vesabios on 10/12/15.
//
//

#include "Serializer.hpp"
#include "ObjectFactory.h"
#include "Weapon.h"

Serializer serializer;


void Serializer::save() {
    
    printf("saving\n");
    
    FILE * file = NULL;
    file = fopen(ofToDataPath("data.dat").c_str(),"wb");
    
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
    
    for (int i=0; i<Object::elements().size(); i++) {
        
        printf("saving object: %d\n", i);
        
        msgpack::sbuffer sbuf;
        msgpack::packer<msgpack::sbuffer> pk(&sbuf);
        
        pk.pack(*(Object::elements()[i]->pack()));
        size_t dSize = sbuf.size();
        
        fwrite( &dSize, sizeof(size_t), 1, file);
        fwrite( sbuf.data(), sbuf.size(), 1, file);
        
    }
    
    fclose(file);
    
}


void Serializer::load() {
    

    
    for (int i=0; i<Object::elements().size(); i++) {
        delete Object::elements()[i];
    }
    
    printf("loading\n");
    Object::elements().clear();
    
    FILE * file = NULL;
    file = fopen(ofToDataPath("data.dat", true).c_str(), "rb");
    
    msgpack::unpacked result;
    
    bool end = false;
    while (!end) {
        size_t size = 0;
        fread(&size, sizeof(size_t),1,file);
        if ( size > 0) {
            char *buf = new char[size];
            fread(buf, size,1, file);
            msgpack::unpack( &result, buf, size);
            msgpack::object o = result.get();
            
            Msg h;
            o.convert(&h);
            
            Object::ObjectType t = ((Object::ObjectType)h.type);
            
            if (Object::doesObjectTypeExist(t)) {

                Object * n;
                
                if (t==Object::Player) {
                    
//                    n = core->player;
                    n = Object::create(t);
                    core->player = static_cast<Player *>(n);
                    o.convert(n);
                    n->unpack();
                    
                } else {
                    n = Object::create(t);
                    o.convert(n);
                    n->unpack();
                    
                }
                
            } else {
                printf("object type %d not found\n", t);
            }
            
            delete buf;
            
        } else {
            end=true;
        }
    }
    
    fclose(file);
    
    ofLog() << "Weapons: " << Weapon::elements().size();
    
    
}
