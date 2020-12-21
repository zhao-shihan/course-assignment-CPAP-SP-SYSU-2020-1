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
    const double a = -1.0;
    const double b = 1.0;
    const size_t n_plot = 1000;

    vector<double> x_interpolation(n_plot);
    vector<double> y_interpolation(n_plot);
    LinearNodes(a, b, x_interpolation);

    Interpolation interpolation;

    string file_name_head("linear_nodes_order");
    for (size_t n = 11; n <= 31; n += 10) {
        vector<double> x_sample(n);
        vector<double> y_sample(n);
        LinearNodes(a, b, x_sample);
        Discretize(func, x_sample, y_sample);
        interpolation.Lagrange(x_sample, y_sample, x_interpolation, y_interpolation);
        stringstream ss;
        ss << n - 1 << std::flush;
        OutputXY(file_name_head + ss.str() + "_sample.csv", x_sample, y_sample);
        OutputXY(file_name_head + ss.str() + "_interpolation.csv", x_interpolation, y_interpolation);
    }

    file_name_head = "Chebyshev_nodes_order";
    for (size_t n = 11; n <= 31; n += 10) {
        vector<double> x_sample(n);
        vector<double> y_sample(n);
        ChebyshevNodes(a, b, x_sample);
        Discretize(func, x_sample, y_sample);
        interpolation.Lagrange(x_sample, y_sample, x_interpolation, y_interpolation);
        stringstream ss;
        ss << n - 1 << std::flush;
        OutputXY(file_name_head + ss.str() + "_sample.csv", x_sample, y_sample);
        OutputXY(file_name_head + ss.str() + "_interpolation.csv", x_interpolation, y_interpolation);
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
        x[i] = _2_average + length * cos((i + 0.5) * M_PI / x.size());
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

