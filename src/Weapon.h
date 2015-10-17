//
//  Weapons.h
//  matador
//
//  Created by vesabios on 10/15/15.
//
//

#ifndef Weapons_h
#define Weapons_h

class Weapon : public Item {
public:
    struct data_t {
        BYTE numberOfDice;
        BYTE die;
    };
    
    data_t data;
    
    void initReflectors() override {
        REFLECT(numberOfDice)
        REFLECT(die)
    }
    
    BYTE rollAttack() {
        BYTE dmg = 0;
        for (int i=0; i<data.numberOfDice; i++) {
            dmg += (int)ofRandom(data.die)+1;
        }
        printf("%dd%d rollAttack: %d\n", data.numberOfDice, data.die, dmg);
        return dmg;
    }
    
    
    
    static std::vector<Weapon*> &weapons() {
        static std::vector<Weapon*> v;
        return v;
    }
    
    
    Weapon() {
        ofLog() << "weapon ctor";
        weapons().push_back(this);
    }
    
    virtual ~Weapon() {
        ofLog() << "weapon dtor";
        weapons().erase(std::remove(weapons().begin(), weapons().end(), this), weapons().end());
    }
     
    
    
    
};


#endif /* Header_h */
