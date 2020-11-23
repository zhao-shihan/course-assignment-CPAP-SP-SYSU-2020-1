#include <iostream>
#include "high_precision_computing/float.hh"
#include "high_precision_computing/math.hh"
#include "analysis/analysis.hh"

Analysis<hp_float> anal;
static const hp_float dx = 1e-200;
hp_float phi(hp_float x);
hp_float func(hp_float x);

int main() {
    anal.FindRoot(func, 0.9, 10000, 1e-128).Print(128);
}

hp_float phi(hp_float x) {
    return -0.5 * (x * x * x - 6.0 * x * x + 9.0 * x - 6.0);
}

hp_float func(hp_float x) {
    return fabs(anal.D(phi, x, dx)) - 1.;
}