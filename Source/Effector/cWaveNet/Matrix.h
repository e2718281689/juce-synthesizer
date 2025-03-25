#ifndef _MATRIX_H
#define _MATRIX_H



#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

// 定义矩阵结构体
typedef struct 
{
    float* data;
    int rows;
    int cols;
} Matrix;

Matrix createMatrix(int rows, int cols);
void setColumn(Matrix* mat, int colIndex, const Matrix* vec);
// void printMatrix(const Matrix* mat);
void freeMatrix(Matrix* mat);
float getElement(const Matrix* mat, int row, int col);
void setElement(Matrix* mat, int row, int col, float value);
void getRow(const Matrix* mat, int rowIndex, Matrix* result);
void cwiseProduct(const Matrix* mat1, const Matrix* mat2, Matrix* result);
float cwiseProductAndSum(const Matrix* mat1, const Matrix* mat2);
void copyColumn(Matrix* state_cols, int colIndex, const Matrix* state, int stateColIndex);
void copySelectedColumns(Matrix* state_cols, const Matrix* state, const int* state_ptrs, int kernel_length);
float cwiseProductAndSum(const Matrix* mat1, const Matrix* mat2);
void clearMatrix(Matrix* mat);
int matrixMultiply(const Matrix* A, const Matrix* B, Matrix* C);
void matrixAdd(const Matrix* mat1, const Matrix* mat2, Matrix* result);
void matrixTanh(const Matrix* input, Matrix* output);
#ifdef __cplusplus
}
#endif

#endif /* _MATRIX_H */