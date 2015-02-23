//
//  main.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 9/17/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include "GeographicalMap.h"
#include "Problem.h"
#include "Sucessor.h"
#include "hdf5.h"
#include "Frontera.h"



int main(int argc, const char * argv[]){

    std::vector<int> avaiableMaps = *new std::vector<int> ();
    avaiableMaps.push_back(711); avaiableMaps.push_back(712);
    avaiableMaps.push_back(736); avaiableMaps.push_back(737);

    GeographicalMap myMap(avaiableMaps);
    
    /*-- UMT to Page-Row-Column Coordinates --*/
    std::vector<int>* coords;

    
    /*-- Page--Row--Column for Height Data --*/
    /*
    std::cout.precision(4);
    std::cout << "Prueba para conocer altura de 711,776,0: " <<myMap.height(711,776,0)<< std::endl;
    
    myMap.coordToUmt(736, 0, 0, coords);
    std::cout << "Prueba de coordenadas 736, 0, 0 a UMT: " <<coords->at(0) << " y " << coords->at(1) << std::endl;
    
    //limite superior de 736 el 4354850
    
    myMap.geographicalPlace(397400, 4354850, coords);
    
    if(coords->at(0) != -1){
        std::cout << "Pruebas de UMT 397400 4354850 a Coordenadas: " << coords->at(0) << " en la \ncolumna: "
        << coords->at(1) << " fila: " << coords->at(2) << "\n";
    }else
        std::cout << "Coordinates out of Bounds";
    
    myMap.geographicalPlace(397400, 4353800, coords);
    
            std::cout << "Pruebas de UMT 397400 4353800 a Coordenadas: " << coords->at(0) << " en la \ncolumna: "
        << coords->at(1) << " fila: " << coords->at(2) << "\n";*/
    
    //Followers Checking by terminal
    int page, row, column, deep, init_umt_x, init_umt_y, obj_umt_x, obj_umt_y;
    std::string estrategia;
    std::cout << "Para el estado Inicial" << std::endl;
    std::cout << "Indique UMT_X" << std::endl;
    std::cin >> init_umt_x;
    //umt_x = 397400;
    //init_umt_x = 404079; //Optimizacion A hardcore
    //init_umt_x = 398638; //B
    //init_umt_x = 397388; //A
    std::cout << "Indique UMT_Y" << std::endl;
    std::cin >> init_umt_y;
    //umt_y = 4353800;
    //init_umt_y = 4369026; //Optimizacion A hardcore
    //init_umt_y = 4355039; //B
    //init_umt_y = 4353788; //A
    
    myMap.geographicalPlace(init_umt_x, init_umt_y, coords);
    page = coords->at(0);
    row = coords->at(1);
    column = coords->at(2);
    
    State initial = *new State(page,row,column, myMap.height(page, row, column), *new std::string(""));
    std::cout << "Para el estado Final" << std::endl;
    std::cout << "Indique UMT_X" << std::endl;
    std::cin >> obj_umt_x;
    //umt_x = 397800;
    //obj_umt_x = 399690; //Optimizacion A hardcore
    //obj_umt_x = 398638; //B
    //obj_umt_x = 399690; //A
    std::cout << "Indique UMT_Y" << std::endl;
    std::cin >> obj_umt_y;
    //umt_y = 4354150;
    //obj_umt_y = 4360861; //Optimizacion A hardcore
    //obj_umt_y = 4350039; //B
    //obj_umt_y = 4360861; //A
    std::cout << "Indique estrategia a aplicar para resolver" << std::endl;
    std::cin >> estrategia;
    std::cout << "Indique profundidad maxima" << std::endl;
    std::cin >> deep;
    std::cout << "Indique opción:" << std::endl;
    std::cout << "1.-Optimización activa" << std::endl;
    std::cout << "0.-Optimización inactiva" << std::endl;
    bool optimization;
    int temp;
    std::cin >> temp;
    optimization = (temp==1);
    
    std::vector<int>* coordsObj;

    
    myMap.geographicalPlace(obj_umt_x, obj_umt_y, coordsObj);
    page = coordsObj->at(0);
    row = coordsObj->at(1);
    column = coordsObj->at(2);
    
    State final = *new State(page,row,column, myMap.height(page, row, column), *new std::string(""));
    std::string cadenavacia("");
    Sucessor root(initial, cadenavacia, 0, false);
    
    Problem firstProblem = *new Problem(initial, final, myMap, optimization);
    
    std::vector<unsigned long>* metaData = new std::vector<unsigned long>();
    std::vector<State*> sucesores = firstProblem.search(estrategia, deep, metaData);
    std::ofstream myfile;
    std::ostringstream ostream;
    ostream << "./[" << init_umt_x << "] [" << init_umt_y << "]" << "_a_" << "[" << obj_umt_x
    << "][" << obj_umt_y << "]" << "_Estrategia_" << estrategia << ".txt";
    myfile.open (ostream.str());
    if(!metaData->empty()){
        std::cout << "Coste: " << metaData->at(0) << " Profundidad de Solucion:" << metaData->at(1);
        std::cout << " Profundidad Total: " << metaData->at(4);
        std::cout << " Segundos: " << metaData->at(3)/1000000000  << std::endl;
        myfile << "Coste: " << metaData->at(0) << " Profundidad de Solucion:" << metaData->at(1);
        myfile << " Profundidad Total: " << metaData->at(4);
        myfile << "Segundos: " << metaData->at(3)/1000000000  << std::endl;
    
    for(std::vector<State*>::iterator it = sucesores.begin(); it != sucesores.end(); ++it){
        std::vector<int>* coords;

        myMap.coordToUmt((*it)->getPage(), (*it)->getX(), (*it)->getY(), coords);
        std::cout << "UMT_X[" << coords->at(0) << "] UMT_Y[" << coords->at(1) << "]"  <<
        " HEIGHT[" <<
        myMap.height((*it)->getPage(), (*it)->getX(), (*it)->getY()) << "]" << std::endl;
        myfile << "UMT_X[" << coords->at(0) << "] UMT_Y[" << coords->at(1) << "]"  <<
        " HEIGHT[" <<
        myMap.height((*it)->getPage(), (*it)->getX(), (*it)->getY()) << "]" << std::endl;
    }
    }else{
        std::cout << "No existe solucion con los parametros indicados";
        myfile << "No existe solucion con los parametros indicados";
    }

    myfile.close();

    

    
    
    return 0;
}

