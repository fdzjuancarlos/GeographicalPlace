//
//  Problem.h
//  MinimumGeographicCost
//
//  Created by carlos on 10/6/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__Problem__
#define __MinimumGeographicCost__Problem__

#include <iostream>
#include <vector>
#include <stack> 
#include "Sucessor.h"
#include "Frontera.h"
#include "GeographicalMap.h"

class Problem{
    
    GeographicalMap *currentMap;
    
    State initialState;
    State objective;
    Frontera frontier;
    std::vector<State> solution;
    bool optimization;
    
public:
    
    
    Problem(State &n_initialState, State &n_objective, GeographicalMap &n_currentMap, bool n_optimization);
    
    State getInitialState();
    void setInitialState(State &n_initialState);
    
    State getObjetive();
    void setObjective(State &n_objective);
    
    std::vector<Sucessor>* followingStates(Sucessor &father);
    
    std::vector<State*> getSolution(NodoArbolBusqueda* hijo);
    
    void stressTest();
    std::vector<State*> search(std::string estrategia, int max_prof, std::vector<unsigned long>* metadata);
    bool target(State *state);
    
};

#endif /* defined(__MinimumGeographicCost__Problem__) */
