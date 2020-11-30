#include <iostream>
#include <cstdlib>
#include "linear_algebra/linear_algebra.hh"
#include "high_precision_computing/hpfloat.hh"
using linearalgebra::Matrix;
using linearalgebra::Vector;
using linearalgebra::LinearSystem;
using std::cout;
using std::endl;

int main() {
    LinearSystem<hpfloat>* ls = LinearSystem<hpfloat>::GetInstance();
    for (size_t dim = 3; dim <= 12; ++dim) {
        Matrix<hpfloat> H(dim, dim);
        Matrix<hpfloat> dH(dim, dim);
        Vector<hpfloat> b(dim, "col");
        Vector<hpfloat> db(dim, "col");
        for (size_t i = 0; i < dim; ++i) {
            b.value(i) = 1.;
            db.value(i) = (hpfloat)2e-16 * (hpfloat)((double)rand() / (double)RAND_MAX - 0.5);
            for (size_t j = 0; j < dim; ++j) {
                H.value(i, j) = (hpfloat)1. / (hpfloat)(i + j + 1.);
                dH.value(i, j) = (hpfloat)2e-16 * (hpfloat)((double)rand() / (double)RAND_MAX - 0.5);
            }
        }
        Vector<hpfloat> x_star(ls->DirectGauss(H, b));
        Vector<hpfloat> x1(ls->DirectGauss(H + dH, b + db));
        Vector<hpfloat> x2(ls->IterGaussSeidel(H + dH, b + db, 1e-100, 10000UL));
        cout << "Dimension = " << dim << endl
            << "                  condInf H = " << H.CondInf() << endl
            << "                    delta H = " << dH.NormInf() << endl
            << "                    delta b = " << db.NormInf() << endl
            << "(Direct Gauss)      delta x = " << (x_star - x1).NormInf() << endl
            << "(Iter Gauss Seidel) delta x = " << (x_star - x2).NormInf() << endl
            << "--------------------------------------------" << endl;
    }
    delete ls;
    return 0;
}