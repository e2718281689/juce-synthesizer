#include "Matrix.h"

// 创建矩阵
Matrix createMatrix(int rows, int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (float*)malloc(rows * cols * sizeof(float));
    return mat;
}

// 释放矩阵内存
void freeMatrix(Matrix* mat) {
    free(mat->data);
    mat->data = NULL;
}
// 根据索引复制列
void copySelectedColumns(Matrix* state_cols, const Matrix* state, const int* state_ptrs, int kernel_length) 
{
    for (int k = 0; k < kernel_length; ++k) {
        int stateColIndex = state_ptrs[k];
        copyColumn(state_cols, k, state, stateColIndex);
    }
}
// 复制一列
void copyColumn(Matrix* state_cols, int colIndex, const Matrix* state, int stateColIndex) 
{
    if (state->rows != state_cols->rows) {
        printf("Matrix row size mismatch!\n");
        return;
    }
    
    for (int i = 0; i < state->rows; ++i) {
        float value = getElement(state, i, stateColIndex);
        setElement(state_cols, i, colIndex, value);
    }
}
// 访问矩阵中的元素（行优先存储）
float getElement(const Matrix* mat, int row, int col) {
    return mat->data[row * mat->cols + col];
}

void setElement(Matrix* mat, int row, int col, float value) {
    mat->data[row * mat->cols + col] = value;
}

// 将向量插入到矩阵的指定列
void setColumn(Matrix* mat, int colIndex, const Matrix* vec) 
{
    if (vec->cols != 1 || vec->rows != mat->rows) {
        printf("Matrix size mismatch!\n");
        return;
    }
    
    for (int i = 0; i < mat->rows; ++i) {
        mat->data[i * mat->cols + colIndex] = vec->data[i];
    }
}

void cwiseProduct(const Matrix* mat1, const Matrix* mat2, Matrix* result) {
    // 检查矩阵的大小是否匹配
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        printf("Matrix dimensions must match for cwiseProduct!\n");
        return;
    }
    
    // 进行逐元素相乘并存入 result
    for (int i = 0; i < mat1->rows; ++i) {
        for (int j = 0; j < mat1->cols; ++j) {
            float value = getElement(mat1, i, j) * getElement(mat2, i, j);
            setElement(result, i, j, value);
        }
    }
}
float cwiseProductAndSum(const Matrix* mat1, const Matrix* mat2) 
{
    float sum=0;
    // 检查矩阵的大小是否匹配
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        printf("Matrix dimensions must match for cwiseProduct!\n");
        return;
    }
    

    // 进行逐元素相乘并存入 result
    for (int i = 0; i < mat1->rows; ++i) {
        for (int j = 0; j < mat1->cols; ++j) {
            float value = getElement(mat1, i, j) * getElement(mat2, i, j);
            sum += value;
        }
    }
    return sum;
}

//矩阵乘法
int matrixMultiply(const Matrix* A, const Matrix* B, Matrix* C) {
    if (A->cols != B->rows) {
        printf("Matrix size mismatch for multiplication! A: %d x %d, B: %d x %d\n",
               A->rows, A->cols, B->rows, B->cols);
        return 0; // 返回 0 表示乘法失败
    }

    if (A->rows != C->rows || B->cols != C->cols) {
        printf("Output matrix size mismatch! C should be %d x %d\n", A->rows, B->cols);
        return 0;
    }

    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < B->cols; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < A->cols; ++k) {
                sum += getElement(A, i, k) * getElement(B, k, j);
            }
            setElement(C, i, j, sum);
        }
    }

    return 1; // 返回 1 表示乘法成功
}
// 将两个矩阵相加，结果存储在 result 中
void matrixAdd(const Matrix* mat1, const Matrix* mat2, Matrix* result)
{
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        printf("Matrix size mismatch in addition!\n");
        return;
    }

    for (int i = 0; i < mat1->rows; ++i) {
        for (int j = 0; j < mat1->cols; ++j) {
            float sum = getElement(mat1, i, j) + getElement(mat2, i, j);
            setElement(result, i, j, sum);
        }
    }
}
void copyMatrix(const Matrix* src, Matrix* dst) 
{
    if (src->rows != dst->rows || src->cols != dst->cols) {
        printf("Matrix size mismatch in copy operation!\n");
        return;
    }

    memcpy(dst->data, src->data, src->rows * src->cols * sizeof(float));
}
// tanh 激活操作
void matrixTanh(const Matrix* input, Matrix* output) 
{
    if (input->rows != output->rows || input->cols != output->cols) {
        printf("Matrix size mismatch in tanh operation!\n");
        return;
    }

    for (int i = 0; i < input->rows * input->cols; ++i) {
        output->data[i] = tanhf(input->data[i]);
    }
}

//清空矩阵
void clearMatrix(Matrix* mat) 
{
    if (mat && mat->data) 
    {
        memset(mat->data, 0, mat->rows * mat->cols * sizeof(float));
    }
}
// 打印矩阵（调试用）
void printMatrix(const Matrix* mat) 
{
    for (int i = 0; i < mat->rows; ++i) 
    {
        for (int j = 0; j < mat->cols; ++j) 
        {
            printf("%.2f ", getElement(mat, i, j));
        }
        printf("\n");
    }
}

// int main() {
//     // 创建 state 矩阵 (4 行, 3 列)
//     Matrix state = createMatrix(4, 3);
    
//     // 初始化 state 为 0
//     for (int i = 0; i < state.rows * state.cols; ++i) {
//         state.data[i] = 0.0f;
//     }

//     // 创建 ins 列向量 (4 行, 1 列)
//     Matrix ins = createMatrix(4, 1);
//     for (int i = 0; i < ins.rows; ++i) {
//         ins.data[i] = (float)(i + 1);
//     }

//     printf("Before setting column:\n");
//     printMatrix(&state);

//     // 将 ins 插入到 state 的第 1 列 (colIndex = 1)
//     setColumn(&state, 1, &ins);

//     printf("\nAfter setting column:\n");
//     printMatrix(&state);

//     // 释放内存
//     freeMatrix(&state);
//     freeMatrix(&ins);

//     return 0;
// }
