//
//  ObjectFactory.cpp
//
//  Created by vesabios on 10/11/15.
//  Copyright © 2015 vesabios. All rights reserved.
//

#include "ObjectFactory.h"



class MapData : Object {
public:
    
    BYTE id = 0;
    
    struct data_t {
        bool visited = false;
        int ambient = 0;
        int procedural = false;
    };
    
    data_t data;
    
    void initReflectors() override {
        REFLECT(visited)
        REFLECT(ambient)
        REFLECT(procedural)
    }
    
    OBJTYPE(MapData);
    MSGPACK_DEFINE(type, id, bundle);
    
};



