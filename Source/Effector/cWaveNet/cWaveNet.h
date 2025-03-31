#ifndef _CWAVENET_H
#define _CWAVENET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "matrix.h"

float WaveNet_forward(float pcm_in);
void WaveNet_init();
void WaveNet_load_weights(float *weights);
void WaveNet_free();
void WaveNet_reset();
#ifdef __cplusplus
}
#endif

#endif /* _CWAVENET_H */