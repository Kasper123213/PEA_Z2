//
// Created by radom on 18.12.2023.
//

#ifndef PEA2_TS_H
#define PEA2_TS_H
#include <iostream>
#include <vector>

using namespace std;

class Ts {
private:
    int** matrix;
    int matrixSize;
    int neighbourhood;
    int maxTime;
    int tabuLen;
    int bestLen;
    int iterationStopCondition;
    int currentLen; //stan w ktorym teraz przebywamy
    int testLen;//stan do sprawdzenia
    vector<int> bestPath;
    vector<int> currentPath;
    vector<int> testPath;
    vector<vector<int>> tabuList;

    void greedyAlg();
    int calcCost(vector<int> path);
public:
    void start();
    Ts(int **matrix, int matrixSize, int neighbourhood, int maxTime);
    ~Ts();
};


#endif //PEA2_TS_H
