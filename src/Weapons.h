//
//  Weapons.h
//  matador
//
//  Created by vesabios on 10/15/15.
//
//

#ifndef Weapons_h
#define Weapons_h


class Weapons {
public:
    
    enum Properties {
        Light,
        Finesse,
        Thrown,
        Versatile,
        Ammunition,
        TwoHanded
    };

    BYTE numberOfDice;
    BYTE die;
    DamageType damageType;
    
};



#endif /* Header_h */
