//
//  Page.h
//  MinimumGeographicCost
//
//  Created by carlos on 9/18/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__Page__
#define __MinimumGeographicCost__Page__

#include <iostream>
#include <vector>

class Page{
    
    int name;
    int columns;
    int rows;
    int xllCenter;
    int yllCenter;
    int cellSize;
    float **height_Matrix;
    int noData;
    Page(){}
    
public:
    
    Page(char *file, int numberPage);
    Page(int attributes[], float **matrix);

    
    void geographicalPlace(int umt_x, int umt_y, std::vector<int>*& upperVector);
    void coordToUmt(int row, int column, std::vector<int>*& solVector);
    bool checkBounds(int umt_x, int umt_y);
    float height (int column, int row);
    
    int getName();
    int getColumns();
    int getRows();
    int getXllCenter();
    int getYllCenter();
    int getCellSize();
    int getNoData();
    float** getMatrixPointer();



    
};

#endif /* defined(__MinimumGeographicCost__Page__) */
