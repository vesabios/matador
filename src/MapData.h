//
//  MapData.h
//  matador
//
//  Created by vesabios on 10/17/15.
//
//

#ifndef MapData_h
#define MapData_h
#include "ObjectFactory.h"

class MapData : public Object {
public:
    
    BYTE id = 0;
    
    struct data_t {
        bool visited = false;
        BYTE ambient = 0;
        bool procedural = false;
    };
    
    data_t data;
    
    void initReflectors() override {
        REFLECT(visited)
        REFLECT(ambient)
        REFLECT(procedural)
    }
    
    
    virtual string getName() override { return "MapData"; }
    virtual DEBT interactable() override { return 0; }
    virtual DEBT traversable() override { return 0; }
    virtual bool isPortable() override { return false; }
    virtual Pixel render(float luma) override { Pixel p; return p;};
    virtual float update(DEBT d) override { return 0.0f; };
    virtual InteractionType getInteractionTypeForInteractor(Object *) override { return None; }
    virtual void init() override  {};
    
    OBJTYPE(MapData);
    MSGPACK_DEFINE(type, id, bundle);
    
};
#endif /* MapData_h */
