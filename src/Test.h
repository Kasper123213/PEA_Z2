#ifndef PEA1_TEST_H
#define PEA1_TEST_H

#include<iostream>
#include <vector>
#include "time/Time.h"

using namespace std;

class Test {
private:
    string fileName="";
    int maxTime=0;
    int neighbourhood=0;//1 insert 2 swap 3 invert
    double coolingFactor=0;
    int coolingType=0;
    int matrixSize=0;
    int **matrix{};
    int bestSolutionLen = INT_MAX;
    vector<int> bestSolutionPath;
    void pokazDane();
    void startTest();
    void deleteMatrix();
    void startAnneling();
    void startTabuSearch();
    bool readMatrix(string path);
    int checkPath(string path);
    void saveSolution();
    void testyDoSprawozdanie();
public:
    Test();
    ~Test();

};


#endif //PEA1_TEST_H
