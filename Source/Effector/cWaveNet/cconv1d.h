
#ifndef CCONV1D_INCLUDED
#define CCONV1D_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "Matrix.h"

typedef struct
{
    Matrix state;
    Matrix state_cols;
    Matrix *weights;
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

} cConv1d_Layer;



void cconv1d_init(cConv1d_Layer *layer, int in_sizet, int out_sizet, int kernel_size, int dilation_rate, int groups);
void cconv1d_setWeights(cConv1d_Layer *layer, float *ws);
void cconv1d_setBias(cConv1d_Layer *layer, float *biasVals);
void cconv1d_Free(cConv1d_Layer *layer);
void cconv1d_forward(cConv1d_Layer *layer, Matrix *ins);

#ifdef __cplusplus
}
#endif

#endif // CCONV1D_INCLUDED