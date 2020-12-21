#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include "interpolation/interpolation.hh"
using std::vector;
using std::string;
using std::stringstream;
using std::ofstream;

inline double func(const double x);
bool Discretize(double (*f)(double), const vector<double>& x_sample, vector<double>& y_sample);
void LinearNodes(const double x_begin, const double x_end, vector<double>& x);
void ChebyshevNodes(const double x_begin, const double x_end, vector<double>& x);
bool OutputXY(const string& file_name, const vector<double>& x, const vector<double>& y);

int main() {
    Interpolation interpolation;
    vector<double> x_interpolation(100);
    vector<double> y_interpolation(100);

    string file_name_head("linear_nodes_");
    for (size_t n = 5; n < 31; ++n) {
        vector<double> x_sample(n);
        vector<double> y_sample(n);
        LinearNodes(-1.0, 1.0, x_sample);
        Discretize(func, x_sample, y_sample);
        interpolation.Lagrange(x_sample, y_sample, x_interpolation, y_interpolation);
        stringstream ss;
        ss << n << std::flush;
        OutputXY(file_name_head + ss.str() + ".csv", x_interpolation, y_interpolation);
    }

    file_name_head = "Chebyshev_nodes_";
    for (size_t n = 5; n < 31; ++n) {
        vector<double> x_sample(n);
        vector<double> y_sample(n);
        ChebyshevNodes(-1.0, 1.0, x_sample);
        Discretize(func, x_sample, y_sample);
        interpolation.Lagrange(x_sample, y_sample, x_interpolation, y_interpolation);
        stringstream ss;
        ss << n << std::flush;
        OutputXY(file_name_head + ss.str() + ".csv", x_interpolation, y_interpolation);
    }
    return 0;
}

inline double func(const double x) {
    return 1.0 / (1.0 + 25.0 * x * x);
}

bool Discretize(double (*f)(double), const vector<double>& x_sample, vector<double>& y_sample) {
    if (x_sample.size() != y_sample.size()) { return false; }
    for (size_t i = 0; i < x_sample.size(); ++i) {
        y_sample[i] = f(x_sample[i]);
    }
    return true;
}

void LinearNodes(const double x_begin, const double x_end, vector<double>& x) {
    if (x.size() == 1) { return; }
    const double step = (x_end - x_begin) / (x.size() - 1.0);
    *x.begin() = x_begin;
    for (vector<double>::iterator it = x.begin() + 1; it != x.end(); ++it) {
        *it = *(it - 1) + step;
    }
}

void ChebyshevNodes(const double x_begin, const double x_end, vector<double>& x) {
    const double _2_average = x_begin + x_end;
    const double length = x_end - x_begin;
    for (size_t i = 0; i < x.size(); ++i) {
        x[i] = _2_average + length * cos((2.0 * i - 1.0) * M_PI / (2.0 * (x.size() + 1.0)));
        x[i] /= 2.0;
    }
}

bool OutputXY(const string& file_name, const vector<double>& x, const vector<double>& y) {
    if (x.size() != y.size()) { return false; }
    ofstream fout(file_name);
    if (fout.is_open()) {
        fout << "x,y" << std::endl;
        for (size_t i = 0; i < x.size(); ++i) {
            fout << x[i] << ',' << y[i] << std::endl;
        }
        fout.close();
        return true;
    } else {
        return false;
    }
}

