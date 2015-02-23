//
//  GeographicalMap.h
//  MinimumGeographicCost
//
//  Created by carlos on 9/17/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__GeographicalMap__
#define __MinimumGeographicCost__GeographicalMap__

#include <iostream>
#include <vector>
#include "Page.h"
#include "GlobalVariables.h"
#include "State.h"


//#include <zip.h>

class GeographicalMap{
    
    std::vector<Page*> v_pages;
    std::vector<int> avaiableMaps;
    int *memory_Page;
    float **memory_Matrix;
    
    GeographicalMap(){}
    
    
    
    
public:
    
    GeographicalMap(std::vector<int> maps);
    
    void geographicalPlace(int umt_x, int umt_y, std::vector<int>*& upperVector);
    void coordToUmt(int page, int row, int column, std::vector<int>*& solVector);
    float height(int pageNumber,int coord_x, int coord_y);
    Page* getPage(int pageNumber);
    
};

#endif /* defined(__MinimumGeographicCost__GeographicalMap__) */
