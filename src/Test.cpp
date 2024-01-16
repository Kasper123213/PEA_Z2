#include <fstream>
#include <sstream>
#include "Test.h"
#include "algorithms/Sa.h"
#include "algorithms/Ts.h"

//konstruktor klasy
Test::Test() {

    //zmienna przechowująca decyzje użytkownika o uruchomieniu testu automatycznegp
    char choice;

    while (true) {
        cout << "Czy uruchomic testy automatyczne do sprawozdania? T/N/exit" <<endl;
        cout<<">>";
        cin >> choice;
        cout<<endl;

        if (choice == 't' or choice == 'T') {
            testyDoSprawozdanie();
        }else if(choice == 'n' or choice == 'N'){
            startTest();
        }else{
            return;
        }

    }
}

//destruktor klasy Test służy do zwolnienia miejsca w pamięci zajmowanej przez macierz sąsiedstwa grafu
Test::~Test() {
    deleteMatrix();
}


void Test::pokazDane(){
    if(fileName!="")cout<<"Nazwa Pliku: "<<fileName<<endl;
    if(maxTime!=0)cout<<"Kryterium Stopu : "<<maxTime<<"s"<<endl;
    if(neighbourhood!=0){
        cout<<"Wybor Sasiedstwa: ";
        switch (neighbourhood){
            case 1:
                cout<<"Insert"<<endl;
                break;
            case 2:
                cout<<"Swap"<<endl;
                break;
            case 3:
                cout<<"Invert"<<endl;
                break;
        }
    }
    if(coolingFactor!=0)cout<<"Współczynnik Zmiany Temperatury: "<<coolingFactor<<endl;
    if(coolingType!=0){
        cout<<"Rodzaj Zmiany Temperatury: "<<coolingFactor<<endl;
        switch (coolingType){
            case 1:
                cout<<"Geometryczny"<<endl;
                break;
            case 2:
                cout<<"Logarytmiczny"<<endl;
                break;
            case 3:
                cout<<"Wykładniczy"<<endl;
                break;
        }
    }

    cout<<"_____________________________________________________________________________________"<<endl;
}

//uruchomienie testów algorytmu
void Test::startTest(){
    while (true){
        pokazDane();

        cout << "Co chcesz zrobić?\n"
                "1.Wczytanie grafu z pliku\n"
                "2.Wprowadzenie kryterium stopu (wspólne dla obu algorytmów)\n"
                "3.Wybór sąsiedztwa\n"
                "4.Uruchomianie algorytmu TS\n"
                "5.Ustawienie współczynnika zmiany temperatury dla SW\n"
                "6.Uruchomianie algorytmu SW\n"
                "7.Zapis ścieżki rozwiązania do pliku txt\n"
                "8.Wczytanie ścieżki rozwiązania z pliku txt i obliczenie na drogi na podstawie wczytanej tabeli kosztów\n"
                "9.Wybieranie typu schładzania SW\n"
                "0.Wyjście" << endl;
        //wybór czynności przez użytkownika
        int choice;
        cout<<">>";
        cin >> choice;
        cout<<endl;

        string path;

        int ukosnikPos;
        switch (choice) {
            case 1:
                cout<<"Podaj ścieżkę do pliku zawierającego reprezentację grafu:\n>>";
                cin>>path;

                if(readMatrix(path)) {
                    ukosnikPos = path.rfind('/');
                    if (ukosnikPos == string::npos)ukosnikPos = path.rfind('\\');
                    fileName = path.substr(ukosnikPos + 1);
                }else{
                    cout<<"Nie można wczytać ścieżki"<<endl;
                }
                break;
            case 2:
                cout<<"Podaj czas po jakim algorytm ma zakończyć działanie (w sekundach)"<<endl<<">>";
                cin>>maxTime;
                break;
            case 3:
                cout<<"Wybierz metode generowania sąsiedstwa"<<endl
                    <<"1. Insert"<<endl
                    <<"2. Swap"<<endl
                    <<"3. Invert\n>>";
                cin>>neighbourhood;
                break;
            case 4:
                if(neighbourhood!=0 and maxTime>0 and matrixSize>0) {
                    cout << "Uruchamiam algorytm Tabu Search...\n";
                    startTabuSearch();
                }else{
                    cout<<"Nie podano wszystkich wymaganych parametrów.\n";
                }
                break;
            case 5:
                cout<<"Podaj wspolczynnik zmiany temperatury"<<endl<<">>";
                cin>>coolingFactor;
                break;
            case 6:
                if(coolingFactor!=0 and maxTime>0 and matrixSize>0 and coolingType!=0) {
                    if((coolingType == 1 or coolingType==3) and coolingFactor>=1){
                        cout<<"Przy tym typie chłodzenia współczynnik chłodzenia powinien być mniejszy od 1"<<endl;
                        break;
                    }else {
                        cout << "Uruchamiam algorytm Symulowanego wyżarzania...\n";
                        startAnneling();
                    }
                }
                else{
                    cout<<"Nie podano wszystkich wymaganych parametrów.\n";
                }
                break;
            case 7:
                if(bestSolutionPath.empty())cout<<"Nie zapisano żadnego rozwiązania"<<endl;
                else saveSolution();
                break;
            case 8:
                cout<<"Podaj ścieżkę do pliku z cyklem Hamiltona dla wczytanego wcześniej grafu"<<endl<<">>";
                cin>>path;
                cout<<"Długość cyklu to: "<<checkPath(path)<<endl;
                break;
            case 9:
                cout<<"Wybierz typ zmiany temperatury"<<endl<<
                    "1. Geometryczny"<<endl<<
                    "2. Logarytmiczny"<<endl<<
                    "3. Wykładniczy"<<endl<<">>";
                cin>>coolingType;
                break;
            default:
                return;
        }
    }
}


//usuwanie macierzy sąsiedstwa z pamięci
void  Test::deleteMatrix(){
    if(matrixSize!=0 ) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] matrix[i];
        }
        matrixSize = 0;
    }
    delete[] matrix;
}

//rozpoczęcie wyżarzania
void Test::startAnneling() {
    Sa* simAnneling = new Sa(matrix, matrixSize, coolingFactor, maxTime, coolingType);

    simAnneling->start();
    bestSolutionLen=simAnneling->bestLen;
    bestSolutionPath=simAnneling->bestPath;


    cout<<"Najlepsze rozwiązanie to: "<<endl;
    for(auto i:bestSolutionPath){
        cout<<i<<", ";
    }
    cout<<endl<<"Jego koszt to: "<<bestSolutionLen<<endl<<
        "Znaleziono je po czasie: "<<simAnneling->timeOfBestSolution<<endl<<endl;
    delete simAnneling;
}

//rozpoczęcie tabu search
void Test::startTabuSearch(){
    Ts* tabuSearch = new Ts(matrix, matrixSize, neighbourhood, maxTime);

    tabuSearch->startSearching();
    bestSolutionLen=tabuSearch->bestLen;
    bestSolutionPath=tabuSearch->bestPath;

    cout<<"Najlepsze rozwiązanie to: "<<endl;
    for(auto i:bestSolutionPath){
        cout<<i<<", ";
    }
    cout<<endl<<"Jego koszt to: "<<bestSolutionLen<<endl<<
        "Znaleziono je po czasie: "<<tabuSearch->timeOfBestSolution<<endl<<endl;
    delete tabuSearch;
}

//wczytanie macierzy sąsiedstwa z pliku
bool Test::readMatrix(string path){
    if(matrixSize!=0) deleteMatrix();

    int size{};
    // Otwórz plik
    ifstream file(path);

    // Sprawdź, czy udało się otworzyć plik
    if (!file.is_open()) {
        return false;
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
            matrixSize = size;
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
    return true;
}

//znalezienie kosztu podanej ścieżki
int Test::checkPath(string path){
    int len = 0;

    int lastCity=-1;
    int currentCity=-1;
    // Otwarcie pliku do odczytu
    ifstream file(path);

    // Sprawdzenie, czy plik został otwarty poprawnie
    if (!file.is_open()) {
        cout << "Błąd podczas otwierania pliku." << endl;
    }
    int size;
    file >> size;
    file>> lastCity;

    while (file >> currentCity) {
        len+=matrix[lastCity][currentCity];

        lastCity = currentCity;
    }


    // Zamknięcie pliku
    file.close();

    return len;
}



//zapisanie rozwiązania do pliku
void Test::saveSolution(){
    int index = 1;
    string name ;
    while(true){
        name = "solution" + to_string(index) + ".txt";

        ifstream file(name);
        if(not file.good()) break;
        index++;
    }

    string solutionFile = name;

    // Otwarcie pliku w trybie do zapisu (truncation)
    ofstream file(solutionFile, ios::trunc);

    // Sprawdzenie, czy plik został otwarty poprawnie
    if (file.is_open()) {
        file << bestSolutionPath.size()-1 << endl;

        for(int i:bestSolutionPath){
            file << i <<endl;
        }

        // Zamknięcie pliku
        file.close();

        cout << "Zapisano do pliku." << endl;
    } else {
        cout << "Błąd podczas otwierania pliku." << endl;
    }
}




void Test::testyDoSprawozdanie(){

    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\ftv55.atsp");
    cout<<"odpalam ftv55, cooling type = 1"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_ftv55_1_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.9989, 2*60, 1);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }


    cout<<"odpalam ftv55, cooling type = 2"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_ftv55_2_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.00014, 2*60, 2);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }


    cout<<"odpalam ftv55, cooling type = 3"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_ftv55_3_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.99999992, 2*60, 3);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }






    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\ftv170.atsp");
    cout<<"odpalam ftv170, cooling type = 1"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_ftv170_1_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.9952, 4*60, 1);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }




    cout<<"odpalam ftv170, cooling type = 2"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_ftv170_2_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.0012, 4*60, 2);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }




    cout<<"odpalam ftv170, cooling type = 3"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_ftv170_3_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.9999981, 4*60, 3);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }




    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\rbg358.atsp");

    cout<<"odpalam rbg358, cooling type = 1"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_rbg358_1_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.986, 6*60, 1);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }



    cout<<"odpalam rbg358, cooling type = 2"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_rbg358_2_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.003, 6*60, 2);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }



    cout<<"odpalam rbg358, cooling type = 3"<<endl;
    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "SA_rbg358_3_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Sa* simAnneling = new Sa(matrix, matrixSize, 0.999986, 6*60, 3);
        simAnneling->start();

        for(int x:simAnneling->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:simAnneling->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete simAnneling;
    }




    cout<<"########################################################"<<endl<<
        "Tabu Search"<<endl<<
        "########################################################"<<endl;

    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\ftv55.atsp");
    cout<<"odpalam ftv55, sąsiedzi = 1"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_ftv55_1_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 1, 2*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }






    cout<<"odpalam ftv55, sąsiedzi = 2"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_ftv55_2_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 2, 2*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }




    cout<<"odpalam ftv55, sąsiedzi = 3"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_ftv55_3_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 3, 2*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }

    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\ftv170.atsp");
    cout<<"odpalam ftv170, cooling type = 1"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_ftv170_1_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 1, 4*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }






    cout<<"odpalam ftv170, cooling type = 2"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_ftv170_2_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 2, 4*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }




    cout<<"odpalam ftv170, cooling type = 3"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_ftv170_3_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 3, 4*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }

    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\rbg358.atsp");
    cout<<"odpalam rbg358, cooling type = 1"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_rbg358_1_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 1, 6*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }




    cout<<"odpalam rbg358, cooling type = 2"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_rbg358_2_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 2, 6*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }





    cout<<"odpalam rbg358, cooling type = 3"<<endl;

    for(int i=0;i<10;i++) {
        cout<<i<<endl;

        string nazwa = "TS_rbg358_3_" + to_string(i) + ".csv";
        ofstream excelFile(nazwa);

        Ts* tabuSearch = new Ts(matrix, matrixSize, 3, 6*60);
        tabuSearch->startSearching();

        for(int x:tabuSearch->wykresBesty){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->wykresCzasy){
            excelFile<<x<<";";
        }
        excelFile<<endl;
        for(int x:tabuSearch->bestPath){
            excelFile<<x<<";";
        }

        excelFile.close();

        delete tabuSearch;
    }
}