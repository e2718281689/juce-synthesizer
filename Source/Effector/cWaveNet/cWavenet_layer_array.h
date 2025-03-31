#ifndef _CWAVENET_LAYER_ARRAY_H
#define _CWAVENET_LAYER_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cWavenet_Layer.h"
#include "cDenseT.h"


typedef struct cWavenet_layer_array
{
    cWavenet_Layer* layers;
    int DilationsSum;
    int in_size;
    int condition_size;
    int head_size;
    int channels;
    int has_head_bias;
    
    Matrix head_outputs;
    Matrix layer_outputs;
    cDenseT_Layer rechannel;
    cDenseT_Layer head_rechannel;

} cWavenet_layer_array;

void cWavenet_layer_array_init(cWavenet_layer_array *layer, 
    int in_size, 
    int condition_size, 
    int head_size, 
    int channels, 
    int kernel_size, 
    int* DilationsSequence,
    int DilationsSum,
    int has_head_bias);

void cWavenet_layer_array_load_weights(cWavenet_layer_array *layer ,float** weights );
void cWavenet_layer_array_forward(cWavenet_layer_array* layer, Matrix* ins, Matrix* condition, Matrix* head_io);
void cWavenet_layer_array_free(cWavenet_layer_array *layer);
void cWavenet_layer_array_reset(cWavenet_layer_array* layer);
#ifdef __cplusplus
}
#endif

#endif /* _CWAVENET_LAYER_ARRAY_H */
// Compare this snippet from Source/Effector/cWaveNet/cWavenet_layer_array.h: