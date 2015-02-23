//
//  GeographicalMap.cpp
//  MinimumGeographicCost
//
//  Created by carlos on 9/17/14.
//  Copyright (c) 2014 fdzjuancarlos. All rights reserved.
//

#include "GeographicalMap.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
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
#include "Page.h"
#include "hdf5.h"
#include "zip.h"
//#include <zip.h>

using namespace std;

GeographicalMap::GeographicalMap(vector<int> maps){
    
    avaiableMaps.swap(maps);
    
    v_pages = *new vector<Page*>;
    char final_path[200];
    ifstream readFile;
    
    /*HDF5 Variables*/
    hid_t       file, dataset, attribute_id, space;         /* file and dataset handles */
    hid_t       datatype, dataspace, aid1, dset;   /* handles */
    hsize_t     dimsf[2];             /* dataset dimensions */
    hsize_t     adim = { 7 };   /*attribute matrix size*/
    herr_t      status, ret;
    int         attr_data[7];
    int rows, columns;
    bool        writeNewData=false;
    int ndims;
    float** rdata;
    int rattribute[6];
    Page* actualPage;
    char name[7];

    
    hid_t error_stack;
    H5Eset_auto (NULL, NULL, NULL);
    
    //For every map we try to get the data and meta-data
    for(vector<int>::iterator it = avaiableMaps.begin(); it != avaiableMaps.end() && !writeNewData;
        ++it) {
    
    
    /*---------------------READ PROCCESS------------------*/
    

    
    

    

    file = H5Fopen ("myData.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    sprintf(name,"Page%d", *it);
    dset = H5Dopen (file, name,H5P_DEFAULT);
    attribute_id = H5Aopen_name(dset,"PageAttributes");
        
    //Detecting if we have that page in the HDF5 file ####################################
        
    if((status  = H5Aread(attribute_id, H5T_NATIVE_INT, &rattribute)) == 0){

    hsize_t dims[2] = { static_cast<hsize_t>(rattribute[1]), static_cast<hsize_t>(rattribute[2])};
    std::cout << "Page " <<rattribute[0] << " loaded." << endl;
    
    space = H5Dget_space (dset);
    ndims = H5Sget_simple_extent_dims (space, dims, NULL);
    rdata = (float **) malloc (dims[0] * sizeof (float *));
    rdata[0] = (float *) malloc (dims[0] * dims[1] * sizeof (float));
        
    //Reading Matrix
    for (int i=1; i<dims[0]; i++)
        rdata[i] = rdata[0] + i * dims[1];

    //Reading meta-data
    status = H5Dread (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                      rdata[0]);
        
        
    v_pages.push_back(new Page(rattribute, rdata));
    
    /*
     * Close and release resources.
     */
    free (rdata[0]);
    free (rdata);
    status = H5Aclose (attribute_id);
    status = H5Dclose (dset);
    status = H5Sclose (space);
    status = H5Fclose (file);
        
    }else{//Not valid data, writing a new HDF5 File
        std::cout << "No valid data on HDF5 File, writing new file" << endl;
        writeNewData=true;
        v_pages = *new vector<Page*>; //Delete old taken Pages
        status = H5Aclose (attribute_id);
        status = H5Dclose (dset);
        status = H5Sclose (space);
        status = H5Fclose (file);
        


        //*---------Taking data from ZIP files----------*
        for(vector<int>::iterator it = avaiableMaps.begin(); it != avaiableMaps.end(); ++it) {
            
            //personal userpath /Users/Lion/Dropbox/Universidad/3 Curso/Sistemas Inteligentes/Trabajo/source/MinimumGeographicCost/resources
            
            sprintf(final_path,"./MDT25-%0*d.zip",4 , *it);
            v_pages.push_back(new Page(final_path, *it));
            
        }
        
        /*-------Writing Process--------*/
        
        file = H5Fcreate("myData.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

        for(vector<int>::iterator it = avaiableMaps.begin(); it != avaiableMaps.end(); ++it) {
            actualPage = getPage(*it);
            rows = actualPage->getRows();
            columns = actualPage->getColumns();
            float data[rows][columns];
            
            //Writing Matrix
            for (int i=0; i<rows; i++){
                for( int j=0; j<columns; j++)
                    data[i][j] = getPage(*it)->getMatrixPointer()[i][j];
            }
            
            //Writing meta-data
            attr_data[0]= actualPage->getName();
            attr_data[1]= actualPage->getRows();
            attr_data[2]= actualPage->getColumns();
            attr_data[3]= actualPage->getXllCenter();
            attr_data[4]= actualPage->getYllCenter();
            attr_data[5]= actualPage->getCellSize();
            attr_data[6]= actualPage->getNoData();
            
            
            dimsf[0] = actualPage->getRows();
            dimsf[1] = actualPage->getColumns();
            dataspace = H5Screate_simple(2, dimsf, NULL);
            
            datatype = H5Tcopy(H5T_NATIVE_FLOAT);
            status = H5Tset_order(datatype, H5T_ORDER_LE);
            
            sprintf(name,"Page%d", *it);
            
            dataset = H5Dcreate(file, name, datatype, dataspace,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
            status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL,
                              H5P_DEFAULT, data);

            aid1 = H5Screate(H5S_SIMPLE);
            ret  = H5Sset_extent_simple(aid1, 1, &adim, NULL);
            attribute_id = H5Acreate2 (dataset, "PageAttributes", H5T_STD_I32BE, aid1,
                                       H5P_DEFAULT, H5P_DEFAULT);
            
            status = H5Awrite(attribute_id, H5T_NATIVE_INT, attr_data);
            status = H5Aclose(attribute_id);
            
            
            //Closing
            H5Sclose(dataspace);
            H5Tclose(datatype);
            H5Dclose(dataset);
            
            
        }
        H5Fclose(file);


    }
}
    
}



void GeographicalMap::geographicalPlace(int umt_x, int umt_y, std::vector<int>*& upperVector){
    
    bool pageNotFound=true;
    
    for(vector<Page*>::iterator it = v_pages.begin(); it != v_pages.end() && pageNotFound; ++it) {
        
        if((*it)->checkBounds(umt_x, umt_y)){
            pageNotFound=false;
            (*it)->geographicalPlace(umt_x, umt_y, upperVector);
        }

    }
    
    if (pageNotFound){ //Error control
        upperVector = new std::vector<int>(3, -1);
    }
    
    
    
}


float GeographicalMap::height(int pageNumber,int coord_x, int coord_y){
    
    float height=-999;
    
    for(vector<Page*>::iterator it = v_pages.begin(); it != v_pages.end(); ++it) {
        
        if((*it)->getName() == pageNumber)
            height= (*it)->height(coord_y, coord_x);
        
    }
    
    
    return height;
}

Page* GeographicalMap::getPage(int pageNumber){
    
    Page* finalPage=NULL;
    
    for(vector<Page*>::iterator it = v_pages.begin(); it != v_pages.end(); ++it) {
        
        if((*it)->getName() == pageNumber)
            finalPage= *it;
        
    }
    
    
    return finalPage;
    
}

void GeographicalMap::coordToUmt(int page, int row, int column, std::vector<int>*& solVector){
    
    getPage(page)->coordToUmt(column,row, solVector);
    
}


