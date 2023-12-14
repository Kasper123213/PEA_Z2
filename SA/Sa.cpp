
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <random>
#include "Sa.h"
#include "../time/Time.h"

Sa::Sa(int **matrix, int matrixSize, double coolingFactor, int maxTime) {//todo dodac reszte par tpu czas
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->coolingFactor = coolingFactor;
    this->maxTime = maxTime;
    greedyAlg();
    beginningTemperature = calcBeginningTemperature(bestLen, 0.01);
};



Sa::~Sa(){
};


void Sa::start(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);	// uniform distribution between 0 and 1


    currentPath = bestPath;
    testPath = bestPath;

    currentLen = bestLen;
    testLen = bestLen;

    double currentTemperature = beginningTemperature;
    int eraLen = calcEra(matrixSize, 3);//todo sprawdzic alpha
    int eraNumber = 0;
    pair<int, int> swappedPoints;
    Time* time = new Time();
    time->start();
    do{
        for(int era=0;era<eraLen;era++){

//            cout<<"##################################"<<endl;
            testPath = currentPath;
            swappedPoints = generateSwapPoints();
            swapPoints(swappedPoints);
            calcLen();

//            for(auto x: testPath){
//                cout<<x<<", ";
//            }
//            cout<<" dlugosc: "<<testLen<<endl;

            int delta = testLen - currentLen;

            if(delta<=0) {
                currentLen = testLen;
                currentPath = testPath;
                if(currentLen<=bestLen){//todo usunąćte niedozwolone triki chyba
                    bestLen = currentLen;
                    bestPath = currentPath;
                }
            }else{
                double randomValue = dis(gen);
                if (randomValue < exp(-delta / currentTemperature)){
//                    cout<<"Zmieniammm"<<endl;
                    currentLen = testLen;
                    currentPath = testPath;
                }
            }
        }
        for(auto x: currentPath){
            cout<<x<<", ";
        }
        cout<<" dlugosc: "<<currentLen<< endl<<"temperatura: "<< currentTemperature<<endl;
        cout<<"##########################"<<endl;


        eraNumber++;
        currentTemperature = calcNewTemperature(currentTemperature, eraNumber);
    }while(currentTemperature>=pow(10,-10) and time->getTime()<=maxTime);//todo ptzemyslec czy pow(10,-15)

    cout<<"Juz po wyrzazeniu"<<endl;
    cout<<"Czas to: "<<time->getTime()<<" a oczekiwany to : "<<maxTime<<endl;//todo to do  przeniesienia
    cout<<"Temperatura to: "<<currentTemperature<<endl;//todo to do  przeniesienia
    cout<<"Rozwiązanie po wyżażeniu to : ";
    for(int i:bestPath){
        cout<<i<<", ";
    }
    cout<<endl<<"dlugosc to: "<<bestLen;

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
    cout<<"Greedy: najlepsza path ";//todo usunąć
    for(auto i:bestPath){
        cout<<i<<", ";
    }

    cout<<"len: "<<bestLen<<endl;
    //##############################################

}


int Sa::calcBeginningTemperature(int bestLen, double N){
    return bestLen*N;
}


double Sa::calcNewTemperature(double T, int eraNumber){
    return calcGeometricTemp(T);
//    return calcLogaritmicTemp(T, eraNumber);
//    return calcExpotentialTemp(T, eraNumber);
}

double Sa::calcGeometricTemp(double T){
    double alfa = 0.99; //todo  zostawic 0.999
    return T*alfa*coolingFactor;
}

double Sa::calcLogaritmicTemp(double T, int eraNumber){
    return T/(1+coolingFactor*log(eraNumber+1));
}

double Sa::calcExpotentialTemp(double T, int eraNumber){
    return pow(coolingFactor, eraNumber)*T;
}

pair<int, int> Sa::generateSwapPoints(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);	// uniform distribution between 0 and 1

    int indexI = (int)(dis(gen) * matrixSize);
    int indexJ = (int)(dis(gen) * matrixSize);

    if(indexJ!=indexI) {
        pair<int, int> swapped(indexI, indexJ);
        return swapped;
    }else{
        return generateSwapPoints();
    }

}

void Sa::swapPoints(pair<int, int> swappedPoints){

    int i = swappedPoints.first;
    int j = swappedPoints.second;

    swap(testPath[i], testPath[j]);
    if(i == 0 or j==0){
        testPath[matrixSize] = testPath[0];
    }


}

void Sa::calcLen(){
    testLen = 0;
    for(int i = 0; i< matrixSize; i++){
        testLen += matrix[testPath[i]][testPath[i+1]];
    }

}



int Sa::calcEra(int size, int alpha){
    return size*alpha;
}