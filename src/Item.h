//
//  Item.h
//  matador
//
//  Created by vesabios on 10/17/15.
//
//

#ifndef Item_h
#define Item_h
#include "ObjectFactory.h"

class Actor;

class Item : public Object {
public:
    
    virtual DEBT use(Actor * a) = 0;
    
};


#endif /* Item_h */
