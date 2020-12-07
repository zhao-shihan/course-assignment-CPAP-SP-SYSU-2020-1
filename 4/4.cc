#include <iostream>
#include "linear_algebra/linear_algebra.hh"
using std::cout;
using std::endl;
using linearalgebra::Matrix;
using linearalgebra::EigenSystem;

int main() {
    EigenSystem<double>* es = new EigenSystem<double>;
    Matrix<double> A(3, 3);
    A[0][0] = 9;    A[0][1] = 4.5;    A[0][2] = 3;
    A[1][0] = -56;  A[1][1] = -28;    A[1][2] = -18;
    A[2][0] = 60;   A[2][1] = 30;     A[2][2] = 19;
    cout << es->QRHouseholder(A, 1e-6, 1000) << endl;
    A[2][2] = 18.95;
    cout << es->QRHouseholder(A, 1e-6, 1000) << endl;
    A[2][2] = 19.05;
    cout << es->QRHouseholder(A, 1e-6, 1000) << endl;
    return 0;
}
