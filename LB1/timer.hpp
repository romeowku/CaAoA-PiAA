#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

class Timer {
public:
    Timer() {
        start_time = high_resolution_clock::now();
    }

    void stop() {
        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end_time - start_time);
        cout << "Время выполнения: " << duration.count() << " микросекунд." << endl;
    }

private:
    high_resolution_clock::time_point start_time;
};

#endif