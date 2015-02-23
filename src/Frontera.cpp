//
//  Frontera.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 15/10/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "Frontera.h"


Frontera::Frontera(){
    frontier = new std::priority_queue<NodoArbolBusqueda*, std::vector<NodoArbolBusqueda*>, LessValoration>;
    tree = new std::queue<NodoArbolBusqueda*>;
    optimizationDictionary = new std::map<std::string, double>();

}

Frontera::~Frontera(){/*
    delete frontier;
    NodoArbolBusqueda* toDelete;
    while(tree->size() != 0){
        toDelete = tree->front();
        delete toDelete;
    }
    delete tree;*/
}

double Frontera::addNode(NodoArbolBusqueda *node, bool optimization){
    double valoration = node->getValoration();

    if (!optimization)
        frontier->push(node);
    else if (optimizationDictionary->count(node->getHashName())){
        
        
        if(valoration < 0) //Profundidad
            valoration = -valoration;
        
        if (optimizationDictionary->at(node->getHashName()) > valoration){
            optimizationDictionary->erase(node->getHashName());
            optimizationDictionary->insert ( std::pair<std::string,double>(node->getHashName(),
                                                                        valoration) );
            frontier->push(node);
        }
        
        
    }else{
        frontier->push(node);
        optimizationDictionary->insert ( std::pair<std::string,double>(node->getHashName(),
                                                                    valoration) );
    }
    
    return 0;
}

NodoArbolBusqueda* Frontera::getNode(){
    NodoArbolBusqueda* value= frontier->top();
    frontier->pop();
    return value;
}

bool Frontera::isEmpty(){
    return frontier->empty();
}