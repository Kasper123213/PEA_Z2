#include <fstream>
#include "Test.h"
#include "algoritgms/BruteForce.h"

//konstruktor klasy
Test::Test() {
    //zmienna przechowująca decyzje użytkownika o uruchomieniu testu automatycznegp
    char choice;

    while (true) {
        cout << "Czy chciałbyś wykonać testowanie automatyczne algorytmów dla wielu wartości? t/n" << endl
             << "(Opcja do generowania sprawozdania). Aby wyjść naciśnij dowolną inną literę" <<endl;
        cin >> choice;
        cout<<endl;

        if (choice == 't' or choice == 'T') {
            startAutoTesting();
        } else if (choice == 'n' or choice == 'N') {
            startTest();
        } else {
            return;
        }

    }
}


//destruktor klasy Test służy do zwolnienia miejsca w pamięci zajmowanej przez macierz sąsiedstwa grafu
Test::~Test() {
    deleteMatrix();
}

//uruchomienie testów algorytmu
void Test::startTest(){
    while (true){

        cout << "Co chcesz zrobić?\n" <<
             "1.Wczytanie danych z pliku\n" <<
             "2.Wygenerowanie danych losowych\n" <<
             "3.Wyświetlenie ostatnio wczytanych lub wygenerowanych danych\n" <<
             "4.Uruchomienie algorytmu przeglądu zupełnego dla ostatnio wczytanych danych\n"
             <<

             "0.Wyjście" << endl;
        //wybór czynności przez użytkownika
        int choice;
        cin >> choice;
        cout<<endl;

        string path;
        switch (choice) {
            //wczytanie macierzy sąsiedstwa z pliku o podanej ścieżce
            case 1:
                deleteMatrix();
                cout << "Podaj ścieżkę absolutną do pliku txt"<<endl;
                cin >> path;
                cout<<endl;
                readFromFile(path);
                break;
            //wygenerowanie macierzy sąsiedstwa o podanej liczbie wierzchołków i maksymalnej długości krawędzi
            case 2:
                deleteMatrix();

                cout << "Podaj liczbe wierzchołków" << endl;
                cin >> matrixSize;
                cout<<endl;
                cout << "Podaj maksymalną długość ścieżek" << endl;
                int maxLen;
                cin >> maxLen;
                cout<<endl;
                generateData(maxLen);
                break;
            //wyświetlenie macierzy sąsiedstwa
            case 3:
                printMatrix();
                break;
            //uruchomienie wybranego algorytmu
            case 4:
                if(matrixSize!=0) {
                    runAlgorithm();
                }else{
                    cout<<"Najpierw wczytaj graf"<<endl<<endl;
                }
                break;
            default:
                return;
        }
    }
}

//uruchomienie testów automatycznych
void Test::startAutoTesting(){
    int maxLen;
    int instances;
    ofstream excelFile("dane.csv");

    if (!excelFile.is_open()) {
        cout << "Nie można otworzyć pliku Excela." << endl;
        return;
    }

    cout << "Testy będą się zaczynały od grafu o 2 wierzchołkach do grafu o rozmiarze podanym przez Ciebie." << endl
    << "Jaki jest maksymalny rozmiar?"<< endl;
    cin >> maxLen;
    cout<<endl;
    cout << "Po ile grafów ma być wygenerowanych dla każdego kolejnego rozmiaru?"<<endl;
    cin >> instances;
    cout<<endl;

    //dla każdej ilości wierzchołków uruchamiamy test
    for(int len=2; len<=maxLen;len++){
        cout << len<<endl;
        matrixSize = len;
        excelFile << len << ";;";
        //dla każdego rozmiaru generujemy podaną ilość instancji
        for(int i=0; i<instances;i++){
            cout<< "\t"<<i<<endl;
            generateData(100);

            BruteForce* bruteForce = new BruteForce(matrix, matrixSize);

            time.start();
            bruteForce->start();
            time.stop();

            excelFile << time.getTime() << ";";

        }
        excelFile<<endl;

    }

    excelFile.close();

}


void Test::readFromFile(string path)  {
    // Otwórz plik
    ifstream file(path);

    // Sprawdź, czy udało się otworzyć plik
    if (!file.is_open()) {
        cout << "Nie można otworzyc." << endl;
    }

    // Zmienna przechowująca wartość wczytaną z pliku
    int wartoscZPliku;

    // Wyczyść flagi błędów i pozycję odczytu pliku
    file.clear();
    file.seekg(0, ios::beg);

    int i = 0;
    int j = 0;
    while (file >> wartoscZPliku) {

        if (i == 0) {
            //tworzenie tabeli ścieżek
            matrixSize = wartoscZPliku;
            matrix = new int *[matrixSize]; // Deklaracja tablicy wskaźników na wskaźniki do int

            for (int i = 0; i < matrixSize; ++i) {
                matrix[i] = new int[matrixSize]; // Alokacja pamięci dla każdego wiersza
            }

        } else {
            if (i > matrixSize) {
                j += 1;
                i = 1;
            }


            matrix[j][i - 1] = wartoscZPliku;

        }

        i++;
    }
    file.close();

}

//generowanie tablicy sąsiedstwa
void Test::generateData(int maxLen) {
    matrix = new int *[matrixSize]; // Deklaracja tablicy wskaźników na wskaźniki do int

    for (int i = 0; i < matrixSize; ++i) {
        matrix[i] = new int[matrixSize]; // Alokacja pamięci dla każdego wiersza
    }


    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (i == j) {   //wierzchołki nie mają krawędzi z samym sobą dlatego gdy indeksy są równe do tablicy wpisujemy -1
                matrix[i][j] = -1;
                continue;
            }
            matrix[i][j] = rand() % maxLen + 1;


        }

    }

}

//uruchamianie algorytmu oraz pomiar jego czasu wykonania
void Test::runAlgorithm() {

    BruteForce* bruteForce = new BruteForce(matrix, matrixSize);

    time.start();
    bruteForce->start();
    time.stop();

    cout<<"#############################################################################################"<<endl;
    cout << "\nNajkrótsza ścieżka: ";
    int *minPath = bruteForce->getMinPath();
    for (int i = 0; i <= matrixSize; i++) {
        cout << minPath[i] << ", ";
    }
    cout << " ma długość: " << bruteForce->getMinLenght() << endl;


    cout << "Czas wykonania algorytmu to: " << time.getTime()/1000000 << "ms\n" << endl;
    cout<<"#############################################################################################"<<endl<<endl;
    delete bruteForce;

}

//wyświetlanie macierzy sąsiedstwa
void Test::printMatrix() {
    if(matrixSize==0){
        cout<<"Tablica jest pusta"<<endl<<endl;
        return;
    }
    cout << "Tabela sąsiedstwa z wagami:" << endl;
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "Rozmiar grafu: " << matrixSize << endl<<endl;
}

//usuwanie macierzy sąsiedstwa z pamięci
void  Test::deleteMatrix() {
    if(matrixSize!=0 ) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] matrix[i];
        }

        matrixSize = 0;

    }
    delete[] matrix;
}