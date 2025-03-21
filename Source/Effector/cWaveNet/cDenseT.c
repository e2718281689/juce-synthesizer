#include "cDenseT.h"

void cDenseT_init(cDenseT_Layer* layer, int in_sizet, int out_sizet, int has_bias)
{
    layer->in_size = in_sizet;
    layer->out_size = out_sizet;
    layer->dense_has_bias = has_bias;

    // 如果有偏置项，则增加一行，否则保持原大小
    if (has_bias)
    {
        layer->ins_internal = createMatrix(in_sizet + 1, 1);
        clearMatrix(&layer->ins_internal);
        setElement(&layer->ins_internal, in_sizet, 0, 1.0f); // 设置 bias 项
    }
    else
    {
        layer->ins_internal = createMatrix(in_sizet, 1); // 不需要偏置项
        clearMatrix(&layer->ins_internal);
    }

    // 权重矩阵的大小也要根据是否有偏置项调整
    if (has_bias)
    {
        layer->weights = createMatrix(out_sizet, in_sizet + 1); // 额外加一列用于存储偏置项
        clearMatrix(&layer->weights);
    }
    else
    {
        layer->weights = createMatrix(out_sizet, in_sizet); // 无偏置项时权重矩阵为 (out_size, in_size)
        clearMatrix(&layer->weights);
    }

    // 输出矩阵的大小为 (out_size, 1)
    layer->outs = createMatrix(out_sizet, 1);
    clearMatrix(&layer->outs);


}

void cDenseT_forward(cDenseT_Layer* layer, Matrix* ins)
{
    if (layer->dense_has_bias == 1)
    {
        // 将输入矩阵拷贝到 ins_internal 并插入偏置项
        for (int i = 0; i < layer->in_size; ++i) 
        {
            setElement(&layer->ins_internal, i, 0, getElement(ins, i, 0));
        }

        // 偏置项的操作
        matrixMultiply((&layer->weights), (&layer->ins_internal), (&layer->outs));
    }
    else
    {
        // 没有偏置项的情况，直接使用输入矩阵与权重矩阵相乘
        matrixMultiply((&layer->weights), ins, (&layer->outs));
    }
}
void cDenseT_free(cDenseT_Layer* layer)
{
    freeMatrix(&layer->ins_internal);
    freeMatrix(&layer->weights);
    freeMatrix(&layer->outs);
}


// 设置权重：newWeights 是一个 float*，行优先存储
void cDenseT_setWeights(cDenseT_Layer* layer, const float* newWeights)
{
    for (int i = 0; i < layer->out_size; ++i) 
    {
        for (int k = 0; k < layer->in_size; ++k) 
        {
            setElement(&layer->weights, i, k, newWeights[i * layer->in_size + k]);
        }
    }
}

// 设置 bias：bias_vals 是一个 float*，大小为 out_size
void cDenseT_setBias(cDenseT_Layer* layer, const float* bias_vals)
{
    if (layer->dense_has_bias == 1) 
    {
        for (int i = 0; i < layer->out_size; ++i) 
        {
            setElement(&layer->weights, i, layer->in_size, bias_vals[i]);
        }
    }
}
