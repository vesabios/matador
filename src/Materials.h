//
//  Materials.h
//  matador
//
//  Created by vesabios on 10/10/15.
//
//

#ifndef Materials_h
#define Materials_h

enum MaterialType {
    VoidMat,
    StoneWallMat,
    StoneFloorMat,
    AlgaeMat,
    GreenGrassMat,
    DenseForestMat
};


#include "Material.h"
#include "MStoneWall.h"
#include "MStoneFloor.h"
#include "MAlgae.h"
#include "MGreenGrass.h"
#include "MDenseForest.h"
#include "MVoid.h"


class Materials {
public:
    
    Material * mats[256];

    void init() {
        
        mats[DenseForestMat] = new class MDenseForest(DenseForestMat);
  
        mats[StoneWallMat] = new class MStoneWall(StoneWallMat);
        mats[StoneFloorMat] = new class MStoneFloor(StoneFloorMat);
        mats[VoidMat] = new class MVoid(VoidMat);
        mats[AlgaeMat] = new class MAlgae(AlgaeMat);
        mats[GreenGrassMat] = new class MGreenGrass(GreenGrassMat);

    }
    
    Materials() {
        init();
        
    }
};

extern Materials materials;


#endif /* Materials_h */
