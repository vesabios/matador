//
//  Engine.hpp
//  matador
//
//  Created by vesabios on 10/7/15.
//
//

#ifndef Engine_h
#define Engine_h

#include <stdio.h>


class Engine {
    
public:
    void init();
    void update(float deltaTime);
    
};


extern Engine engine;


#endif /* Engine_h */
