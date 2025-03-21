#include "cconv1d.h"

typedef struct
{
    Matrix state;
    Matrix state_cols;
    Matrix weights[5];
    Matrix bias;
    Matrix outs;

    int in_size;
    int out_size;
    int dilation;
    int kernel_size;
    int kernel_length;
    int dilation_rate;
    float filters_per_group;
    float channels_per_group;

    int state_ptr;
    int state_size;

    Matrix state_ptrs;

    // float buf_state[IN_SIZE][STATE_SIZE];
    // float buf_state_cols[IN_SIZE][KERNEL_LENGTH];
    // float buf_weights[OUT_SIZE][IN_SIZE];
    // float buf_bias[OUT_SIZE];
    // float buf_outs[OUT_SIZE];
    // float buf_state_ptrs[KERNEL_LENGTH];

} Layer;

void cconv1d_init(Layer *layer, int in_sizet, int out_sizet, int kernel_size, int dilation_rate, int groups)
{

    layer->in_size = in_sizet;
    layer->out_size = out_sizet;
    layer->dilation = dilation_rate;
    layer->kernel_size = kernel_size;
    layer->kernel_length = kernel_size;

    layer->filters_per_group = layer->in_size / groups;
    layer->channels_per_group = layer->out_size / groups;

    layer->state_size = (layer->kernel_size - 1) * dilation_rate + 1;

    layer->state = createMatrix(layer->in_size, layer->out_size);
    layer->state_cols = createMatrix((int)(layer->filters_per_group), layer->kernel_size);
    for (int i = 0; i < layer->out_size; ++i)
    {
        layer->weights[i] = createMatrix((int)(layer->filters_per_group), layer->kernel_size);
    }
    layer->bias = createMatrix(layer->out_size, 1);
    layer->outs = createMatrix(layer->out_size, 1);
    layer->state_ptr = 0;
    layer->state_size = layer->state_size;
    layer->out_size = layer->out_size;
    layer->state_ptrs = createMatrix(layer->kernel_size, 1);
}
void cconv1d_setWeights(Layer *layer, float *ws, int il, int kl, int jl)
{
    for (int i = 0; i < layer->out_size; ++i)
    {
        for (int k = 0; k < layer->filters_per_group; ++k)
        {
            for (int j = 0; j < layer->kernel_size; ++j)
            {
                int index = (i * kl * jl) + (k * jl) + j;
                float value = ws[index];
                setElement(&layer->weights[i], k, j, value);
            }
        }
    }
}
void cconv1d_Free(Layer *layer)
{
    freeMatrix(&layer->state);
    freeMatrix(&layer->state_cols);
    for (int i = 0; i < layer->out_size; ++i)
    {
        freeMatrix(&layer->weights[i]);
    }
    freeMatrix(&layer->bias);
    freeMatrix(&layer->outs);
    freeMatrix(&layer->state_ptrs);
}

void cconv1d_forward(Layer *layer, Matrix *ins)
{
    // state.col(state_ptr) = ins;
    setColumn(&layer->state, layer->state_ptr, ins);

    for (int k = 0; k < (layer->kernel_size); ++k)
    {
        // state_ptrs[k] = (state_ptr + state_size - k * dilation_rate) % state_size;

        setElement(&layer->state_ptrs, k, 0, ((layer->state_ptr) + (layer->state_size) - k * (layer->dilation_rate)) % (layer->state_size));
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