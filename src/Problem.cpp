//
//  Problem.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 10/6/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "Problem.h"
#include "Frontera.h"
#include <cmath>

Problem::Problem(State &n_initialState, State &n_objective, GeographicalMap &n_currentMap, bool n_optimization){
    initialState = n_initialState;
    objective = n_objective;
    currentMap = &n_currentMap;
    solution = *new std::vector<State>;
    optimization = n_optimization;
}

std::vector<State*> Problem::search(std::string estrategia, int max_prof, std::vector<unsigned long>* metadata){
    
    auto start = std::chrono::high_resolution_clock::now();
    bool solution = false;
    NodoArbolBusqueda* nodoActual = NULL;
    std::vector<Sucessor> *listSucessor;
    
    std::string cadenavacia("");
    Sucessor root(initialState, cadenavacia, 0, false);
    unsigned long id = 0;
    unsigned long maxDeep = 0;
    NodoArbolBusqueda *nodoPadre = new NodoArbolBusqueda(id, root, NULL, estrategia, currentMap);
    frontier.addNode(nodoPadre, optimization);
    
    
    

    while (!solution && !frontier.isEmpty()){
        nodoActual  = frontier.getNode();
        if (target(nodoActual->getState()))
            solution = true;
        else{
            if (nodoActual->getDeep()<max_prof){
                std::string cadena=nodoActual->getAction();
                Sucessor sucesorNodoActual(nodoActual->getState(), cadena,nodoActual->getAbsoluteSlope(), nodoActual->getChangedDirection());
                
                listSucessor = followingStates(sucesorNodoActual);
                
                if(nodoActual->getDeep()+1 > maxDeep){
                    maxDeep= nodoActual->getDeep()+1;
                }
                
                for(std::vector<Sucessor>::iterator it = listSucessor->begin(); it != listSucessor->end(); ++it){
                    NodoArbolBusqueda* hijo = new NodoArbolBusqueda(++id, *it, nodoActual, estrategia, currentMap, &objective);
                    frontier.addNode(hijo, optimization);
                }

                
            }
        }
        
        
        
    }
    auto finish = std::chrono::high_resolution_clock::now();

    
    if(solution){
        metadata->push_back(nodoActual->getCoste());
        metadata->push_back(nodoActual->getDeep());
        metadata->push_back(id);
        metadata->push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count());
        metadata->push_back(maxDeep);
    }
    
    /*#Resultado
    si solución entonces
    devolver CreaSolución(n_actual)
    si_no
    devolver NO_Solución*/
    if (solution) {
        return getSolution(nodoActual);
    }else{
        return std::vector<State*>();
    }
}



void Problem::stressTest(){
    std::string cadenavacia("");
    Sucessor root(initialState, cadenavacia, 0, false);
    std::vector<Sucessor> *listSucessor = followingStates(root);
    unsigned long id = 0;
    NodoArbolBusqueda *nodoPadre = new NodoArbolBusqueda(id, root, NULL, "profundidad", currentMap);
    double nanosec;
    double accumulated = 0;
    double max=0;
    double min=10000;
    
    //160 millones de nodos permitidos en memoria
    
    //575.2 nanosegundos media [sube y baja]
    //98: Min
    //2.709e+09: Max
    while(true){ //ouch!
        for(std::vector<Sucessor>::iterator it = listSucessor->begin(); it != listSucessor->end(); ++it)
        {
            NodoArbolBusqueda* hoja = new NodoArbolBusqueda(++id, *it, nodoPadre, "profunidad" ,currentMap);
            nanosec=frontier.addNode(hoja, true);
            accumulated += nanosec;
            if (nanosec < min)
                min=nanosec;
            if (nanosec > max)
                max=nanosec;
            if(id % 10000000 == 0){
                std::cout << id << std::endl;
                std::cout << (accumulated/(id+1)) << " de media" << std::endl;
                std::cout << min << ": Min" << std::endl;
                std::cout << max << ": Max" << std::endl;
                std::cout << std::endl;
            }
        }
        nodoPadre = frontier.getNode();
        //falta recargar listSucessor
        
    }
    
}

State Problem::getInitialState(){
    return initialState;
}

void Problem::setInitialState(State &n_initialState){
    initialState = n_initialState;
}

State Problem::getObjetive(){
    return objective;
}

void Problem::setObjective(State &n_objective){
    objective = n_objective;
}

std::vector<Sucessor>* Problem::followingStates(Sucessor &father){
    
    State* possibleStates;
    State state = (father.getState());
    std::vector<Sucessor>* listSucessors = new std::vector<Sucessor>();
    
    

    std::vector<int> *newCoords;
    std::vector<int> *umt_coords;
    float height;
    float errorValue = (currentMap->getPage(state.getPage()))->getNoData();
    int cellsize = currentMap->getPage(state.getPage())->getCellSize();
    int actualX, actualY;
    float slope;
    std::string action;
    
    // X O O
    // O O O
    // O O O
    action = *new std::string("NW");
    actualX= state.getX(); actualY= state.getY();
    
    height = currentMap->height(state.getPage(), actualX-1, actualY-1);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX-1, actualY-1, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));

    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0)-cellsize, umt_coords->at(1)+cellsize, newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    
    // O X O
    // O O O
    // O O O
    action = *new std::string("N");
    actualX= state.getX(); actualY= state.getY();
    
    height = currentMap->height(state.getPage(), actualX, actualY-1);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX, actualY-1, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0), umt_coords->at(1)+cellsize, newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    // O O X
    // O O O
    // O O O
    action = *new std::string("NE");
    
    height = currentMap->height(state.getPage(), actualX+1, actualY-1);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX+1, actualY-1, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0)+cellsize, umt_coords->at(1)+cellsize, newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    // O O O
    // X O O
    // O O O
    action = *new std::string("W");
    
    height = currentMap->height(state.getPage(), actualX-1, actualY);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX-1, actualY, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0)-cellsize, umt_coords->at(1), newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    // O O O
    // O O X
    // O O O
    action = *new std::string("E");
    
    height = currentMap->height(state.getPage(), actualX+1, actualY);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX+1, actualY, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0)+cellsize, umt_coords->at(1), newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    // O O O
    // O O O
    // X O O
    action = *new std::string("SW");
    
    height = currentMap->height(state.getPage(), actualX-1, actualY+1);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX-1, actualY+1, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0)-cellsize, umt_coords->at(1)-cellsize, newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    // O O O
    // O O O
    // O X O
    action = *new std::string("S");
    
    height = currentMap->height(state.getPage(), actualX, actualY+1);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX, actualY+1, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0), umt_coords->at(1)-cellsize, newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
    }
    
    // O O O
    // O O O
    // O O X
    action = *new std::string("SE");
    
    height = currentMap->height(state.getPage(), actualX+1, actualY+1);
    if(height != -999){
        possibleStates = new State(state.getPage(), actualX+1, actualY+1, height
                                            , action);
        slope =  fabs(father.getState().getHeight() - height);
        listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
    }
    else{
        currentMap->coordToUmt(state.getPage(), actualX, actualY , umt_coords);
        currentMap->geographicalPlace(umt_coords->at(0)+cellsize, umt_coords->at(1)-cellsize, newCoords);
        if(newCoords->at(0) != -1){
            height = currentMap->height(newCoords->at(0), newCoords->at(1), newCoords->at(2));
            possibleStates = new State(newCoords->at(0), newCoords->at(1), newCoords->at(2), height, action);
            slope =  fabs(father.getState().getHeight() - height);
            listSucessors->push_back(*new Sucessor(*possibleStates, action, slope, father.getAction().compare(action)));
        }
        
        
    }
    
    
    
    
    return listSucessors;
}

bool Problem::target(State *state){
    return state->getX() == objective.getX() && state->getY() == objective.getY() && state->getPage() == objective.getPage();
}

std::vector<State*> Problem::getSolution(NodoArbolBusqueda* hijo){
    std::vector<State*> path = std::vector<State*>();
    NodoArbolBusqueda* actualNode = hijo;
    std::stack<NodoArbolBusqueda*> stack;
    
    while (actualNode->getFather() != NULL){
        stack.push(actualNode);
        actualNode = actualNode->getFather();
    }
    stack.push(actualNode);
    
    while (!stack.empty()){
        actualNode= stack.top();
        stack.pop();
        path.push_back(actualNode->getState());
    }
    
    
    return path;
}