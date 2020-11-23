#include <iostream>
#include "high_precision_computing/float.hh"
#include "high_precision_computing/math.hh"
#include "analysis/analysis.hh"
using std::endl;

Analysis<hp_float> anal;

static const hp_float dx = 1e-40;
static const size_t max_iter = 100;
hp_float phi(hp_float x);
hp_float func(hp_float x);

int main() {
    for (size_t i = 1; i <= 3; ++i) {
        if (func(i) < 0.) {
            hp_float left = anal.FindRoot(func, i - 0.1, max_iter, 1e-32);
            hp_float right = anal.FindRoot(func, i + 0.1, max_iter, 1e-32);
            if (left < right) {
                cout << "For x = " << i << ", the convergence domain is" << endl << "( ";
                left.Print(32);
                cout << ", ";
                right.Print(32);
                cout << " )" << endl;
                continue;
            }
        }
        cout << "For x = " << i << ", the equation does not converges." << endl;
    }
    return 0;
}

hp_float phi(hp_float x) {
    return -0.5 * (x * x * x - 6.0 * x * x + 9.0 * x - 6.0);
}

hp_float func(hp_float x) {
    hp_float a = fabs(anal.D(phi, x, dx)) - 1.;
    return a;
}