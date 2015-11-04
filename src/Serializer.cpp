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
        
        ofLog() << "saving object " << i <<": "<<Object::elements()[i]->getName();
        
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
        
        MapData * md = dynamic_cast<MapData*>(Object::elements()[i]);
        if (!md)
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
                
                n = Object::create(t);
                o.convert(n);
                n->unpack();
                
                if (t==Object::Player) {
                    engine.player = static_cast<Player *>(n);
                } else if (t==Object::MapData) {
                    MapData* md = dynamic_cast<MapData*>(n);
                    if (md) {
                        engine.mapData[md->id] = md;
                    }
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
    
    ofLog() << "Elements: " << Object::elements().size();
    
    
}
