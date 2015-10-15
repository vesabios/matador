//
//  Serializer.hpp
//  matador
//
//  Created by vesabios on 10/12/15.
//
//

#ifndef Serializer_hpp
#define Serializer_hpp

#include "ofMain.h"
#include "Core.h"
#include <msgpack.hpp>

class Serializer {
    

public:
    
    void save();
    void load();
     
    
};

extern Serializer serializer;

#endif /* Serializer_hpp */
