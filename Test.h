#ifndef PEA1_TEST_H
#define PEA1_TEST_H

#include<iostream>
#include "time/Time.h"

using namespace std;

class Test {
private:
    string fileName="";
    int stopCondition=NULL;
    string neighbours="";
    int dT=NULL;
    int matrixSize{};
    int **matrix{};
    void pokazDane();
    void startTest();
    void readFromFile(string path);//todo na czas testów
    void deleteMatrix();//todo na czas testów
    void printMatrix();//todo na czas testów
    void startAnneling();
public:
    Test();
    ~Test();

};


#endif //PEA1_TEST_H
