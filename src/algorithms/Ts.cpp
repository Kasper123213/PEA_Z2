//
// Created by radom on 18.12.2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Ts.h"
#include "../time/Time.h"

using namespace std;
Ts::Ts(int **matrix, int matrixSize, int neighbourhood, int maxTime){
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->neighbourhood = neighbourhood;
    this->maxTime = maxTime;
    tabuLen = matrixSize * 1.2;
    iterationStopCondition = matrixSize * 20;

    for(int i=0;i<matrixSize;i++){
        tabuList.push_back({});
        for(int j=0;j<matrixSize;j++) {
            tabuList[i].push_back(0);
        }
    }
}



Ts::~Ts() {
}


void Ts::startSearching() {
    greedyAlg();
    vector<pair<int,int>> const changes = getChanges();
    time = new Time();
    time->start();
    timeOfBestSolution = time->getTime();
    currentPath = bestPath;
    currentLen = bestLen;
    pair<int, int> bestPair;
    int iterationCounter = 0;
    do{
        //wybieranie sąsiadów
        bestPair = bestChange(changes);

        tabuList[bestPair.first][bestPair.second] = tabuLen;
        iterationCounter++;

        //zmniejszanie listy tabu
        for(auto & v : tabuList){
            for(int j=0;j<tabuList.size();j++){
                if(v[j]>0){
                    v[j]-=1;
                }
            }
        }


        //mechanizm mający zapewnić dywersyfikacje
        if(iterationCounter>iterationStopCondition){
            iterationCounter = 0;
            clearTabu();
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0, changes.size()-1);
            pair<int, int> randomchoice;

            for(int i=0;i<matrixSize/2;i++) {
                randomchoice = changes[dis(gen)];
                doChange(randomchoice.first, randomchoice.second);
            }
        }
    }while(time->getTime()<maxTime);

    delete time;
}



void Ts::greedyAlg(){
    int currentCity = 0;
    bestPath.push_back(currentCity);
    bestLen=0;
    int nextCity;
    int minLen = INT_MAX;

    while(size(bestPath)!=matrixSize){
        minLen=INT_MAX;
        for(int i=0;i<matrixSize;i++){
            auto it = find(bestPath.begin(), bestPath.end(), i);

            if(it!=bestPath.end()) continue;
            else{
                if(matrix[currentCity][i]<minLen){
                    nextCity = i;
                    minLen = matrix[currentCity][nextCity];
                }
            }
        }
        bestPath.push_back(nextCity);
        bestLen+=minLen;
        currentCity = nextCity;
    }
    bestPath.push_back(0);
    bestLen+=matrix[currentCity][0];

}


vector<pair<int, int>> Ts::getChanges(){
    vector<pair<int, int>> changes;

    for(int i = 0; i< matrixSize; i++){
        int j;
        //ten warunek sprawie, że swap oraz invery ma mniejszą liste sąsiadów ponieważ w tych
        //wersjach generowania sąsiedstwa, doSwap(2,5) oraz doSwap(5,2) jest jednoznaczne.
        //w insert natomiast już nie
        if(neighbourhood==1) j=0;
        else j=i+1;

        for(; j< matrixSize; j++){
            if(i==j)continue;
            //dzieki temu zabiegowi nie mamy powtórzeń na przykład (1,2) oraz (2,1). W insert jest to to samo przykształcenie
            else if(neighbourhood == 1 and i-1==j) continue;
            pair<int, int> pair = {i,j};
            changes.push_back(pair);
        }
    }

    return changes;
}


pair<int, int> Ts::bestChange(vector<pair<int, int>> changes){
    vector<int> bestNeighbour;
    pair<int, int> bestPair;
    int neighbourLen = INT_MAX;
    bool found = false;
    for(pair<int, int> change:changes){
        //sprawdzanie czy sąsiead jest na liście tabu
        if(tabuList[change.first][change.second]>0) continue;

        testPath = currentPath;
        doChange(change.first, change.second);
        calcCost();

        if(testLen<neighbourLen){
            bestPair = change;
            bestNeighbour = testPath;
            neighbourLen = testLen;
            found = true;
        }
    }


    if(found){
        currentPath = bestNeighbour;
        currentLen = neighbourLen;
        if(currentLen<bestLen){
            bestPath = currentPath;
            bestLen = currentLen;
            timeOfBestSolution = time->getTime();

        }
    }else{ //sprawdzanie kryterium aspiracji (najdłużej na liście tabu)
        bestPair = {-1,-1};
        int min=INT_MAX;
        for(int i=0;i<tabuList.size();i++){
            for(int j=0;j<tabuList.size();j++){
                if(tabuList[i][j]==0)continue;
                if(tabuList[i][j]<min){
                    min = tabuList[i][j];
                    bestPair.first = i;
                    bestPair.second = j;
                }
            }
        }

        doChange(bestPair.first, bestPair.second);
        calcCost();
        currentPath = testPath;
        currentLen = testLen;
        if(currentLen<bestLen) {
            bestPath = currentPath;
            bestLen = currentLen;
        }
    }
    return bestPair;

}




void Ts::doChange(int indexFirst, int indexSecond){
    switch(neighbourhood) {
        case 1:
            doInsert(indexFirst, indexSecond);
            break;
        case 2:
            doSwap(indexFirst, indexSecond);
            break;
        case 3:
            doInvert(indexFirst, indexSecond);
            break;
    }
}
void Ts::doInsert(int where, int from){
    vector<int> newPath;
    for(int i = 0;i<testPath.size()-1;i++){
        if(i==where) newPath.push_back(testPath[from]);
        else if(i == from)continue;
        newPath.push_back(testPath[i]);
    }
    newPath.push_back(newPath[0]);
    testPath = newPath;
}


void Ts::doSwap(int indexFirst, int indexSecond){
    swap(testPath[indexFirst], testPath[indexSecond]);
    testPath[testPath.size()-1]=testPath[0];
}


void Ts::doInvert(int indexSmaller, int indexBigger){
    vector<int> inverted;
    vector<int> newPath;
    for(int i = indexBigger; i >=indexSmaller; i--){
        inverted.push_back(testPath[i]);
    }
    for(int i=0; i<testPath.size()-1; i++){
        if(i<=indexBigger and i>=indexSmaller) {
            newPath.push_back(inverted[i-indexSmaller]);
        }else{
            newPath.push_back(testPath[i]);
        }
    }
    newPath.push_back(newPath[0]);
    testPath = newPath;
}

//liczenie kosztu ścieżki
void Ts::calcCost(){
    testLen = 0;
    for(int i=0; i<testPath.size()-1;i++){
        testLen+=matrix[testPath[i]][testPath[i+1]];
    }

}
//zerowanie listy tabu
void Ts::clearTabu(){
    for(vector<int> v: tabuList){
        v.clear();
    }
}














