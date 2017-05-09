#ifndef tspsolver_Clock_h
#define tspsolver_Clock_h

#include <ctime>

class Clock {
private:
    clock_t start;
public:
    Clock() {
        reset();
    }

    void reset() {
        start = clock();
    }

    double getseconds() {
        clock_t end = clock();

        return (double) (end - start) / CLOCKS_PER_SEC;
    }
};

#endif