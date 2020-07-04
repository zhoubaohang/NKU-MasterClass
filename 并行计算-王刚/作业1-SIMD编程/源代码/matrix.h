#ifndef MATRIX_H
#define MATRIX_H

#include <QDebug>
#include <cstdlib>
#include <QString>
#include <immintrin.h>

using namespace std;

class matrix
{
public:
    matrix();
    ~matrix();
    matrix(int row, int col);
    matrix(int row, int col, const double init);
    matrix(const matrix& B);
    // 索引取值
    double& operator() (int i, int j);
    const double operator() (int i, int j) const;
    // 矩阵赋值
    matrix& operator =(matrix&& B);
    // 矩阵元素判等
    matrix operator ==(matrix B);
    // 减法运算
    matrix operator -(double a);
    matrix operator -(matrix a);
    // 加法运算
    matrix operator +(matrix a);
    // 元素乘法运算
    matrix operator *(double a);
    matrix operator *(matrix a);
    // 索引运算
    matrix index(vector<int> idx);
    // 最大值索引
    matrix argmax(int axis);

    // 矩阵乘法
    matrix mult(const matrix &B);
    // SIMD 加速
    matrix sse_mult(const matrix &B);
    // 矩阵转置
    matrix transpose();

    int getRowSize();
    int getColSize();
    QString toString();

private:
    double **p_matrix;
    int p_row, p_col;
    void sse_mult_kernel(double **c, double **a, double **b, int row, int col, int b_col);
};

// 标量减法运算
matrix operator -(double a, matrix b);
// 元素log运算
matrix ele_log(matrix a);
// 求和运算
double sum(matrix a);
// 均值运算
double mean(matrix a);

#endif // MATRIX_H
