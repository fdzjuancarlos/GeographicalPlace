//
//  State.h
//  MinimumGeographicCost
//
//  Created by carlos on 10/6/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#ifndef __MinimumGeographicCost__State__
#define __MinimumGeographicCost__State__

#include <iostream>
#include <cstdlib>
#include <iostream>     // std::cout, std::ios
#include <sstream>

class State{
    
    int page, x, y;
    float height;
    std::string action;
    
public:
    
    State(int n_page, int n_x, int n_y, int n_height, std::string &string);
    State()
    : page(0), x(0), y(0), height(0), action(*new std::string(""))
    {
    }
    
    
    int getPage();
    void setPage(int newPage);
    
    int getX();
    void setX(int newX);
    
    int getY();
    void setY(int newY);
    
    float getHeight();
    void setHeight(float newHeight);
    
    std::string getString();
    void setString(std::string &newString);
    
    
    
};

#endif /* defined(__MinimumGeographicCost__State__) */
