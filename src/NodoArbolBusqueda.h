//
//  NodoArbolBusqueda.h
//  MinimumGeographicCost
//
//  Created by carlos on 14/10/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__NodoArbolBusqueda__
#define __MinimumGeographicCost__NodoArbolBusqueda__

#include <iostream>
#include <cmath>
#include "State.h"
#include "Sucessor.h"
#include "GeographicalMap.h"
#include <cstdlib>
#include <iostream>     // std::cout, std::ios
#include <sstream>

class NodoArbolBusqueda {
    
    unsigned long id;
    NodoArbolBusqueda* father;
    
    /*Information*/
    State* actualState;
    std::string action;
    int deep;
    double valoration;
    float coste;
    float absoluteSlope;
    float heuristic;
    int f_umt_x;
    int f_umt_y;
    bool changedDirection;
    
public:
    
    NodoArbolBusqueda(unsigned long name,Sucessor& n_sucessor, NodoArbolBusqueda* n_father, std::string estrategia, GeographicalMap* map);
    NodoArbolBusqueda(unsigned long name,Sucessor& n_sucessor, NodoArbolBusqueda* n_father, std::string estrategia, GeographicalMap* map, State* objective);
    int getDeep();
    double getValoration() const;
    State* getState();
    std::string getAction();
    float getAbsoluteSlope();
    bool getChangedDirection();
    NodoArbolBusqueda* getFather();
    float getCoste();
    unsigned long getId();
    inline std::string getHashName(){
        std::ostringstream stringStream;
        stringStream << actualState->getPage() << "-" << actualState->getX() << "-" <<actualState->getY();
        
        return stringStream.str();
    }
};

#endif /* defined(__MinimumGeographicCost__NodoArbolBusqueda__) */
