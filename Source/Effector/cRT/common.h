#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <math.h>

float inner_product(float *vec, float *row, int size);
float sigmoid(float value);
float tanh_approximate(float x);
#endif /* _COMMON_H */
