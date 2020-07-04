#include "matrix.h"

matrix::matrix()
{
    this->p_matrix = nullptr;
    this->p_row = 0;
    this->p_col = 0;
}

matrix::matrix(int row, int col)
{
    this->p_row = row;
    this->p_col = col;

    if (row == 0 || col == 0)
        return;

    this->p_matrix = (double**)_mm_malloc(col*sizeof(double*), 16);
    double **p = this->p_matrix;
    double **end = this->p_matrix + col;
    do {
        *(p++) = (double*)_mm_malloc(row*sizeof(double), 16);
    } while (p != end);
}

matrix::matrix(int row, int col, const double init)
{
    this->p_row = row;
    this->p_col = col;

    if (row == 0 || col == 0)
        return;

    this->p_matrix = (double**)_mm_malloc(col*sizeof(double*), 16);
    double **pr = this->p_matrix;
    double **endr = this->p_matrix + col;
    double *p, *end;

    do {
        p = *(pr++) = (double*)_mm_malloc(row*sizeof(double), 16);
        end = p + row;
        do {
            *(p++) = init;
        } while(p != end);
    } while (pr != endr);
}

matrix::matrix(const matrix &B)
{
    this->p_row = B.p_row;
    this->p_col = B.p_col;
    this->p_matrix = (double**)_mm_malloc(this->p_col*sizeof(double*), 16);
    double **pbr = B.p_matrix, **endbr = B.p_matrix + p_col;
    double **par = this->p_matrix, **endar = this->p_matrix + p_col;
    double *pa, *pb, *enda, *endb;

    do {
        pa = *(par++) = (double*)_mm_malloc(p_row*sizeof(double), 16);
        enda = pa + p_row;
        pb = *(pbr++);
        endb = pb + p_row;
        do {
            *(pa++) = *(pb++);
        } while (pa != enda);
    } while (par != endar);
}

matrix::~matrix()
{
    if (!this->p_matrix) return;
    double **p = p_matrix, **end = p_matrix + p_col;

    do {
        _mm_free(*(p++));
    } while (p != end);

    p_row = p_col = 0;
    _mm_free(p_matrix);
}

double& matrix::operator() (int i, int j)
{
    return this->p_matrix[j][i];
}

const double matrix::operator () (int i, int j) const
{
    return this->p_matrix[j][i];
}

matrix& matrix::operator =(matrix&& B)
{
    if (!this->p_matrix) {
        double **p = p_matrix, **end = p_matrix + p_col;

        do {
            _mm_free(*(p++));
        } while (p != end);

        p_row = p_col = 0;
        _mm_free(p_matrix);
    }

    this->p_row = B.p_row;
    this->p_col = B.p_col;
    this->p_matrix = B.p_matrix;
    B.p_matrix = nullptr;
    return *this;
}

matrix matrix::operator ==(matrix B)
{
    if (B.p_col != p_col && B.p_row != p_row)
        return (*this);

    matrix tmp(p_row, p_col, 0);
    for (int i = 0; i < p_row; i++)
        for (int j = 0; j < p_col; j++)
            tmp(i, j) = p_matrix[j][i] == B(i, j) ? 1 : 0;

    return tmp;
}

matrix matrix::mult(const matrix &B)
{
    if (p_col != B.p_row) return *this;
    matrix tmp(p_row, B.p_col, 0);
    int i = 0, j, k;

    do {
        j = 0;
        do {
            k = 0;
            do {
                tmp(i, j) += (*this)(i, k) * B(k, j);
                k ++;
            } while (k < p_col);
            j ++;
        } while (j < B.p_col);
        i ++;
    } while (i < p_row);

    return tmp;
}

void matrix::sse_mult_kernel(double **c, double **a, double **b, int row, int col, int b_col)
{
    __m128d t01_0, t01_1, t01_2, t01_3;
    __m128d t23_0, t23_1, t23_2, t23_3;
    __m128d a0, a1, b0, b1, b2, b3;

    t01_0 = t01_1 = t01_2 = t01_3 = _mm_set1_pd(0);
    t23_0 = t23_1 = t23_2 = t23_3 = _mm_set1_pd(0);

    double *pb0(b[col]), *pb1(b[col+1]), *pb2(b[col+2]), *pb3(b[col+3]);
    double *pa0(a[0]), *pa1(a[1]);
    double *endb0 = pb0 + p_col;

    do {
        a0 = _mm_load_pd(pa0);
        a1 = _mm_load_pd(pa1);

        b0 = _mm_set1_pd(*(pb0++));
        if (col+1 < b_col)
            b1 = _mm_set1_pd(*(pb1++));
        if (col+2 < b_col)
            b2 = _mm_set1_pd(*(pb2++));
        if (col+3 < b_col)
            b3 = _mm_set1_pd(*(pb3++));

        t01_0 += a0 * b0;
        if (col+1 < b_col)
            t01_1 += a0 * b1;
        if (col+2 < b_col)
            t01_2 += a0 * b2;
        if (col+3 < b_col)
            t01_3 += a0 * b3;

        t23_0 += a1 * b0;
        if (col+1 < b_col)
            t23_1 += a1 * b1;
        if (col+2 < b_col)
            t23_2 += a1 * b2;
        if (col+3 < b_col)
            t23_3 += a1 * b3;

        pa0 += 2;
        pa1 += 2;
    } while (pb0 != endb0);

    _mm_store_pd(&c[col][row], t01_0);
    _mm_store_pd(&c[col][row+2], t23_0);
    if (col+1 < b_col) {
        _mm_store_pd(&c[col+1][row], t01_1);
        _mm_store_pd(&c[col+1][row+2], t23_1);
    }
    if (col+2 < b_col) {
        _mm_store_pd(&c[col+2][row], t01_2);
        _mm_store_pd(&c[col+2][row+2], t23_2);
    }
    if (col+3 < b_col) {
        _mm_store_pd(&c[col+3][row], t01_3);
        _mm_store_pd(&c[col+3][row+2], t23_3);
    }
}

matrix matrix::sse_mult(const matrix &B)
{
    if (p_col != B.p_row) return *this;

    matrix tmp(p_row, B.p_col, 0);

    double *ta[2];
    ta[0] = (double*)_mm_malloc(sizeof(double)*2*p_col, 16);
    ta[1] = (double*)_mm_malloc(sizeof(double)*2*p_col, 16);

    int i = 0, j = 0, k;
    do {
        k = 0; i = 0;
        do {
            ta[0][k]   = p_matrix[i][j];
            ta[1][k++] = p_matrix[i][j+2];
            ta[0][k]   = p_matrix[i][j+1];
            ta[1][k++] = p_matrix[i++][j+3];
        } while (i < p_col);

        i = 0;
        do {
            this->sse_mult_kernel(tmp.p_matrix, ta, B.p_matrix, j, i, B.p_col);
            i += 4;
        } while (i < B.p_col);

        j += 4;
    } while (j < p_row);

    _mm_free(ta[0]);
    _mm_free(ta[1]);

    return tmp;
}

matrix matrix::transpose()
{
    matrix tmp(p_col, p_row, 0);

    for (int i = 0; i < p_row; i++)
        for (int j = 0; j < p_col; j++)
            tmp(j, i) = p_matrix[j][i];

    return tmp;
}

matrix matrix::index(vector<int> idx)
{
    matrix tmp(idx.size(), p_col, 0);
    for (int i = 0; i < idx.size(); i++) {
        for (int j = 0; j < p_col; j++) {
            tmp(i, j) = p_matrix[j][idx[i]];
        }
    }
    return tmp;
}

matrix matrix::argmax(int axis)
{
    if (axis == 0) {
        matrix tmp(1, p_col, 0);
        for (int j = 0; j < p_col; j++) {
            int idx = 0; double val = 0;
            for (int i = 0; i < p_row; i++) {
                if (p_matrix[j][i] > val) {
                    idx = i;
                    val = p_matrix[j][i];
                }
            }
            tmp(0, j) = idx;
        }
        return tmp;
    } else if (axis == 1) {
        matrix tmp(p_row, 1, 0);
        for (int i = 0; i < p_row; i++) {
            int idx = 0; double val = 0;
            for (int j = 0; j < p_col; j++) {
                if (p_matrix[j][i] > val) {
                    idx = j;
                    val = p_matrix[j][i];
                }
            }
            tmp(i, 0) = idx;
        }
        return tmp;
    }
}

matrix matrix::operator +(matrix a)
{
    matrix tmp(*this);
    if (p_col == a.getColSize() && p_row == a.getRowSize()) {
        for (int i = 0; i < p_row; i++) {
            for (int j = 0; j < p_col; j++) {
                tmp(i, j) += a(i, j);
            }
        }
    } else if (p_col == a.getColSize()) {
        for (int i = 0; i < p_row; i++) {
            for (int j = 0; j < p_col; j++) {
                tmp(i, j) += a(0, j);
            }
        }
    } else if (p_row == a.getRowSize()) {
        for (int i = 0; i < p_row; i++) {
            for (int j = 0; j < p_col; j++) {
                tmp(i, j) += a(j, 0);
            }
        }
    }
    return tmp;
}

matrix matrix::operator -(double a)
{
    matrix tmp(*this);
    for (int i = 0; i < tmp.getRowSize(); i++)
        for (int j = 0; j < tmp.getColSize(); j++)
            tmp(i, j) -= a;
    return tmp;
}

matrix matrix::operator -(matrix a)
{
    matrix tmp(*this);
    if (p_col == a.getColSize() && p_row == a.getRowSize()) {
        for (int i = 0; i < p_row; i++) {
            for (int j = 0; j < p_col; j++) {
                tmp(i, j) -= a(i, j);
            }
        }
    } else if (p_col == a.getColSize()) {
        for (int i = 0; i < p_row; i++) {
            for (int j = 0; j < p_col; j++) {
                tmp(i, j) -= a(0, j);
            }
        }
    } else if (p_row == a.getRowSize()) {
        for (int i = 0; i < p_row; i++) {
            for (int j = 0; j < p_col; j++) {
                tmp(i, j) -= a(j, 0);
            }
        }
    }
    return tmp;
}

matrix operator -(double a, matrix b)
{
    matrix tmp(b);
    for (int i = 0; i < tmp.getRowSize(); i++)
        for (int j = 0; j < tmp.getColSize(); j++)
            tmp(i, j) = a - tmp(i, j);
    return tmp;
}

matrix ele_log(matrix a)
{
    matrix tmp(a);
    for (int i = 0; i < tmp.getRowSize(); i++)
        for (int j = 0; j < tmp.getColSize(); j++)
            tmp(i, j) = log(tmp(i, j));
    return tmp;
}

double sum(matrix a)
{
    double sum = 0;
    for (int i = 0; i < a.getRowSize(); i++)
        for (int j = 0; j < a.getColSize(); j++)
            sum += a(i, j);
    return sum;
}

double mean(matrix a)
{
    double tmp = sum(a);
    return tmp / double(a.getRowSize() * a.getColSize());
}

matrix matrix::operator *(double a)
{
    matrix tmp(*this);
    for (int i = 0; i < tmp.getRowSize(); i++)
        for (int j = 0; j < tmp.getColSize(); j++)
            tmp(i, j) *= a;
    return tmp;
}

matrix matrix::operator *(matrix a)
{
    if (p_col != a.getColSize() && p_row != a.getRowSize())
        return (*this);

    matrix tmp(*this);
    for (int i = 0; i < tmp.getRowSize(); i++)
        for (int j = 0; j < tmp.getColSize(); j++)
            tmp(i, j) *= a(i, j);
    return tmp;
}

QString matrix::toString()
{
    QString result = "";
    for (int i = 0; i < p_row; i++) {
        for (int j = 0; j < p_col; j++) {
            result +=  QString::number(p_matrix[j][i]) + " ";
        }
        result += "\n";
    }
    return result;
}

int matrix::getRowSize()
{
    return p_row;
}

int matrix::getColSize()
{
    return p_col;
}
