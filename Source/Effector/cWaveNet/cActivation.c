#include "cActivation.h"

// 初始化 TanhActivation
void TanhActivation_init(TanhActivation* layer, int size) {
    layer->size = size;
    layer->outs_internal = createMatrix(size, 1);
    layer->outs = &layer->outs_internal;
    clearMatrix(&layer->outs_internal); // 输出置零
}

// 重置输出
void TanhActivation_reset(TanhActivation* layer) {
    clearMatrix(&layer->outs_internal);
}

// 执行前向传播（tanh 激活）
void TanhActivation_forward(TanhActivation* layer, const Matrix* ins) {
    matrixTanh(ins, layer->outs);
}

// 释放内存
void TanhActivation_free(TanhActivation* layer) {
    freeMatrix(&layer->outs_internal);
}