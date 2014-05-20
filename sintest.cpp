// source: https://bugs.launchpad.net/ubuntu/+source/glibc/+bug/192134
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>

using namespace std;

const int RT_PRIORITY = 49;
const int NSEC_PER_SEC = 1000000000;

int main(int argc, char** argv) {
    
    sched_param param;
    param.__sched_priority = RT_PRIORITY;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)
    {
        cerr << "sched_setscheduler failed" << endl;
        return (EXIT_FAILURE);
    }

    volatile union {
        double dbl;
        unsigned char hex[sizeof(double)];
    } value;
    
    const char *funcs[] = {"sin", "sinf", "sinl"};
    int func_index = 0;
    if (argc >= 2)
    {
        string arg = argv[1];
        bool do_shift = false;
        if (arg == "sin")
        {
            func_index = 0;
            do_shift = true;
        }
        if (arg == "sinf")
        {
            func_index = 1;
            do_shift = true;
        }
        else if (arg == "sinl")
        {
            func_index = 2;
            do_shift = true;
        }
        if (do_shift)
        {
            // Shift arguments
            --argc;
            ++argv;
        }
    }

    if (argc == sizeof(double)+1) {
        for (int i=0; i < sizeof(double); i++) {
            istringstream s(argv[i+1]);
            int tmp;
            s >> hex >> tmp;
            value.hex[i] = tmp;
        }
    } else if (argc == 2) {
        istringstream s(argv[1]);
        double tmp;
        s >> tmp;
        value.dbl = tmp;
    } else {
        cout << "usage: sintest 00 b0 6b e3 75 de ed 3f\n"
                " sintest 0.93340582292648832662962377071381" << endl;
        return (EXIT_FAILURE);
    }

    cout.precision(32);

    timespec t_start;
    const int count = 10000;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    switch (func_index)
    {
        case 0:
            for (int i=0; i < count; i++)
                volatile double out = sin(value.dbl);
            break;
        case 1:
            for (int i=0; i < count; i++)
                volatile double out = sinf(value.dbl);
            break;
        case 2:
            for (int i=0; i < count; i++)
                volatile double out = sinl(value.dbl);
            break;
        default:
            cerr << "Invalid func_index: " << func_index << endl;
            return (EXIT_FAILURE);
    }
    timespec t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    long long diftime = NSEC_PER_SEC * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_nsec - t_start.tv_nsec);
    double diff = diftime / double(NSEC_PER_SEC);

    cout
        << "-   value: " << value.dbl << endl
        << "    seconds: " << diff << endl;
        // << "    func: " << funcs[func_index] << endl;

    return (EXIT_SUCCESS);
}
