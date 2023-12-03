#ifndef PEA1_TEST_H
#define PEA1_TEST_H

#include<iostream>
#include "time/Time.h"

using namespace std;

class Test {
private:
    int matrixSize{};
    int **matrix{};
    Time time;


    void deleteMatrix();
public:
    Test();
    ~Test();

    void readFromFile(string path);

    void generateData(int maxLen);

    void runAlgorithm();
    void printMatrix();
    void startTest();
    void startAutoTesting();
};


#endif //PEA1_TEST_H
