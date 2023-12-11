#include "Time.h"

//włączanie "stopera", zapisywanie obecnego czasu do zmiennej startTime
void Time::start() {
    startTime = high_resolution_clock::now();
}
//wyłączanie "stopera", zapisywanie obecnego czasu do zmiennej stopTime
void Time::stop() {
    stopTime = high_resolution_clock::now();
}

//odejmowanie czasu początkowego i koncowego w celu obliczenia różnicy
//dla lepszego efektu wynik jest zwracany z dużą dokładnością.
// Wyświetlany jest w jednostkach łatwiejszych do wyobrażenia sobie przez człowieka jednak na wykresie
//zapisywana jest dokładniejsza wersja; w nanosekundach
unsigned long long Time::getTime() {
    return duration_cast<seconds >(stopTime - startTime).count();
}
