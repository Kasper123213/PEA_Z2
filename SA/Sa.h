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
    vector<int> bestPath;
    int bestLen;
    void greedyAlg();
public:
    void start();
    Sa(int **matrix, int matrixSize);
    ~Sa();
};


#endif //PEA2_SA_H
