#include "cConv1d.h"



void cconv1d_init(cConv1d_Layer *layer, int in_sizet, int out_sizet, int kernel_size, int dilation_rate, int groups)
{
    layer->in_size = in_sizet;
    layer->out_size = out_sizet;
    layer->dilation = dilation_rate;
    layer->dilation_rate = dilation_rate;
    layer->kernel_size = kernel_size;
    layer->kernel_length = kernel_size;

    layer->filters_per_group = layer->in_size / groups;
    layer->channels_per_group = layer->out_size / groups;

    layer->state_size = (layer->kernel_size - 1) * dilation_rate + 1;

    layer->state = createMatrix(layer->in_size, layer->state_size);
    clearMatrix(&layer->state);

    layer->state_cols = createMatrix((int)(layer->filters_per_group), layer->kernel_size);
    clearMatrix(&layer->state_cols);

    // 新增 未作测试
    layer->weights = (Matrix *)malloc(layer->out_size * sizeof(Matrix));

    for (int i = 0; i < layer->out_size; ++i)
    {
        layer->weights[i] = createMatrix((int)(layer->filters_per_group), layer->kernel_size);
        clearMatrix(&layer->weights[i]);
    }

    layer->bias = createMatrix(layer->out_size, 1);
    clearMatrix(&layer->bias);
    
    layer->outs = createMatrix(layer->out_size, 1);
    clearMatrix(&layer->outs);

    layer->state_ptr = 0;
    layer->state_size = layer->state_size;
    layer->out_size = layer->out_size;

    layer->state_ptrs = createMatrix(layer->kernel_size, 1);
    clearMatrix(&layer->state_ptrs);
}
void cconv1d_reset(cConv1d_Layer *layer)
{
    clearMatrix(&layer->state);
    clearMatrix(&layer->state_cols);
    clearMatrix(&layer->state_ptrs);

    layer->state_ptr = 0;

}
void cconv1d_setWeights(cConv1d_Layer *layer, float *ws)
{
    for (int i = 0; i < layer->out_size; ++i)
    {
        for (int k = 0; k < layer->filters_per_group; ++k)
        {
            for (int j = 0; j < layer->kernel_size; ++j)
            {
                int index =  (i * layer->filters_per_group * layer->kernel_size + k * layer->kernel_size + j);
                float value = ws[index];
                setElement(&layer->weights[i], k, j, value);
            }
        }
    }
}



void cconv1d_setBias(cConv1d_Layer *layer, float *biasVals)
{
    for(int i = 0; i < layer->out_size; ++i)
    {
        // layer->bias(i) = biasVals[i];
        setElement(&layer->bias, i, 0, biasVals[i]);
    }
    
}
void cconv1d_Free(cConv1d_Layer *layer)
{
    freeMatrix(&layer->state);
    freeMatrix(&layer->state_cols);
    for (int i = 0; i < layer->out_size; ++i)
    {
        freeMatrix(&layer->weights[i]);
    }

    // 未作检查 
    free(&layer->weights);
    layer->weights = NULL;

    freeMatrix(&layer->bias);
    freeMatrix(&layer->outs);
    freeMatrix(&layer->state_ptrs);
}

void cconv1d_forward(cConv1d_Layer *layer, Matrix *ins)
{
    // state.col(state_ptr) = ins;
    setColumn(&layer->state, layer->state_ptr, ins);

    for (int k = 0; k < (layer->kernel_size); ++k)
    {
        // state_ptrs[k] = (state_ptr + state_size - k * dilation_rate) % state_size;

        setElement(&layer->state_ptrs, k, 0, (float)(((layer->state_ptr) + (layer->state_size) - ( k * (layer->dilation_rate))) % (layer->state_size)));
    }

    for (int k = 0; k < (layer->kernel_length); ++k)
    {
        // state_cols.col(k) = state.col(state_ptrs(k));

        copyColumn((&layer->state_cols), k, (&layer->state), (int)getElement((&layer->state_ptrs), k, 0));
    }

    for (int i = 0; i < layer->out_size; ++i)
    {
        // outs(i) = state_cols.cwiseProduct(weights[i]).sum() + bias(i);
        float sum = cwiseProductAndSum((&layer->state_cols), &(layer->weights[i]));
        setElement((&layer->outs), i, 0, sum + getElement((&layer->bias), i, 0));
    }

    (layer->state_ptr) = ((layer->state_ptr) == (layer->state_size) - 1 ? 0 : (layer->state_ptr) + 1); // iterate state pointer forwards
}