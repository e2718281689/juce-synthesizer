
#ifndef _CDENSET_H
#define _CDENSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Matrix.h"

typedef struct
{

    Matrix ins_internal;
    Matrix weights;
    Matrix outs;

    int in_size;
    int out_size;
    int dense_has_bias;

} cDenseT_Layer;

void cDenseT_forward(cDenseT_Layer* layer, Matrix* ins);
void cDenseT_init(cDenseT_Layer* layer, int in_sizet, int out_sizet, int has_bias);
void cDenseT_free(cDenseT_Layer* layer);

void cDenseT_setBias(cDenseT_Layer* layer, const float* bias_vals);
void cDenseT_setWeights(cDenseT_Layer* layer, const float* newWeights);
#ifdef __cplusplus
}
#endif

#endif /* _CDENSET_H */