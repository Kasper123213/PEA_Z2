#ifndef PEA1_TEST_H
#define PEA1_TEST_H

#include<iostream>
#include <vector>
#include "time/Time.h"

using namespace std;

class Test {
private:
    string fileName="";
    int maxTime=-1;
    string neighbours="";
    double coolingFactor=-1;
    int matrixSize{};
    int **matrix{};
    void pokazDane();
    void startTest();
    void readFromFile(string path);//todo na czas testów
    void deleteMatrix();//todo na czas testów
    void printMatrix();//todo na czas testów
    void startAnneling();
    void readMatrix(string path);
    void checkPath(string solution);
public:
    Test();
    ~Test();

};


#endif //PEA1_TEST_H
