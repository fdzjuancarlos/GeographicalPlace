//
//  NodoArbolBusqueda.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 14/10/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "NodoArbolBusqueda.h"


NodoArbolBusqueda::NodoArbolBusqueda(unsigned long name, Sucessor& n_sucessor, NodoArbolBusqueda*n_father, std::string estrategia, GeographicalMap* map){
    
    actualState = n_sucessor.p_getState();
    action = n_sucessor.getAction();
    absoluteSlope = n_sucessor.getSlope();
    changedDirection = n_sucessor.getChangedDirection();
    float ld;
    
    f_umt_x = 0;
    f_umt_y = 0;
    
    if(n_father == NULL){
        id = 0;
        father = NULL;
        deep = 0;
    }else{
        id = name;
        father = n_father;
        deep = n_father->getDeep() +1;
    }
    
    if(n_father != NULL){
        if (action.compare("N") == 0 || action.compare("S") == 0 ||
            action.compare("E") == 0 || action.compare("O") == 0)
            ld = 25;
        else
            ld = sqrtf(2*25*25);
        
        coste = sqrtf(ld*ld+absoluteSlope*absoluteSlope);
        if (changedDirection)
            coste = coste * 1.1;
    }else
        coste=0;
    
    //heuristic = map->coordToUmt(actualState->getPage(), actualState->getY(), actualState->getX(), )
    

    
    
}

NodoArbolBusqueda::NodoArbolBusqueda(unsigned long name, Sucessor& n_sucessor, NodoArbolBusqueda*n_father, std::string estrategia, GeographicalMap* map, State* objective){
    
    actualState = n_sucessor.p_getState();
    action = n_sucessor.getAction();
    absoluteSlope = n_sucessor.getSlope();
    changedDirection = n_sucessor.getChangedDirection();
    float ld;
    
    
    
    if(n_father == NULL){
        id = 0;
        father = NULL;
        deep = 0;
    }else{
        id = name;
        father = n_father;
        deep = n_father->getDeep() +1;
    }
    
    if(n_father != NULL){
        if (action.compare("N") == 0 || action.compare("S") == 0 ||
            action.compare("E") == 0 || action.compare("O") == 0)
            ld = 25;
        else
            ld = sqrtf(2*25*25);
        
        coste = sqrtf(ld*ld+absoluteSlope*absoluteSlope);
        if (changedDirection)
            coste = coste * 1.1;
        
        coste = coste + n_father->getCoste();
    }else
        coste=0;
    
    std::vector<int>* umdState;
    map->coordToUmt(actualState->getPage(), actualState->getX(), actualState->getY(), umdState); //FIXME
    std::vector<int>* umdObjective;
    map->coordToUmt(objective->getPage(), objective->getX(), objective->getY(), umdObjective); //FIXME
    
    f_umt_x = umdState->at(0);
    f_umt_y = umdState->at(1);
    
    int xdxn =abs((umdObjective->at(0)-umdState->at(0)));
    int ydyn =abs((umdObjective->at(1)-umdState->at(1)));
    
    int cuadX = xdxn * xdxn;
    int cuadY = ydyn * ydyn;
    
    heuristic = sqrt(cuadX+cuadY);
    

    
    
    if(estrategia.compare("profundidad") == 0){
        valoration= -deep;
    }else if(estrategia.compare("anchura") == 0){
        valoration= deep;
    }else if(estrategia.compare("uniforme") == 0){
        valoration= coste;
    }else if(estrategia.compare("voraz") == 0){
        valoration= heuristic;
    }else if(estrategia.compare("A") == 0){
        valoration= coste+heuristic;
    }
    
    
    /*if(actualState->getPage()== 736)
        std::cout << actualState->getPage() << std::endl;*/
    /*if(actualState->getX() == 61)
    std::cout << actualState->getPage() << "-" << actualState->getX() << "-" << actualState->getY() << std::endl;*//*
    
    if(actualState->getX() == 51)
        std::cout << actualState->getPage() << "-" << actualState->getX() << "-" << actualState->getY() << std::endl;*/
    
}

std::string NodoArbolBusqueda::getAction(){
    return action;
}

int NodoArbolBusqueda::getDeep(){
    return deep;
}

double NodoArbolBusqueda::getValoration() const{
    return valoration;
}

State* NodoArbolBusqueda::getState(){
    return actualState;
}

float NodoArbolBusqueda::getAbsoluteSlope(){
    return absoluteSlope;
}

bool NodoArbolBusqueda::getChangedDirection(){
    return changedDirection;
}

NodoArbolBusqueda* NodoArbolBusqueda::getFather(){
    return father;
}

float NodoArbolBusqueda::getCoste(){
    return coste;
}
unsigned long NodoArbolBusqueda::getId(){
    return id;
}






/*
NodoArbolBusqueda& NodoArbolBusqueda::operator=(NodoArbolBusqueda &rshs){
    
    return *this;
}*/
