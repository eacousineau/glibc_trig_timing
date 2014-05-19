// source: https://bugs.launchpad.net/ubuntu/+source/glibc/+bug/192134
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <sys/time.h>

using namespace std;

int main(int argc, char** argv) {

    volatile union {
        double dbl;
        unsigned char hex[sizeof(double)];
    } value;

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
                " sintest 0.93340582292648832662962377071381";
        return (EXIT_FAILURE);
    }

    cout.precision(32);
    cout << value.dbl << endl;

    cout << "start\n";

    struct timeval time1, time2;
    gettimeofday(&time1, NULL);
    for (int i=0; i < 10000; i++) {
        volatile double out = sinl(value.dbl);
    }
    gettimeofday(&time2, NULL);

    long long diftime = 1000000ll * (time2.tv_sec - time1.tv_sec) +
                         (time2.tv_usec - time1.tv_usec);

    cout << "end: " << diftime / 1000000ll << '.' << setw(6) << setfill('0') << diftime % 1000000ll << " s" << endl;

    return (EXIT_SUCCESS);
}
