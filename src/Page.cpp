//
//  Page.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 9/18/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "Page.h"
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/resource.h>
#include "zip.h"

using namespace std;

Page::Page(int attributes[], float **matrix){
    
    
    name = attributes[0];
    rows = attributes[1];
    columns = attributes[2];
    xllCenter = attributes[3];
    yllCenter = attributes[4];
    cellSize = attributes[5];
    noData = attributes[6];
        
    height_Matrix = new float*[rows];
    for(int i = 0; i < rows; ++i)
        height_Matrix[i] = new float[columns];
    
    for (int i=0; i<rows; i++){
        for( int j=0; j<columns; j++)
            height_Matrix[i][j] = matrix[i][j];
    }
}


Page::Page(char* file, int numberPage){
    
    
    char output[100];
    string s_data;
    name = numberPage;
    bool gettingMetadata = true;
    //za = zip_open(final_path, 0, &err);
    const char *prg;
    int fd, len;
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    long long sum;
    int err;
    
    //fstream *filee;
    
    if ((za = zip_open(file, 0, &err)) == NULL) {
       // zip_error_to_str(buf, sizeof(buf), err, errno);
        //fprintf(stderr, "%s: can't open zip archive `%s': %s/n", prg,
         //       file, buf);
        //return 1;
    }
    zip_stat_index(za, 0, 0, &sb);
    zf = zip_fopen_index(za, 0, 0);
    
    char str[10];

    //Creates an instance of ofstream, and opens example.txt
    ofstream a_file ( "diskbufferr.txt" );

    char buf[sb.size/5];
    char line[sb.size/5];
    int i=0;
    sum = 0;

    while (sum < sb.size) {
        len = zip_fread(zf, buf, 5000);

        
        sum += len;
        
        a_file << buf;
        //std::cout << buf << endl;

    }
    a_file.close();
    ifstream filee ( "diskbufferr.txt" );
    
    
    if (filee.is_open()) {
        while (gettingMetadata){
            filee.getline(output, 100); //Getting info from metadata file, first line
            char* typeOfData = strtok(output, " ");
            char* data = strtok(NULL, " ");
            
            string str_TypeOfData(typeOfData);
            string str_Data(data);
            
            if(str_TypeOfData == "NCOLS") //Checking out
                columns = atoi(data);
            else if(str_TypeOfData == "NROWS")
                rows = atoi(data);
            else if(str_TypeOfData == "XLLCENTER")
                xllCenter = atoi(data);
            else if(str_TypeOfData == "YLLCENTER")
                yllCenter = atoi(data);
            else if(str_TypeOfData == "CELLSIZE")
                cellSize = atoi(data);
            else if(str_TypeOfData == "NODATA_VALUE"){
                noData = atoi(data);
                gettingMetadata=false;
            }
        }
        
            //---Extracting height data---
            //Initializing float matrix
            height_Matrix = new float*[rows];
            for(int i = 0; i < rows; ++i)
                height_Matrix[i] = new float[columns];
            int i=0;
            int j=0;
            
        while (filee >> s_data && std::strncmp(s_data.c_str(), "\x1a", sizeof(char) * 4) != 0 && i < rows){
            
                height_Matrix[rows-1-i][j] = atof(s_data.c_str());
                if(++j == columns){
                    j=0; i++;
                }
            }
            
            
        }
        
    filee.close();
    zip_fclose(zf);

    }


// Vector{ Page Name, X coordinate, Y coordinate }

void Page::geographicalPlace(int umt_x, int umt_y, vector<int>*& upperVector){
    
    int coord_x, coord_y;
    
    int xReference= 0;
    //int yReference= -(rows-1);
    int yReference= 0;

    
    coord_x = abs(((xllCenter - umt_x)/cellSize) - xReference);
    coord_y = abs(((yllCenter - umt_y)/cellSize) - yReference);
    
    int array[] = {name, coord_x, coord_y};
    upperVector = new vector<int> (array, array + sizeof(array) / sizeof(int));
        
}

bool Page::checkBounds(int umt_x, int umt_y){
    
    bool inBounds=false;
    
    if (xllCenter  <= umt_x &&
        xllCenter + ( (columns-1 ) *cellSize) >= umt_x)
        if (yllCenter + ( (rows-1 ) *cellSize) >= umt_y &&
            yllCenter  <= umt_y)
            inBounds=true;
    
    return inBounds;
}

float Page::height (int row, int column){
    
    float data = -999;
    
    if(row < rows && column < columns && row >= 0 && column >= 0){
        data = height_Matrix[row][column];
    }

    return data;
}

float** Page::getMatrixPointer(){
    return height_Matrix;
}


int Page::getName(){
    return name;
}

int Page::getColumns(){
    return columns;
}

int Page::getRows(){
    return rows;
}

int Page::getXllCenter(){
    return xllCenter;
}

int Page::getYllCenter(){
    return yllCenter;
}

int Page::getCellSize(){
    return cellSize;
}

int Page::getNoData(){
    return noData;
}

void Page::coordToUmt(int row, int column, std::vector<int>*& solVector){
    
    int xutm=-1, yutm=-1;
    
    xutm= column*cellSize +xllCenter;
    //yutm= ((getRows()-1)-row)*cellSize + yllCenter;
    yutm= (row)*cellSize + yllCenter;
    
    int array[] = {xutm, yutm};
    solVector = new vector<int> (array, array + sizeof(array) / sizeof(int));
    
}







