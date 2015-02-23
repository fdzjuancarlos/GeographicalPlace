//
//  Sucessor.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 14/10/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "Sucessor.h"

Sucessor::Sucessor(State &state, std::string &_action, float slope, bool _direction){
    newState = &state;
    action = _action;
    absoluteSlope = slope;
    changeDirection = _direction;
}

Sucessor::Sucessor(State *state, std::string &_action, float slope, bool _direction){
    newState = state;
    action = _action;
    absoluteSlope = slope;
    changeDirection = _direction;
}

Sucessor::~Sucessor(){}

State Sucessor::getState(){
    return *newState;
}

State* Sucessor::p_getState(){
    return newState;
}




float Sucessor::getSlope(){
    return absoluteSlope;
}
std::string Sucessor::getAction(){
    return action;
}

bool Sucessor::getChangedDirection(){
    return changeDirection;
}
