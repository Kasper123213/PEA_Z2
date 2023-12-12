//
// Created by radom on 11.12.2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "Sa.h"

Sa::Sa(int **matrix, int matrixSize) {
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    greedyAlg();
};



Sa::~Sa(){
};


void Sa::start(){

    vector<int> currentPath = bestPath;
    int currentLen = bestLen;
    cout<<"DUuuuuuuuuuuuuuuupa"<<endl;

}




void Sa::greedyAlg(){
    int currentCity = 0;
    bestPath.push_back(currentCity);
    bestLen=0;
    int nextCity;
    int minPath = INT_MAX;

    while(size(bestPath)!=matrixSize){
        minPath=INT_MAX;
        for(int i=0;i<matrixSize;i++){
            auto it = find(bestPath.begin(), bestPath.end(), i);

            if(it!=bestPath.end()) continue;
            else{
                if(matrix[currentCity][i]<minPath){
                    nextCity = i;
                    minPath = matrix[currentCity][nextCity];
                }
            }
        }
        bestPath.push_back(nextCity);
        bestLen+=minPath;
        currentCity = nextCity;
    }
    bestPath.push_back(0);
    bestLen+=matrix[currentCity][0];

    //##############################################
    cout<<"Best Path ";//todo usunąć
    for(auto i:bestPath){
        cout<<i<<", ";
    }

    cout<<"len: "<<bestLen<<endl;
    //##############################################

}
