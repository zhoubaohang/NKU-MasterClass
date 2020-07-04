#include "conjugategradient.h"

ConjugateGradient::ConjugateGradient()
{

}

ConjugateGradient::~ConjugateGradient()
{

}

matrix ConjugateGradient::solve(matrix A, matrix b)
{
    if (x != nullptr)
        delete x;
    if (d != nullptr)
        delete d;
    if (r != nullptr)
        delete r;
    // 解向量
    x = new matrix(A.getColSize(), 1, 0);
    // 残差向量
    r = new matrix(b - A.mult(*x));
    // 方向向量
    d = new matrix(*r);
    matrix new_r = *r - A.mult(*d);

    double denom1, denom2, alpha, beta, length;

    for (int i = 0; i < 10000; i++) {
        denom1 = r->transpose().mult(*r)(0, 0);
        denom2 = d->transpose().mult(A).mult(*d)(0,0);
        alpha = denom1 / denom2;
        *x = *x + *d * alpha;
        new_r = *r - A.mult(*d) * alpha;

        length = sqrt(new_r.transpose().mult(new_r)(0,0));

        beta = new_r.transpose().mult(new_r)(0,0) /
                r->transpose().mult(*r)(0,0);
        *d = new_r + *d * beta;

        delete r;
        r = new matrix(new_r);
        qDebug()<<length;
    }

    return *x;
}
