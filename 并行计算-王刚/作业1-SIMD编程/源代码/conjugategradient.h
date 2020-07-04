#ifndef CONJUGATEGRADIENT_H
#define CONJUGATEGRADIENT_H

#include "matrix.h"
#include <QDebug>

class ConjugateGradient
{
public:
    ConjugateGradient();
    ~ConjugateGradient();

    matrix solve(matrix A, matrix b);

private:
    matrix *x = nullptr;
    matrix *d = nullptr;
    matrix *r = nullptr;
};

#endif // CONJUGATEGRADIENT_H
