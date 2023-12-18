#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <random>
#include <fstream>
#include "Test.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);    //ustawianie polskich znaków

    srand(time(NULL));

    cout << "Autor: Kasper Radom 264023" <<endl<< endl;

    //inicjowanie pamięci na włączenie testów
    Test *test = new Test;

    //zwolnienie pamięci po objekcie test i wywołanie jego destruktora
    delete test;

}






