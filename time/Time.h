#include <iostream>
#include <chrono>

#ifndef PEA1_TIME_H
#define PEA1_TIME_H

using namespace std;
using namespace std::chrono;
class Time {
private:
    high_resolution_clock::time_point startTime;
    high_resolution_clock::time_point stopTime;


public:
    void start();

    void stop();

    unsigned long long getTime();
};


#endif //PEA1_TIME_H
