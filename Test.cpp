#include <fstream>
#include "Test.h"
#include "SA/Sa.h"

//konstruktor klasy
Test::Test() {
    //zmienna przechowująca decyzje użytkownika o uruchomieniu testu automatycznegp
    char choice;

    while (true) {
        cout << "Czy chciałbyś wykonać testowanie automatyczne algorytmów dla wielu wartości? t/n" << endl
             << "(Opcja do generowania sprawozdania). Aby wyjść naciśnij dowolną inną literę" <<endl;
        cout<<">>";
        cin >> choice;
        cout<<endl;

//        system("cls");//todo zostawic to

        if (choice == 't' or choice == 'T') {
//            startAutoTesting();
        } else if (choice == 'n' or choice == 'N') {
            startTest();
        } else {
            return;
        }

    }
}

//destruktor klasy Test służy do zwolnienia miejsca w pamięci zajmowanej przez macierz sąsiedstwa grafu
Test::~Test() {
    //todo dorobic destruktor
    deleteMatrix();
}


void Test::pokazDane(){
    if(fileName!="")cout<<"Nazwa Pliku: "<<fileName<<endl;
    if(stopCondition!="")cout<<"Kryterium Stopu : "<<stopCondition<<endl;
    if(neighbours!="")cout<<"Wybor Sasiedstwa: "<<neighbours<<endl;
    if(dT!="")cout<<"Współczynnik Zmiany Temperatury: "<<dT<<endl;
}

//uruchomienie testów algorytmu
void Test::startTest(){
    while (true){
        pokazDane();

        cout << "Co chcesz zrobić?\n"
             "1.Wczytanie danych z pliku\n"
             "2.Wprowadzenie kryterium stopu (wspólne dla obu algorytmów)\n"
             "3.Wybór sąsiedztwa\n"
             "4.Uruchomianie algorytmu TS\n"
             "5.Ustawienie współczynnika zmiany temperatury dla SW\n"
             "6.Uruchomianie algorytmu SW\n"
             "7.Zapis ścieżki rozwiązania do pliku txt\n"
             "8.Wczytanie ścieżki rozwiązania z pliku txt i obliczenie na drogi na podstawie wczytanej tabeli kosztów\n"
             "0.Wyjście" << endl
             <<"9.wczytj z txt" << endl;//todo na czas testów
        //wybór czynności przez użytkownika
        int choice;
        cout<<">>";
        cin >> choice;
        cout<<endl;

        string sciezka;

        int ukosnikPos;
        switch (choice) {
            case 1:
//                system("cls");//todo zostawic to
                cout<<"Podaj ścieżkę do pliku:\n>>";
                cin>>sciezka;

                ukosnikPos = sciezka.rfind('/');
                if(ukosnikPos==string::npos)ukosnikPos = sciezka.rfind('\\');
                fileName = sciezka.substr(ukosnikPos+1);
                break;
            case 2:
//                system("cls");//todo zostawic to
                cout<<"Podaj czas po jakim algorytm ma zakończyć działanie (w sekundach)"<<endl<<">>";
                cin>>stopCondition;
                break;
            case 3:
                //todo jesli zrobie TS
                break;
            case 4:
                //todo jesli zrobie TS
                break;
            case 5:
//                system("cls");//todo zostawic to
                cout<<"Podaj wspolczynnik zmiany temperatury"<<endl<<">>";
                cin>>dT;
                break;
            case 6:
                if(dT!=NULL and stopCondition!=NULL) {
                    startAnneling();
                }
                else{
                    cout<<"Uzupełnij dane do algorytmu"<<endl;
                }
                break;
            case 7:
                break;
            case 8:
                break;
            case 9://todo na czas testów
                readFromFile("C:\\Users\\radom\\Desktop\\PEA2\\z1\\tsp.txt");
                break;
            default:
//                system("cls");//todo zostawic to
                return;
        }
//        system("cls");//todo zostawic to
    }
}


void Test::readFromFile(string path)  {//todo na czas testów
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
    printMatrix();
}

void Test::printMatrix() {//todo na czas testów
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
void  Test::deleteMatrix() {//todo na czas testów
    if(matrixSize!=0 ) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] matrix[i];
        }

        matrixSize = 0;

    }
    delete[] matrix;
}

void Test::startAnneling() {
    Sa* simAnneling = new Sa(matrix, matrixSize);

    simAnneling.start();
    delete simAnneling;
}