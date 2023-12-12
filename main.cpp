#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include "Test.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);    //ustawianie polskich znaków

    srand(time(NULL));

    cout << "Autor: Kasper Radom 264023" << endl <<"Algorytm przeglądu zupełnego"<<endl<< endl;

    //inicjowanie pamięci na włączenie testów
    Test *test = new Test;

    //zwolnienie pamięci po objekcie test i wywołanie jego destruktora
    delete test;

//    vector<int> v = {0,1};
//    vector<int> a = v;
//
//    v.push_back(9);
//    a.push_back(90);
//
//
//    for(auto i:v){
//        cout<<i<<", ";
//    }
//    cout<<endl;
//    for(auto i:a){
//        cout<<i<<", ";
//    }
}