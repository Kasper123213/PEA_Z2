//
// Created by radom on 18.12.2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include "Ts.h"
#include "../time/Time.h"

using namespace std;
Ts::Ts(int **matrix, int matrixSize, int neighbourhood, int maxTime){
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->neighbourhood = neighbourhood;
    this->maxTime = maxTime;
    greedyAlg();
    iterationStopCondition = 1000000;

    for(int i=0;i<matrixSize;i++){
        for(int j=0;j<matrixSize;j++) {
            tabuList[i].push_back(-1);
        }
    }
}



Ts::~Ts(){}


void Ts::start() {
    Time* time = new Time();
    time->start();
    currentPath = bestPath;
    currentLen = bestLen;
    int iterationCounter = 0;
    do{
        while(true){

        }

        iterationCounter++;
    }while(iterationCounter<iterationStopCondition and time->getTime()<maxTime);

    delete time;
}



void Ts::greedyAlg(){
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
    cout<<"Greedy: najlepsza path ";//todo usunąć
    for(auto i:bestPath){
        cout<<i<<", ";
    }

    cout<<"len: "<<bestLen<<endl;
    //##############################################

}



int Ts::calcCost(vector<int> path){
    int cost = 0;
    for(int i=0; i<path.size();i++){
        cost+=matrix[i][i+1];
    }

    return cost;
}
