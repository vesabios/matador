//
//  Materials.cpp
//  matador
//
//  Created by vesabios on 10/10/15.
//
//

#include <stdio.h>
#include "Materials.h"


Materials materials;

void Materials::init() {
    
    mats[Material::DenseForest] = new class MDenseForest(Material::DenseForest);
    mats[Material::StoneWall] = new class MStoneWall(Material::StoneWall);
    mats[Material::StoneFloor] = new class MStoneFloor(Material::StoneFloor);
    mats[Material::Void] = new class MVoid(Material::Void);
    mats[Material::Algae] = new class MAlgae(Material::Algae);
    mats[Material::GreenGrass] = new class MGreenGrass(Material::GreenGrass);
    mats[Material::LightDirt] = new class MLightDirt(Material::LightDirt);
    mats[Material::StoneRubble] = new class MStoneRubble(Material::StoneRubble);
    mats[Material::FortressFloor] = new class MFortressFloor(Material::FortressFloor);
    mats[Material::FortressWall] = new class MFortressWall(Material::FortressWall);
    mats[Material::Weeds] = new class MWeeds(Material::Weeds);

    

}