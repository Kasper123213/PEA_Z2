
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <random>
#include "Sa.h"
#include "../time/Time.h"

Sa::Sa(int **matrix, int matrixSize, double coolingFactor, int maxTime, int coolingType) {//todo dodac reszte par tpu czas
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->coolingFactor = coolingFactor;
    this->maxTime = maxTime;
    this->coolingType = coolingType;
}

Sa::~Sa(){
}

void Sa::start(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    pair<int, int> swappedPoints;
    //na początku algorytmem chciwym wybierane jest początkowe rozwiązanie
    greedyAlg();    //początkowa temperatura jest iloczynem kosztu najlepszego znalezionego rozwiązania oraz pewnego wspolczynnika
    beginningTemperature = calcBeginTemperature();
//    cout<<"T początkowa "<<beginningTemperature<<endl;
    //długość epoki również jest zależna od wielkości problemu
    eraLen = matrixSize * (matrixSize -1) / 2;
    //inicjalizujemy generator liczb losowych w przedziale 0-1

    currentPath = bestPath;
    testPath = bestPath;

    currentLen = bestLen;
    testLen = bestLen;

    //na potrzeby dokladnego zbadania problemu zainicjalizowane zmienne opisujące 3 rozwiązania.
    //najlepsze znalezione rozwiązanie - best*,
    //rozwiązanie którego sąsiadów badamy - current* oraz rozwiązanie którego akceptacje rozważamy test*
    double currentTemperature = beginningTemperature;
    int eraNumber = 0;

    //inicjalizujemy stoper oraz zaczynamy odliczać na nim czas
    Time* time = new Time();
    time->start();
    timeOfBestSolution = time->getTime();
    do{
        //w każdej epoce jest jednakowa ilość iteracji przy jednakowej temperaturze
        for(int era=0;era<eraLen;era++){

            testPath = currentPath;
            //losujemy punkty których zamiana określi nam kolejnego sąsiada
            swappedPoints = generateSwapPoints();
            //przechodzimy do owego sąsiada
            swapPoints(swappedPoints);
            //oraz liczymy jego koszt
            calcLen();

            //obliczamy różnice kosztów sąsiada i obecnego rozwiązania
            int delta = testLen - currentLen;

            //jeśli sąsiad ma mniejszy koszt, przechodzimy do niego
            if(delta<=0) {
                currentLen = testLen;
                currentPath = testPath;
                //sprawdzamy czy obecne rozwiązanie nie jest najlepszym dotychczas znalezionym
                if(currentLen<bestLen){
                    bestLen = currentLen;
                    bestPath = currentPath;
                    timeOfBestSolution = time->getTime();
                }
            }else{
                //jeśli sąsiad ma siększy koszt niż nasze rozwiązanie i tak ma szansę być nowym rozwiązaniem
                // Sprawdzamy to przy pomocy generatora liczb pseudolosowych
                double randomValue = dis(gen);
                //ze wzoru sprawdzamy czy koszt (energia) mieści się w akceptowanych wylosowanych przez nas granicach
                if (randomValue < exp(-delta / currentTemperature)){
                    currentLen = testLen;
                    currentPath = testPath;
                }
            }
        }
        //inkrementujemy nimer epoki oraz wyliczamy nową temperaturę
        eraNumber++;
        currentTemperature = calcNewTemperature(currentTemperature, eraNumber);
        //Jeśli temperatura przekroczyła pewien poziom zatrzymujemy algorytm gdyż szanse, że przyniesie nam
        // lepsze rozwiązanie zmniejszają sie wraz z temperaturą
        //drógim warunkiem jest przekroczenie preferowanego przez nas czasu działania algorytmu
    }while(currentTemperature>=0.01 and time->getTime()<=maxTime);//todo 10,-9
        //dealokujemy nie potrzebny nam już miernik czasu
    delete time;
}


//algorytm chciwy
void Sa::greedyAlg(){
    int currentCity = 0;
    bestPath.push_back(currentCity);
    bestLen=0;
    int nextCity;
    int minLen = INT_MAX;
    //dopuki ścieżka niema zawiera wszywtkich wierzchołków grafu, pętla trwa
    while(size(bestPath)!=matrixSize){
        minLen=INT_MAX;
        //przeglądamy wszystkie krawędzie wychodzące z pierwszego wierzchołka
        for(int i=0;i<matrixSize;i++){
            //sprawdzamy czy dany wierzchołek był już odwiedzony
            auto it = find(bestPath.begin(), bestPath.end(), i);
            //jeśli był, idziemy do kolejnego
            if(it!=bestPath.end()) continue;
            //jeśli niema go na liście sprawdzamy czy krawędź do niego jest mniejsza niż najmniejsza znalezniona
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
    //na końcu dodajemy powrót do początkowego wierzchołka aby powstał cykl
    bestPath.push_back(0);
    //zwiększamy również odpowiednio długość znalezionego cyklu
    bestLen+=matrix[currentCity][0];
}




//metoda obliczająca nową temperaturę w zależności od wybranego typu chłlodzenia
double Sa::calcNewTemperature(double T, int eraNumber){
    switch (coolingType) {
        case 1:
            return calcGeometricTemp(T);
        case 2:
            return calcLogaritmicTemp(T, eraNumber);
        case 3:
            return calcExpotentialTemp(T, eraNumber);

    }
    return 0;
}
//chłodzenie geometryczne
double Sa::calcGeometricTemp(double T){
    return T*coolingFactor; //*0.995 dla 5 minut
}
//chłodzenie logarytmiczne
double Sa::calcLogaritmicTemp(double T, int eraNumber){
    return T/(1+coolingFactor*log(eraNumber+1));
}
//chłodzenie wykładnicze
double Sa::calcExpotentialTemp(double T, int eraNumber){
    return pow(coolingFactor, eraNumber)*T;
}

//funkcja generuje dwa losowe indeksy do zamienienia wierzchołków miejscami w cyklu
pair<int, int> Sa::generateSwapPoints(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);  //losowanie miedzy 0 a 1

    int indexI = (int)(dis(gen) * matrixSize);
    int indexJ = (int)(dis(gen) * matrixSize);

//    jeśli wylosują się punkty o tej samej wartosci powtarzamy proces
    if(indexJ!=indexI) {
        pair<int, int> swapped(indexI, indexJ);
        return swapped;
    }else{
        return generateSwapPoints();
    }
}

//funkcja zamienia punkty miejscami
void Sa::swapPoints(pair<int, int> swappedPoints){

    int i = swappedPoints.first;
    int j = swappedPoints.second;

    swap(testPath[i], testPath[j]);
    //jeśli któryś z pynktów ma indeks 0
    // musimy zwrócić uwage aby również ostatni wierzchołek cyklu zamienić
    if(i == 0 or j==0){
        testPath[matrixSize] = testPath[0];
    }
}

//wyliczamy koszt danego cyklu dodając wagi krawędzi
void Sa::calcLen(){
    testLen = 0;
    for(int i = 0; i< matrixSize; i++){
        testLen += matrix[testPath[i]][testPath[i+1]];
    }
}


long Sa::calcBeginTemperature() {
    testPath = bestPath;
    testLen = bestLen;
    long sum = testLen;

    for(int i=0; i<99; i++){
        swapPoints(generateSwapPoints());
        calcLen();
        sum += testLen;
    }
    sum *= 0.015;
    return sum;
}

