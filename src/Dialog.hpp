//
//  Dialog.hpp
//  matador
//
//  Created by vesabios on 10/22/15.
//
//

#ifndef Dialog_hpp
#define Dialog_hpp

#include <stdio.h>
#include "Console.h"

class Dialog {
    
public:
    
    vector<string> lines;
    
    ofVec2i center;
    
    bool active = false;
    
  
    
    void render();
    
};

extern Dialog dialog;

#endif /* Dialog_hpp */
