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
    
    bool use_long = false;
    if (argc >= 2 && string(argv[1]) == "-l")
    {
        use_long = true;
        // Shift arguments
        --argc;
        ++argv;
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
    cout << value.dbl << endl;
    
    if (use_long)
        cout << "using sinl()\n";

    cout << "start\n";

    timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    const int count = 10000;
    if (use_long)
    {
        for (int i=0; i < count; i++) {
            volatile double out = sinl(value.dbl);
        }
    }
    else
    {
        for (int i=0; i < count; i++) {
            volatile double out = sin(value.dbl);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    long long diftime = NSEC_PER_SEC * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_nsec - t_start.tv_nsec);
    double diff = diftime / double(NSEC_PER_SEC);

    cout << "end: " << diff << " s" << endl;

    return (EXIT_SUCCESS);
}
