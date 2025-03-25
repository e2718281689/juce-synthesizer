#ifndef _C_WAVENET_LAYER_H
#define _C_WAVENET_LAYER_H


#ifdef __cplusplus
extern "C" {
#endif

#include "cConv1d.h"
#include "cDenseT.h"
#include "cActivation.h"
#include "math.h"

typedef struct cWavenet_Layer
{

    int condition_size;
    int channels;
    int kernel_size;
    int dilation;

    cConv1d_Layer conv;
    cDenseT_Layer input_mixin;
    cDenseT_Layer _1x1;
    TanhActivation activation;

    Matrix outs;
    
} cWavenet_Layer;

void cWavenet_Layer_init(cWavenet_Layer *layer, int condition_size, int channels, int kernel_size, int dilation);
void cWavenet_Layer_free(cWavenet_Layer *layer);
void cWavenet_Layer_load_weights(cWavenet_Layer *layer, float* weights);
void cWavenet_Layer_forward(cWavenet_Layer *layer, Matrix *ins, Matrix *condition, Matrix *head_io);

#ifdef __cplusplus
}
#endif

#endif /* _C_WAVENET_LAYER_H */

