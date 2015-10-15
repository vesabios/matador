//
//  Serializer.cpp
//  matador
//
//  Created by vesabios on 10/12/15.
//
//

#include "Serializer.hpp"

Serializer serializer;

/*
 void add( const T data )
 {
 if ( file != NULL )clear();
 if ( pkr != NULL) pkr->pack( data );
 else
 {
 ofLogError() << "please call ofxMsgPacker::setup";
 }
 size_t dSize = size();
 if ( file != NULL ) //write to file
 {
 fwrite( &dSize, sizeof(size_t), 1, file);
 fwrite( sbuff.data(), size(), 1, file);
 }
 }
 */


void Serializer::save() {
    
    printf("saving\n");
    
    FILE * file = NULL;
    file = fopen(ofToDataPath("data.dat").c_str(),"wb");
    
    msgpack::sbuffer sbuf;
    msgpack::packer<msgpack::sbuffer> pk(&sbuf);
    
    for (int i=0; i<core->objects.size(); i++) {
        
        printf("saving object: %d\n", i);
        
        msgpack::sbuffer sbuf;
        msgpack::packer<msgpack::sbuffer> pk(&sbuf);
        
        pk.pack(*(core->objects[i]->pack()));
        size_t dSize = sbuf.size();
        
        fwrite( &dSize, sizeof(size_t), 1, file);
        fwrite( sbuf.data(), sbuf.size(), 1, file);
        
    }
    
    fclose(file);
    
}


void Serializer::load() {
    
    printf("loading\n");

    
    FILE * file = NULL;
    file = fopen(ofToDataPath("data.dat", true).c_str(), "rb");
    
    msgpack::unpacked result;
    
    bool end = false;
    while (!end) {
        printf("loading next...\n");
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
                    
                    n = core->player;
                    o.convert(n);
                    n->unpack();
                    
                } else {
                    n = Object::create(t);
                    o.convert(n);
                    n->unpack();
                    
                    core->objects.push_back(n);
                }


                
            } else {
                printf("object type %d not found\n", t);
            }
            
        } else {
            end=true;
        }
    }
    
}
