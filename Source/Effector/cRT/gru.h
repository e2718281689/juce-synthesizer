#ifndef _DEEP_GRU_H
#define _DEEP_GRU_H

#ifdef __cplusplus
extern "C" {
#endif

#define GRU_OUT_SIZE (9)
#define GRU_IN_SIZE (1)

#include <stdint.h>
#include <math.h>

void setWVals(float wVals[][27]);
void setUVals(float uVals[][27]);
void setBVals(float bVals[][27]);

void Gru_reset();
float* GRU_forward(float ins[GRU_IN_SIZE]);

#ifdef __cplusplus
}
#endif


#endif /* _DEEP_GRU_H */
