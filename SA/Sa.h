//
// Created by radom on 11.12.2023.
//

#ifndef PEA2_SA_H
#define PEA2_SA_H
#include <iostream>
#include <vector>

using namespace std;
class Sa {
private:
    int** matrix;
    int matrixSize;
    int maxTime;
    int bestLen;
    int currentLen; //stan w ktorym teraz przebywamy
    int testLen;//stan do sprawdzenia
    int beginningTemperature;
    double coolingFactor;
    vector<int> bestPath;
    vector<int> currentPath;
    vector<int> testPath;
    void greedyAlg();
    int calcBeginningTemperature(int bestLen, double N);
    void calcLen();
    double calcNewTemperature(double currentTemperature, int eraNumber);
    double calcGeometricTemp(double T);
    double calcLogaritmicTemp(double T, int eraNumber);
    double calcExpotentialTemp(double T, int eraNumber);
    pair<int, int> generateSwapPoints();
    void swapPoints(pair<int, int> swappedPoints);
    int calcEra(int size, int alpha);
public:
    void start();
    Sa(int **matrix, int matrixSize, double coolingFactor, int maxTime);
    ~Sa();
};


#endif //PEA2_SA_H
