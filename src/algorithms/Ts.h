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
    int neighbourhood;//1 insert 2 swap 3 invert
    int maxTime;
    int tabuLen;
    int iterationStopCondition;
    int testLen;//stan do sprawdzenia
    vector<int> testPath;
    vector<vector<int>> tabuList;

    void greedyAlg();
    vector<pair<int, int>> getChanges();
    pair<int, int> bestChange(vector<pair<int, int>> changes);
    void doChange(int indexFirst, int indexSecond);
    void doInsert(int where, int from);
    void doSwap(int indexFirst, int indexSecond);
    void doInvert(int indexSmaller, int indexBigger);
    void calcCost();
    void clearTabu();
public:
    void startSearching();
    Ts(int **matrix, int matrixSize, int neighbourhood, int maxTime);
    ~Ts();

    int currentLen; //stan w ktorym teraz przebywamy
    int bestLen;
    vector<int> bestPath;
    vector<int> currentPath;

};


#endif //PEA2_TS_H
