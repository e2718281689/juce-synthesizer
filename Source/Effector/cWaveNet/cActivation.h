#ifndef _C_ACTIVATION_H
#define _C_ACTIVATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "matrix.h"

// 定义 TanhActivation 结构体
typedef struct 
{
    Matrix outs_internal; // 内部存储的输出
    Matrix* outs;         // 输出的指针
    int size;             // 输入/输出的大小
} TanhActivation;

// 初始化 TanhActivation
void TanhActivation_init(TanhActivation* layer, int size);

// 重置输出
void TanhActivation_reset(TanhActivation* layer);

// 执行前向传播（tanh 激活）
void TanhActivation_forward(TanhActivation* layer, const Matrix* ins);

// 释放内存
void TanhActivation_free(TanhActivation* layer);

#ifdef __cplusplus
}
#endif

#endif /* _C_ACTIVATION_H */

