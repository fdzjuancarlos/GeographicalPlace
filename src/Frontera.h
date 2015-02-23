//
//  Frontera.h
//  MinimumGeographicCost
//
//  Created by carlos on 15/10/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__Frontera__
#define __MinimumGeographicCost__Frontera__

#include <iostream>
#include <vector>
#include "NodoArbolBusqueda.h"
#include <queue>
#include <map>

struct LessValoration
{
    bool operator()(const NodoArbolBusqueda* rn, const NodoArbolBusqueda* ln) const
    {
        return ln->getValoration() < rn->getValoration();
    }
};

class Frontera {
    std::queue<NodoArbolBusqueda*>* tree;
    std::priority_queue<NodoArbolBusqueda*, std::vector<NodoArbolBusqueda*>, LessValoration>* frontier;
    std::map<std::string, double>* optimizationDictionary;
    
public:
    
    Frontera();
    ~Frontera();
    double addNode(NodoArbolBusqueda *nodo, bool optimization);
    NodoArbolBusqueda* getNode();
    
    bool isEmpty();
};

#endif /* defined(__MinimumGeographicCost__Frontera__) */
