#include <fstream>
#include <sstream>
#include "Test.h"
#include "algorithms/Sa.h"

//konstruktor klasy
Test::Test() {
    //zmienna przechowująca decyzje użytkownika o uruchomieniu testu automatycznegp
    char choice;

//    readFromFile("C:\\Users\\radom\\Desktop\\PEA2\\z1\\tsp.txt");

//    string path = "C:\\Users\\radom\\OneDrive\\Pulpit\\PEA\\PEA_Z2\\z1\\tsp.txt";
////    ftvReader* f;
//    readMatrix(path);
////    string solution = "0, 68, 65, 34, 37, 48, 26, 69, 14, 55, 60, 35, 19, 17, 38, 8, 59, 63, 15, 25, 47, 13, 11, 39, 30, 6, 64, 53, 66, 12, 67, 56, 33, 45, 22, 42, 18, 50, 54, 32, 1, 58, 20, 7, 46, 3, 9, 31, 16, 62, 5, 41, 24, 70, 44, 10, 61, 51, 52, 43, 28, 2, 40, 23, 27, 57, 49, 4, 36, 29, 21, 0,";
////    checkPath(solution);
////    cout<<matrix[0][1];
//    startAnneling();//todo na potrzeby testowania

//    cout<<endl<<"ftv55"<<endl;
//    string path = "C:\\Users\\radom\\OneDrive\\Pulpit\\PEA\\PEA_Z2\\z2_z3\\ATSP\\ftv55.atsp";
//    readMatrix(path);
//    startAnneling();
//
//    cout<<endl<<"ftv170"<<endl;
//    path = "C:\\Users\\radom\\OneDrive\\Pulpit\\PEA\\PEA_Z2\\z2_z3\\ATSP\\ftv170.atsp";
//    readMatrix(path);
//    startAnneling();

    cout<<endl<<"rgb358"<<endl;
    string path = "C:\\Users\\radom\\OneDrive\\Pulpit\\PEA\\PEA_Z2\\z2_z3\\ATSP\\rbg358.atsp";
    readMatrix(path);
    startAnneling();
    return;
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
    if(maxTime!=-1)cout<<"Kryterium Stopu : "<<maxTime<<endl;
    if(neighbours!="")cout<<"Wybor Sasiedstwa: "<<neighbours<<endl;
    if(coolingFactor!=-1)cout<<"Współczynnik Zmiany Temperatury: "<<coolingFactor<<endl;
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
                cin>>maxTime;
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
                cin>>coolingFactor;
                break;
            case 6:
//                if(coolingFactor!=-1 and maxTime!=-1) {
                    startAnneling();
//                }
//                else{
//                    cout<<"Uzupełnij dane do algorytmu"<<endl;
//                }
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
//    Sa* simAnneling = new Sa(matrix, matrixSize, coolingFactor, maxTime);//todo to zostawic
    Sa* simAnneling = new Sa(matrix, matrixSize, 0.998, 4*60);

    simAnneling->start();
    delete simAnneling;
}

void Test::readMatrix(string path){
    int size{};
    // Otwórz plik
    ifstream file(path);

    // Sprawdź, czy udało się otworzyć plik
    if (!file.is_open()) {
        cout << "Nie można otworzyc." << endl;
    }

    // Zmienna przechowująca wartość wczytaną z pliku
    string word;

    // Wyczyść flagi błędów i pozycję odczytu pliku
    file.clear();
    file.seekg(0, ios::beg);

    bool startReading = false;
    bool readSize = false;

    int x = 0;
    int y = 0;
    while (file >> word) {
        if (word.find("DIMENSION:") != string::npos) {
            readSize = true;
            continue;
        }
        if (readSize) {
            istringstream iss(word);
            iss >> size;
            matrix = new int *[size];
            for(int i=0; i<size;i++){
                matrix[i] = new int[size];
            }
            readSize = false;
        }

        if (word.find("EDGE_WEIGHT_SECTION") != string::npos) {
            startReading = true;
            continue;
        }
        if (startReading) {
            istringstream iss(word);
            float weight{};
            while (iss >> weight) {
                matrix[y][x] = weight;
                x++;
                if(x==size){
                    x=0;
                    y++;
                }
            }
        }

    }
    file.close();

//    for(int i=0;i<y;i++){
//        for(int j=0;j<y;j++){
//            cout<<matrix[i][j]<<", ";
//        }
//        cout<<endl<<"########################################"<<endl;
//    }
    matrixSize = y;


}


void Test::checkPath(string solution){
    std::vector<int> numbers;
    std::stringstream ss(solution);
    int number;

    // Wczytaj liczby ze stringa do wektora
    while (ss >> number) {
        numbers.push_back(number);

        // Jeśli następny znak to przecinek, zignoruj go
        if (ss.peek() == ',')
            ss.ignore();
    }

    int sum = 0;
    for(int i=0; i<numbers.size()-1;i++){
        sum+=matrix[numbers[i]][numbers[i+1]];
    }
    cout<<"Długość podanej ścieżki to: "<<sum<<endl;
}