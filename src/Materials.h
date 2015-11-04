//
//  Materials.h
//  matador
//
//  Created by vesabios on 10/10/15.
//
//

#ifndef Materials_h
#define Materials_h

#include "Defines.h"

#include "Material.h"
#include "MStoneWall.h"
#include "MStoneFloor.h"
#include "MAlgae.h"
#include "MGreenGrass.h"
#include "MLightDirt.h"
#include "MDenseForest.h"
#include "MStoneRubble.h"
#include "MFortressFloor.h"
#include "MFortressWall.h"
#include "MWeeds.h"

#include "MVoid.h"


class Materials {
public:
    
    Material * mats[256];

    void init();
    
    Materials() {
        init();
        
    }
};

extern Materials materials;


#endif /* Materials_h */
