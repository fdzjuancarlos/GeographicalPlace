//
//  Sucessor.h
//  MinimumGeographicCost
//
//  Created by carlos on 14/10/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__Sucessor__
#define __MinimumGeographicCost__Sucessor__

#include <iostream>
#include "State.h"

class Sucessor {
    State* newState;
    std::string action;
    float absoluteSlope;
    bool changeDirection;
    
public:
    
    Sucessor(State &state, std::string &_action, float slope, bool _direction);
    Sucessor(State *state, std::string &_action, float slope, bool _direction);

    ~Sucessor();
    
    State getState();
    State* p_getState();
    float getSlope();
    std::string getAction();
    bool getChangedDirection();
};

#endif /* defined(__MinimumGeographicCost__Sucessor__) */
