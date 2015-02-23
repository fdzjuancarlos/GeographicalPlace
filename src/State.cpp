//
//  State.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 10/6/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "State.h"

State::State(int n_page, int n_x, int n_y, int n_height, std::string &string){
    setPage(n_page);
    setX(n_x);
    setY(n_y);
    setHeight(n_height);
    setString(string);
}



int State::getPage(){
    return page;
}
void State::setPage(int newPage){
    page = newPage;
}

int State::getX(){
    return x;
}

void State::setX(int newX){
    x = newX;
}

int State::getY(){
    return y;
}
void State::setY(int newY){
    y= newY;
}

float State::getHeight(){
    return height;
}
void State::setHeight(float newHeight){
    height = newHeight;
}

std::string State::getString(){
    return action;
}
void State::setString(std::string &newString){
    action = newString;
}